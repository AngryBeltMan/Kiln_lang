#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
typedef enum {
    CLI_Run,
    CLI_Compile,
    CLI_Translate,
    CLI_New,
    CLI_None
} CliType;
typedef enum {
    O0,
    O1,
    O2,
    O3,
    Os,
} OptLevel;

typedef union {
    struct {
        OptLevel opt;
    } Run;
    struct {
        OptLevel opt;
    } Compile;
    struct {
        char* output_name;
    } Translate;
    struct { } New;
} CliUnion;
typedef struct {
    const char* file_name;
    CliType type;
    CliUnion info;
} CliSettings;

static CliSettings S_cli_settings;

static inline CliSettings CLISETTINGS_identify_cli_type(int argc,char** argv) {
    assert(argc != 1 && "No Arguments were given");

    assert(argc >= 3 && "CLI ERROR: Expected Name");

    CliSettings settings;
    settings.file_name = argv[2];

    if (!strcmp(argv[1], "run")) {
        settings.type = CLI_Run;
        settings.info.Run.opt = O0;
    } else if (!strcmp(argv[1], "compile")) {
        settings.type = CLI_Compile;
        settings.info.Compile.opt = O0;
    } else if (!strcmp(argv[1], "translate")) {
        settings.type = CLI_Translate;
        if (argc == 4) {
            settings.info.Translate.output_name = argv[3];
        } else {
            settings.info.Translate.output_name = "translated.c";
        }
    }
    return settings;
}

static inline void CLISETTINGS_parse(CliSettings *cli_settings) {
    if (cli_settings->type == CLI_Compile) {
        int result = system("gcc build.c");
        if (result) {
            fprintf(stderr, "Error: Could not compile build file\n");
        }
    }
    if (cli_settings->type == CLI_Run) {
        int result = system("gcc -Wall build.c && ./a.out");
        if (result) {
            fprintf(stderr, "Error: Could not run file\n");
        }
    }

}
