#ifndef FORLOOP
#include "../variables/variables.h"
#define FORLOOP
typedef struct ForLoopOpt {
    VarOpts var;
    int start;
    int end;
    int step;
} ForLoopOpt;

ForLoopOpt FORLOOP_new();

ForLoopOpt FORLOOP_parse(Expression *P_expr);

void FORLOOP_write_to_file(Compiler* P_comp, ForLoopOpt forloopopt);

#endif
