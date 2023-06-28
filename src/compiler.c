#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "compiler.h"
#include "contents.c"
#include "parser.h"
#define TOKEN_CMP(ident,name,type) if (!strcmp(ident,name)) { return type; }

Compiler COMPILER_new() {
    Compiler compiler;
    FILE *fs = fopen(BUILDNAME, "w");
    compiler.file = fs;
    return compiler;
}
void expr(Expression e) {}

Contents token_string_parse(Expression* P_expr,int start) {
    Contents string = CONTENTS_new();
    for (int text = start; text < (P_expr->size/sizeof(Token)); ++text) {
        Token *token = &P_expr->tokens[text];
        if (token->token_type == 5) { break; }
        if (token->value) {
            CONTENTS_append_str(&string,token->value);
        } else {
            CONTENTS_append(&string,token->character);
        }
    }
    return string;
}

void write_print(Compiler* P_comp,Expression* P_expr,char* formatting) {
    fprintf(P_comp->file, "printf(\"");
    Contents contents = token_string_parse(P_expr, 3);
    fprintf(P_comp->file, "%s%s",contents.file,formatting);
    free(contents.file);
    contents.file = NULL;
}

IdentType get_ident_type(char *ident) {
    TOKEN_CMP(ident, "print",IdentType_print);
    TOKEN_CMP(ident, "println", IdentType_println);
    TOKEN_CMP(ident, "fprint",IdentType_printf);
    TOKEN_CMP(ident, "fprintln",IdentType_printf);
    TOKEN_CMP(ident, "let",IdentType_var_init);
    TOKEN_CMP(ident, "var",IdentType_varg_var);
    TOKEN_CMP(ident, "ref",IdentType_varg_var);
    TOKEN_CMP(&ident[0], "$",IdentType_vartype);
    return IdentType_varname;
}
void variable_value_parse(int ident_token,Expression* P_expr,Compiler* P_comp) {
    for (; ident_token < (P_expr->size)/sizeof(Token); ++ident_token) {
        Token value = P_expr->tokens[ident_token];
        switch (value.token_type) {
            case TokenType_DoubleQuote:
                printf("");
                Contents value_str = token_string_parse(P_expr, ident_token + 1);
                printf("value str %s\n",value_str.file);
                fprintf(P_comp->file, "= \"%s\"",value_str.file);
                free(value_str.file);
                value_str.file = NULL;
                return;
            case TokenType_Ident:
                // TODO check if it is a function call
                if (!strcmp(value.value, "NONE")) { break; }
                if (!strcmp(value.value, "ref")) {  }
                if (!strcmp(value.value, "@heap")) {  }
                printf("var value %s\n",value.value);
                fprintf(P_comp->file, "= %s",value.value);
                continue;
            default:
                continue;
        }
    }
}

void COMPILER_parse(Compiler *P_comp,Expressions *P_exprs) {
    fprintf(P_comp->file, "#include<stdio.h>\n#include<stdlib.h>\n int main(void) {\n");
    for (int i = 0; i < (P_exprs->size/sizeof(Expression)); ++i) {
        uint e = P_exprs->exprs[i].size;
        Token token = P_exprs->exprs[i].tokens[0];
        switch (token.token_type) {
            case TokenType_Ident:
                switch (get_ident_type(token.value)) {
                    // println("hello world");
                    case IdentType_println:
                        write_print(P_comp, &P_exprs->exprs[i], "\\n\");\n");
                        continue;
                    case IdentType_print:
                        write_print(P_comp, &P_exprs->exprs[i], "\");\n");
                        continue;
                        // let num $int = 10;
                    case IdentType_var_init:
                        printf("initing var\n");
                        Token name;
                        int ident_token = 2;
                        // get the name of the variable
                        for (; ident_token < (P_exprs->exprs[i].size)/sizeof(Token); ++ident_token) {
                            printf("index %i\n",ident_token);
                            if (P_exprs->exprs[i].tokens[ident_token].token_type == 0) {
                                name = P_exprs->exprs[i].tokens[ident_token];
                                break;
                            }
                        }
                        ++ident_token;
                        Token type;
                        // get the type of the variable
                        for (; ident_token < (P_exprs->exprs[i].size)/sizeof(Token); ++ident_token) {
                            printf("index a %i\n",ident_token);
                            if (P_exprs->exprs[i].tokens[ident_token].token_type == 7) {
                                type = P_exprs->exprs[i].tokens[ident_token + 1];
                                break;
                            }
                        }
                        ident_token += 2;
                        printf("index b %i\n",ident_token);
                        fprintf(P_comp->file,"%s %s",type.value,name.value);
                        // get the value of the variable
                        variable_value_parse(ident_token, &P_exprs->exprs[i], P_comp);
                        fprintf(P_comp->file,";\n");
                        printf("%s %s\n",type.value,name.value);
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
