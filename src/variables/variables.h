#include "../compiler/compiler.h"

#ifndef VARIABLESETTINGS
#define VARIABLESETTINGS
typedef struct VarOpts {
    char* name;
    char* type;
    char* value;
    int heap_allocated;
    int const_val;
    int static_val;
    int size_mult;
    int string;
} VarOpts;

void variable_value_parse(int* ident_token,Expression* P_expr,Compiler* P_comp,VarOpts* P_varopts);

VarOpts VAROPTS_expression_parse(Expression* P_expr, Compiler *P_comp);

char* parse_setting_var(Expression *P_expr, int *ident_token);

void VAROPTS_create_var(VarOpts varopts,Compiler* P_comp);

char* get_var_ident(Expression* P_expr, int* index);

// returns an incompleted Varopts struct
VarOpts VAROPTS_new();
#endif

