#include "builtins.h"

const char *BUILT_IN_FUNCTIONS[] = {
    "transfer",
};

boolean isBuiltInFunction(char *name) {
    for (int i = 0; i < sizeof(BUILT_IN_FUNCTIONS) / sizeof(char *); ++i) {
        if (strcmp(BUILT_IN_FUNCTIONS[i], name) == 0) {
            return true;
        }
    }
    return false;
}
