#include "kiln_module.h"
#include "../settings.h"
#include "../functions/functions.h"
#include "../contents.c"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
static inline void parse_module_features(Expression *P_expr, KilnModule *module,Hashmap *P_func_map, unsigned int token_index);
static inline unsigned int module_setting_parse(Expression *P_expr, KilnModule *module);

// SYNTAX: bring math#abs
KilnModule KILNMODULE_parse(Expression *P_expr, Hashmap *P_function, StringList* kiln_module_path) {
    assert(P_expr->size / 16 > 2 && "ERROR: bring function is too short" );
    KilnModule module = KILNMODULE_new();
    unsigned int token_index = module_setting_parse(P_expr, &module);
    assert(P_expr->tokens[token_index].token_type == TokenType_Ident && "ERROR: Expected ident for module name" );
    module.module_name = P_expr->tokens[token_index].value;
    Contents path = CONTENTS_new();
    if (!module.module_path) {
        CONTENTS_append_formatted(&path, "%s.c", module.module_name);
        module.module_path = path.file;
    } else {
        CONTENTS_append_str(&path, module.module_path);
        assert(path.size > 5 && "Error: Invalid file");
        CONTENTS_pop_back(&path, 4);
        CONTENTS_append(&path, 'c');
    };
    module.module_path = path.file;
    STRINGLIST_append(kiln_module_path, path.file);
    if (P_expr->size / 16 >= 4) {
        token_index += 2;
        parse_module_features(P_expr, &module,P_function, token_index );
    }
    return module;
}
static inline unsigned int module_setting_parse(Expression *P_expr, KilnModule *module) {
    int index = 0;
    // does not mutate any of the arguments
    // parses the expression to check any at signs
    int res = EXPRESSION_token_exist(P_expr, index, TokenType_AtSign);
    // this means an at sign token exists
    if (res != -1) {
        assert(P_expr->size > res + 1 && "ERROR: Expected value after '@' token");
        // get the value after the at sign
        Token setting_val = P_expr->tokens[res + 1];
        assert(setting_val.token_type == TokenType_Ident && "ERROR: Expected Ident after '@' token");
        /* SETTING_VAR_PARSE(setting_type, P_expr,"return",index,setting, opts,return_type, 0); */
        SETTING_VAR_PARSE(setting_val, P_expr, "path", index, res, module, module_path, 1);
    }
    return index;
}

static inline void parse_module_features(Expression *P_expr, KilnModule *module,Hashmap* P_func_map, unsigned int token_index) {
    if (P_expr->tokens[token_index].token_type == TokenType_Ident) {
        module->features[module->features_len] = P_expr->tokens[token_index].value;
    } else if (P_expr->tokens[token_index].token_type == TokenType_LeftParenthesis) {
        token_index += 1;
        do {
            if (P_expr->tokens[token_index].token_type != TokenType_Ident) {
                token_index += 1;
                continue;
            }
            module->features[module->features_len] = P_expr->tokens[token_index].value;
            hashmap_set(P_func_map, &(FuncMap){
                .module_name = module->module_name,
                .function = P_expr->tokens[token_index].value
            });
            ++module->features_len;
            if (module->features_len == module->max_features) {
                module->max_features *= 2;
                module->features = realloc(module->features, module->max_features);
                assert(module->features && "ERROR: Failed to realloc module features");
            }
            token_index += 1;
        } while (P_expr->tokens[token_index].token_type != TokenType_RightParenthesis);

    }

}

void KILNMODULE_write_to_file(Compiler *P_comp, KilnModule module) {
    CONTENTS_append_formatted(&P_comp->contents, "#include \"%s\"\n", module.module_path);
    for (int module_index = 0; module_index < module.features_len; ++module_index) {
        CONTENTS_append_formatted(&P_comp->contents, "#define ___MODULE_%sBRANCH__%s\n",module.module_name, module.features[module_index]);
    }
    free(module.features);
}
