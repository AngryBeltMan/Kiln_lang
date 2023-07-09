#ifndef CALLS
#define CALLS
#include "../compiler.h"
typedef enum Arithmetic {
    ARITHMETIC_addition,
    ARITHMETIC_subtraction,
    ARITHMETIC_multiplication,
    ARITHMETIC_division,
    ARITHMETIC_power,
    ARITHMETIC_root,
} Arithmetic;
typedef struct CallsType {
    enum {
        CALLTYPEARM_arth,
        CALLTYPEARM_fn_call,
        CALLTYPEARM_value,
        CALLTYPEARM_var_assign,

    } CallTypeArm;
    union {
        struct {
            char* var_name;
            Arithmetic arth;
            int input;
        } Arth;
        struct {
            char* fn_name;
            char* args;
        } FnCall;
        struct {
            char* var_name;
            struct CallsType *value;
        } VarAssign;
        struct {
            char* value;
        } Value;
    } Type;
} CallsType;

CallsType CALLSTYPE_parse(Expression *P_expr, int start_index);
void CALLSTYPE_write_to_file(Compiler *P_comp, CallsType call_type);
void CALLSTYPE_free(CallsType call_type);
#endif
