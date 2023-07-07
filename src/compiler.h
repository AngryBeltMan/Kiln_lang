#include <stdio.h>
#include "parser.h"
#include "contents.c"
#include "lib_tools.c"
#include "lib_tools.h"

#ifndef COMPILER
#define COMPILER
static int INITED_HEAP_ARRAY = 0;
// Name for the build file.
#define BUILDNAME "build.c"
typedef enum Module {
    MODULE_stdio,
    MODULE_stdlib,
    MODULE_string,
    // No OS required
    MODULE_math,
    MODULE_ctype,
} Module;

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
void COMPILER_parse(Compiler* P_comp,Expressions *P_exprs);

// frees all of the heap memory held in the compiler
void COMPILER_drop(Compiler comp);

void COMPILER_include_module(Compiler *P_comp);

void COMPILER_add_module(Compiler *P_comp, Module module);

void COMPILER_add_modules(Compiler *P_comp);

void COMPILER_write_to_file(Compiler *P_comp);

IdentType get_ident_type(char* ident);

Contents token_parse_expression_until(Expression* P_expr, int start, TokenType end_token);

Contents token_string_parse(Expression* P_expr,int start);

#endif


