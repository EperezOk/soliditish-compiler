#ifndef BUILTINS_H
#define BUILTINS_H

#include "../support/shared.h"

boolean isBuiltInFunction(char *name);
FunctionCallType getBuiltInType(char *name);

#endif
