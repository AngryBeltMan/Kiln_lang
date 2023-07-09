#ifndef IFSTATEMENT
#include "../compiler/compiler.h"
#include "../parser/parser.h"
#define IFSTATEMENT

typedef struct IfStatementOpt {
    char* value;
} IfStatementOpt;

IfStatementOpt IFSTATEMENT_parse(Expression* P_expr);

void IFSTATEMENT_write_to_file(Compiler* P_comp,IfStatementOpt statement);

#endif
