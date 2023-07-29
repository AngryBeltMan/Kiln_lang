#include <stdio.h>
#include "../parser/parser.h"
#include "../hashmap/hashmap.h"
#include "../contents.c"
#include "../lib_tools.c"
#include "../lib_tools.h"

#ifndef COMPILER
#define COMPILER
// Name for the build file.
#define BUILDNAME "build.c"

typedef struct Compiler {
    // The file that is going to be written to.
    FILE *file;
    Contents contents;
    /// vector of modules that will be included
    Vector modules;

} Compiler;

// Creates a new compiler that will translate the kit file into a c file.
Compiler COMPILER_new();

// parse the file expressions into a c file
void COMPILER_parse(Compiler* P_comp,Expressions *P_exprs, Hashmap *P_hashmap);

// frees all of the heap memory held in the compiler
void COMPILER_drop(Compiler comp);

void COMPILER_write_to_file(Compiler *P_comp);

IdentType get_ident_type(char* ident);

Contents token_parse_expression_until(Expression* P_expr, int start, TokenType end_token);

Contents token_string_parse(Expression* P_expr,int start);

#endif


