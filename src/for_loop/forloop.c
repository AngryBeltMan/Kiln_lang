#ifndef FORLOOPIMPL
#define FORLOOPIMPL
#include "forloop.h"
#include "../variables/variables.h"
#include "../lib_tools.h"
#include "../contents.c"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

ForLoopOpt FORLOOP_new() {
    VarOpts var = VAROPTS_new();
    ForLoopOpt forloopopts;
    forloopopts.var = var;
    forloopopts.end = 0;
    forloopopts.step = 0;
    forloopopts.start = 0;
    return forloopopts;
}
// for i $int 0 -> 10 {}
// for i $int 0 <- 10 {}
ForLoopOpt FORLOOP_parse(Expression *P_expr) {
    ForLoopOpt forloopopt = FORLOOP_new();
    VarOpts var_opt = VAROPTS_new();

    char* name = P_expr->tokens[1].value;
    if (name == NULL) { name = "_i"; }
    char* type = P_expr->tokens[3].value;
    if (type == NULL) { type = "int"; }
    char* start = P_expr->tokens[4].value;
    assert(start != NULL && "ERROR: for loop left hand side value not given %s\n");
    int skip = ((EXPRESSION_token_exist(P_expr, 5,TokenType_RightArrow) != -1) ? 1 : -1);
    char* end = P_expr->tokens[6].value;
    assert(end != NULL && "ERROR: for loop right hand side value not given");
    var_opt.name = name;
    var_opt.type = type;
    forloopopt.start = str_to_int(start);
    forloopopt.end = str_to_int(end);
    forloopopt.step = skip;
    forloopopt.var = var_opt;
    return forloopopt;
}

void FORLOOP_write_to_file(Compiler* P_comp, ForLoopOpt opts) {
    CONTENTS_append_formated(&P_comp->contents,"for (%s %s = %i;",opts.var.type,opts.var.name,opts.start);
    if (opts.start == 1) {
        CONTENTS_append_formated(&P_comp->contents,"%s > %i;",opts.var.name,opts.end);
    } else {
        CONTENTS_append_formated(&P_comp->contents,"%s < %i;",opts.var.name,opts.end);
    }
    CONTENTS_append_formated(&P_comp->contents,"%s += %i) {\n",opts.var.name,opts.step);
}

#endif
