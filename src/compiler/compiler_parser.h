#ifndef COMP_PARSER
#include "../parser/parser.h"
#include "../hashmap/hashmap.h"
#include "compiler.h"
IdentType get_ident_type(char *ident);
void parse_expression_from_keyword(IdentType ident_type, Compiler *P_comp, Expressions *P_exprs, int index, Hashmap *P_hashmap);
#endif
