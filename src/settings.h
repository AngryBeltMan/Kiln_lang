#ifndef SETTINGS
#define SETTINGS
#include "parser/parser.h"
#include <string.h>
#include "parser/tokens.h"
IdentType SETTINGS_get_identtype(Expression *P_expr);

#define SETTING_VALUE_PARSE(P_setting_type, token_name, opts, index, opts_value, opt_type) \
    if (!strcmp(P_setting_type.value, token_name)) { \
        opts->opts_value = opt_type; \
        index += 1; \
    }

#define SETTING_VAR_PARSE(setting_type,P_expr, keyword, index,setting, options, opt_value, skip)  \
    if (!strcmp(setting_type.value, keyword)) { \
        index = setting;\
        char *return_val = parse_setting_var(P_expr, &index); \
        assert(return_val != NULL && "ERROR: return setting var requires a return type value\n"); \
        options->opt_value = return_val; \
        index += skip; \
    }
#endif
