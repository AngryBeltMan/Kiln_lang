#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "compiler.h"
#include "parser.h"
#define TOKEN_CMP(ident,name,type) if (!strcmp(ident,name)) { return type; }

Compiler COMPILER_new() {
    Compiler compiler;
    FILE *fs = fopen(BUILDNAME, "w");
    compiler.file = fs;
    return compiler;
}
void expr(Expression e) {}

IdentType get_ident_type(char *ident) {
    TOKEN_CMP(ident, "println", IdentType_println);
    TOKEN_CMP(ident, "fprint",IdentType_printf);
    TOKEN_CMP(ident, "fprintln",IdentType_printf);
    TOKEN_CMP(ident, "let",IdentType_var_init);
    TOKEN_CMP(ident, "var",IdentType_varg_var);
    TOKEN_CMP(ident, "ref",IdentType_varg_var);
    TOKEN_CMP(&ident[0], "$",IdentType_vartype);
    return IdentType_varname;
}
void COMPILER_parse(Compiler *P_comp,Expressions *P_exprs) {
    fprintf(P_comp->file, "#include<stdio.h>\nint main(void) {\n");
    for (int i = 0; i < (P_exprs->size/sizeof(Expression)); ++i) {
        uint e = P_exprs->exprs[i].size;
        printf("size %i\n",e);
        Token token = P_exprs->exprs[i].tokens[0];
        switch (token.token_type) {
            case TokenType_Ident:
                switch (get_ident_type(token.value)) {
                    // println("hello world");
                    case IdentType_println:
                        printf("ident println\n");
                        fprintf(P_comp->file, "printf(\"");
                        for (int text = 3; text < (P_exprs->exprs[i].size/sizeof(Token)); ++text) {
                            Token *token = &P_exprs->exprs[i].tokens[text];
                            printf("%i\n",token->token_type);
                            if (token->token_type == 5) { printf("breaking\n"); break; }
                            printf("num %i\n",text);
                            printf("num %s\n",token->value);
                            if (token->value) {fprintf(P_comp->file,"%s",token->value);} else { fprintf(P_comp->file,"%c",token->character); }
                        }
                        fprintf(P_comp->file, "\");\n");
                        continue;
                        // let num $int = 10;
                    case IdentType_var_init:
                        printf("initing var\n");
                        Token name;
                        for (int name_token = 0; name_token < (P_exprs->exprs[i].size)/sizeof(Token); ++name_token) {

                        }
                        Token type = P_exprs->exprs[i].tokens[3];
                        printf("%s %s\n",type.value,name.value);
                        fprintf(P_comp->file,"%s %s;\n",type.value,name.value);
                        continue;
                    default:
                        continue;
                }
            default:
                printf("other type %i\n",token.token_type);
                continue;
        }
    }
    fprintf(P_comp->file, "return 0; \n }");
}
void COMPILER_drop(Compiler P_comp) {
    fclose(P_comp.file);
}
