#ifndef __GET_TYPES_HEADER__
#define __GET_TYPES_HEADER__

#include <stdlib.h> // strtoll
#include "../support/shared.h"
#include "./symbol-table.h"

int typeAssignment(Assignment *assignment);
int typeMathAssignment(MathAssignment *mathAssignment);
int typeFunctionCall(FunctionCall *functionCall);
int typeMemberCall(MemberCall *memberCall);
int typeVariableDefinition(VariableDefinition *variableDefinition);
int typeExpression(Expression * expression);
int typeFactor(Factor * factor);
int typeConstant(Constant * constant);
int typeAssignable(Assignable * assignable);
int typeVariable(char * identifier);
int typeInteger(int value);

#endif