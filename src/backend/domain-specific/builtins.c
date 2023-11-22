#include "builtins.h"
#include "../semantic-analysis/abstract-syntax-tree.h"

const char *BUILT_IN_FUNCTIONS[] = {
    "transfer",
    "balanceOf",
    "log",
    "mint",
    "createProxyTo"
};

// Note: built-ins starting with `BUILT_IN` are transformed during code generation
const FunctionCallType BUILT_IN_FUNCTION_TYPES[] = {
    FUNCTION_CALL_WITH_ARGS,
    BUILT_IN_BALANCE_OF,
    BUILT_IN_LOG,
    FUNCTION_CALL_WITH_ARGS,
    BUILT_IN_CREATE_PROXY_TO
};

boolean isBuiltInFunction(char *name) {
    for (int i = 0; i < sizeof(BUILT_IN_FUNCTIONS) / sizeof(char *); ++i) {
        if (strcmp(BUILT_IN_FUNCTIONS[i], name) == 0) {
            return true;
        }
    }
    return false;
}

FunctionCallType getBuiltInType(char *name) {
    for (int i = 0; i < sizeof(BUILT_IN_FUNCTIONS) / sizeof(char *); ++i) {
        if (strcmp(BUILT_IN_FUNCTIONS[i], name) == 0) {
            return BUILT_IN_FUNCTION_TYPES[i];
        }
    }
    return -1;
}
