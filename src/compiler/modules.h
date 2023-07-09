#ifndef MODULES
#define MODULES
#include "compiler.h"
#include "../lib_tools.h"


typedef enum Module {
    MODULE_stdio,
    MODULE_stdlib,
    MODULE_string,
    // No OS required
    MODULE_math,
    MODULE_ctype,
} Module;

void COMPILER_add_module(Compiler *P_comp, Module module);

void MODULES_write_to_file(Compiler *P_comp);

#endif
