#include <stdlib.h>
#include "../contents.c"
#include "../parser/parser.h"
#ifndef IMPORTS
#define IMPORTS

typedef struct {
    char* module_name;
    char* path;
    char* module_path;
} ImportSettings;

static inline ImportSettings IMPORTS_new() {
    ImportSettings module;
    module.path = NULL;
    module.module_path = NULL;
    module.path = NULL;
    return module;
}
ImportSettings IMPORTS_parse_module(Expression *P_expr);

void IMPORTS_write_to_file(Contents *P_con, ImportSettings settings);
#endif
