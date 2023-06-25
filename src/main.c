#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "compiler.h"
#include "parser.h"
#include "parser.c"
#include "contents.c"
#include "compiler.c"

int main(void) {
    FILE *main_file;
    main_file = fopen("main.kit", "r");
    Expressions exprs =  EXPRESSIONS_from_file(main_file);
    Compiler comp = COMPILER_new();

    COMPILER_parse(&comp,&exprs);

    EXPRESSIONS_drop(exprs);
    COMPILER_drop(comp);
    free(main_file);
    return 0;
}
