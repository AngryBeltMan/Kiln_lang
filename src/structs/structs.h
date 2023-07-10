#ifndef STRUCTS
#define STRUCTS
#include "../parser/parser.h"
#include "../compiler/compiler.h"
typedef struct StructOpt {
    char* struct_name;
    char* struct_args;
} StructOpt;
StructOpt STRUCT_new();
StructOpt STRUCT_parse_file(Expression *P_expr);
void STRUCT_write_to_file(Compiler *P_comp, StructOpt opts);
#endif
