#ifndef FORLOOPIMPL
#define FORLOOPIMPL
#include "forloop.h"
#include "../variables/variables.h"
#include "../lib_tools.h"
#include "../contents.c"
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

    int forloop_index = 2;
    char* name = get_var_ident(P_expr, &forloop_index);
    if (name == NULL) { name = "_i"; }
    ++forloop_index;
    char* type = get_var_ident(P_expr, &forloop_index);
    if (type == NULL) { type = "int"; }
    ++forloop_index;
    char* start = get_var_ident(P_expr, &forloop_index);
    if (start == NULL) { printf("ERROR: for loop left hand side value not given %s\n",__FUNCTION__); abort(); }
    ++forloop_index;
    int skip = 0;
    if (EXPRESSION_token_exist(P_expr, forloop_index,TokenType_RightArrow) != -1) { skip = 1; }
    if (EXPRESSION_token_exist(P_expr, forloop_index,TokenType_LeftArrow) != -1) { skip = -1; }
    if (skip == 0) { printf("Error: could not find arrow %s\n",__FUNCTION__); }
    char* end = get_var_ident(P_expr, &forloop_index);
    if (end == NULL) { printf("ERROR: for loop right hand side value not given %s\n",__FUNCTION__); abort(); }
    printf("end = %s\n",end);
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
