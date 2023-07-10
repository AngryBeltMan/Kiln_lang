#include "modules.h"
#include <stdio.h>
#ifndef MODULESIMPL
void COMPILER_add_module(Compiler *P_comp, Module module) {
    int *mod = malloc(sizeof(int));
    *mod = module;
    VECTORappend(&P_comp->modules, mod);
}
void MODULES_write_to_file(Compiler* P_comp) {
    for (int module = 0; module < VECTOR_len(&P_comp->modules); ++module) {
        printf("module\n");
        switch (*(int *)VECTOR_index(&P_comp->modules, module)) {
            case MODULE_stdlib:
                fprintf(P_comp->file, "#include<stdlib.h>\n");
                continue;
            case MODULE_stdio:
                fprintf(P_comp->file, "#include<stdio.h>\n");
                continue;
            case MODULE_string:
                fprintf(P_comp->file, "#include<string.h>\n");
                continue;
            default:
                printf("other module %i\n",*(int *)VECTOR_index(&P_comp->modules, module));
                continue;
        }
    }
}
#endif
