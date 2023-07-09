#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "contents.c"
#include "parser/parser.h"
#include "parser/parser.c"
#include "compiler.h"
#include "compiler.c"
int main(void) {
    FILE *main_file;
    main_file = fopen("main.kiln", "r");

    Expressions exprs =  EXPRESSIONS_from_file(main_file);

    Compiler comp = COMPILER_new();


    COMPILER_parse(&comp,&exprs);
    COMPILER_write_to_file(&comp);

    EXPRESSIONS_drop(exprs);
    COMPILER_drop(comp);

    fclose(main_file);
    return 0;
}
