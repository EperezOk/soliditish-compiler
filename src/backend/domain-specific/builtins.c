#include "builtins.h"
#include "../semantic-analysis/abstract-syntax-tree.h"

const char *BUILT_IN_FUNCTIONS[] = {
    // ERC20 and ERC721 members
    "transfer",
    // Built-in functions
    "transferEth",
    "balance",
    "log",
    "createProxyTo"
};

const FunctionCallType BUILT_IN_FUNCTION_TYPES[] = {
    // ERC20 and ERC721 members
    FUNCTION_CALL_WITH_ARGS,
    // Built-in functions (these are transformed during code generation)
    BUILT_IN_TRANSFER_ETH,
    BUILT_IN_BALANCE,
    BUILT_IN_LOG,
    BUILT_IN_CREATE_PROXY_TO
};

boolean isBuiltInFunction(char *name) {
    for (int i = 0; i < sizeof(BUILT_IN_FUNCTIONS) / sizeof(char *); ++i) {
        if (strcmp(BUILT_IN_FUNCTIONS[i], name) == 0) {
            if (strcmp("createProxyTo", name) == 0) state.libraries[LIBRARY_CLONES] = true;
            else if (strcmp("log", name) == 0) state.libraries[LIBRARY_CONSOLE] = true;
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
