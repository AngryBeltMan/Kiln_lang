#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../compiler/compiler.h"
#ifndef KILNMODULE
#define KILNMODULE
// SYNTAX: bring std#(cool,awesome);
// EXPANDED:
    // #include "std.c"
    // #include "std.h"
    // #define __STD__COOL
//
typedef struct {
    char* module_path;
    char* module_name;
    char** features;
    unsigned int features_len;
    unsigned int max_features;
} KilnModule;

/// stores the paths of the modules so those files can be compiled later
typedef struct {
    char** modules_path;
} ModuleArray;

static inline KilnModule KILNMODULE_new() {
    char** features = malloc(8);
    assert(features && "ERROR: Kiln module failed to malloc");
    KilnModule module =  {
        .module_path = NULL,
        .module_name = NULL,
        .features = features,
        .features_len = 0,
        .max_features = 1
    };
    return module;
}

// gets the module path name and needed features given a expression
KilnModule KILNMODULE_parse(Expression *P_expr, Hashmap *P_func_map, StringList* kiln_module_path);

// writes to the build file
void KILNMODULE_write_to_file(Compiler *P_comp, KilnModule module);

static inline void KILNMODULES_translate(StringList kiln_modules) {
    for (int module = 0; module < kiln_modules.index; ++module) {
        char* path = kiln_modules.items[module];
        Contents input_path = CONTENTS_from(path);
        Contents command = CONTENTS_new();
        CONTENTS_pop_back(&input_path, 1);
        CONTENTS_append_str(&input_path, "kiln");
        CONTENTS_append_formatted(&command,  "./kiln.out translate %s %s",input_path.file, path );
        free(path);
        free(input_path.file);
        int res = system(command.file);
        /* assert(res && "ERROR: failed to compile other files"); */
        CONTENTS_drop(command);
    }
    /* VECTOR_drop(kiln_modules); */
}

#endif
