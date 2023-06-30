#include <stdio.h>
#include "parser.h"
#include "contents.c"

#ifndef COMPILER
#define COMPILER
// Name for the build file.
#define BUILDNAME "build.c"
typedef struct Compiler {
    // The file that is going to be written to.
    FILE *file;
    Contents contents;

} Compiler;
typedef enum Module {
    MODULE_stdio,
    MODULE_stdlib,
    // No OS required
    MODULE_math,
    MODULE_ctype,
} Module;

// Creates a new compiler that will translate the kit file into a c file.
Compiler COMPILER_new();

// parse the file expressions into a c file
void COMPILER_parse(Compiler* P_comp,Expressions *P_exprs);

// frees all of the heap memory held in the compiler
void COMPILER_drop(Compiler comp);

void COMPILER_include_module(Compiler *P_comp);

void COMPILER_add_module(Compiler *P_comp, Module module);

void COMPILER_write_to_file(Compiler *P_comp);

IdentType get_ident_type(char*ident);

#endif


