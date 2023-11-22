#ifndef __GET_TYPES_HEADER__
#define __GET_TYPES_HEADER__

#include <stdlib.h> // strtoll
#include "shared.h"
#include "../../backend/semantic-analysis/symbol-table.h"

int typeExpression(Expression * expression);
int typeFactor(Factor * factor);
int typeConstant(Constant * constant);
int typeAssignable(Assignable * assignable);
int typeVariable(char * identifier);
int typeInteger(int value);
int typeScientificNotation(char * value);

#endif