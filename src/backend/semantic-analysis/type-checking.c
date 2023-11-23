#include <string.h>
#include "type-checking.h"
#include "../domain-specific/builtins.h"

int typeAssignment(Assignment *assignment) {
    int assignable = typeAssignable(assignment->assignable);
    int expression = typeExpression(assignment->expression);

    Factor *factor = assignment->expression->factor;
    switch (assignment->type) {
        case ASSIGNMENT_EXPRESSION:
            if (assignable == expression || (assignable == DATA_TYPE_BYTES && assignment->expression->factor != NULL && assignment->expression->factor->constant != NULL && assignment->expression->factor->constant->type == CONSTANT_STRING)
                                        || (assignable == DATA_TYPE_INT && assignment->expression->factor != NULL && assignment->expression->factor->constant != NULL && assignment->expression->factor->constant->type == CONSTANT_INTEGER)) {
                return assignable;
            } else {
                return -1;
            }
        case ASSIGNMENT_FUNCTION_CALL:
            if (isBuiltInFunction(assignment->functionCall->identifier))
                return DATA_TYPE_VOID;
            else
                return -1;
        default:
            return -1;
    }
}

int typeMathAssignment(MathAssignment *mathAssignment) {
    switch(mathAssignment->type) {
        case MATH_ASSIGNMENT_OPERATOR:
            switch (typeAssignable(mathAssignment->variable)) {
                case DATA_TYPE_INT:
                    break;
                case DATA_TYPE_UINT:
                    break;
                default:
                    return -1;
            }

            switch(typeExpression(mathAssignment->expression)) {
                case DATA_TYPE_INT:
                case DATA_TYPE_UINT:
                    return DATA_TYPE_VOID;
                default:
                    return -1;
            }
        case MATH_ASSIGNMENT_INCREMENT:
        case MATH_ASSIGNMENT_DECREMENT:
            switch (typeAssignable(mathAssignment->variable)) {
                case DATA_TYPE_INT:
                    return DATA_TYPE_INT;
                case DATA_TYPE_UINT:
                    return DATA_TYPE_UINT;
                default:
                    return -1;
            }
        default:
            return -1;
    }
}

int typeFunctionCall(FunctionCall *functionCall) {
    switch(functionCall->type) {
        case FUNCTION_CALL_NO_ARGS:
        case FUNCTION_CALL_WITH_ARGS:
            if (isBuiltInFunction(functionCall->identifier))
                return DATA_TYPE_FUNCTION;

            switch (typeVariable(functionCall->identifier)) {
                case DATA_TYPE_FUNCTION:
                    return DATA_TYPE_FUNCTION;
                default:
                    return -1;
            }
	    case BUILT_IN_TRANSFER_ETH:
	    case BUILT_IN_BALANCE:
	    case BUILT_IN_LOG:
	    case BUILT_IN_CREATE_PROXY_TO:
            return DATA_TYPE_FUNCTION;
        default:
            return -1;
    }
}

int typeMemberCall(MemberCall *memberCall) {
    int assignable = typeAssignable(memberCall->instance);

    switch(assignable) {
        case DATA_TYPE_ERC20:
        case DATA_TYPE_ERC721:
            if (strcmp(memberCall->method->identifier, "transfer") == 0)
                return DATA_TYPE_BOOLEAN;
            else
                return -1;

        default:
            return -1;
    }
}

int typeVariableDefinition(VariableDefinition *variableDefinition) {
    if(variableDefinition->type == VARIABLE_DEFINITION_DECLARATION)
        return variableDefinition->dataType->type;

    int expression = typeExpression(variableDefinition->expression);

    if (variableDefinition->dataType->type == expression)
        return variableDefinition->dataType->type;

    Factor *factor = variableDefinition->expression->factor;
    
    switch (variableDefinition->dataType->type) {
        case DATA_TYPE_ERC20:
        case DATA_TYPE_ERC721:
            if (expression == DATA_TYPE_ADDRESS)
                return variableDefinition->dataType->type;
            else 
                return -1;
        case DATA_TYPE_BYTES:
            if (factor != NULL && factor->constant != NULL && factor->constant->type == CONSTANT_STRING)
                return variableDefinition->dataType->type;
        case DATA_TYPE_INT:
            if (factor != NULL && factor->constant != NULL && factor->constant->type == CONSTANT_INTEGER)
                return variableDefinition->dataType->type;
        default:
            return -1;
    }
}

int typeExpression(Expression *expression) {
    if (expression == NULL) return 0;

    int leftType = typeExpression(expression->left);
    int rightType = typeExpression(expression->right);
    switch (expression->type) {
    case EXPRESSION_ADDITION:
    case EXPRESSION_SUBTRACTION:
    case EXPRESSION_MULTIPLICATION:
    case EXPRESSION_DIVISION:
    case EXPRESSION_MODULO:
    case EXPRESSION_EXPONENTIATION:
        if (leftType == DATA_TYPE_UINT && rightType == DATA_TYPE_UINT) {
            return DATA_TYPE_UINT;
        } else if ((leftType == DATA_TYPE_INT && rightType == DATA_TYPE_INT)
                    || (leftType == DATA_TYPE_UINT && rightType == DATA_TYPE_INT)
                    || (leftType == DATA_TYPE_INT && rightType == DATA_TYPE_UINT)){
            return DATA_TYPE_INT;
        } else {
            return -1;
        }
    case EXPRESSION_EQUALITY:
    case EXPRESSION_INEQUALITY:
        if (leftType == rightType) {
            return DATA_TYPE_BOOLEAN;
        }
        else {
            return -1;
        }
    case EXPRESSION_LESS_THAN:
    case EXPRESSION_LESS_THAN_OR_EQUAL:
    case EXPRESSION_GREATER_THAN:
    case EXPRESSION_GREATER_THAN_OR_EQUAL:
        if (leftType == rightType || (leftType == DATA_TYPE_INT && rightType == DATA_TYPE_UINT) || (leftType == DATA_TYPE_UINT && rightType == DATA_TYPE_INT)) {
            if (leftType == DATA_TYPE_UINT || leftType == DATA_TYPE_INT) {
                return DATA_TYPE_BOOLEAN;
            }
            else {
                return -1;
            }
        }
        else {
            return -1;
        }
    case EXPRESSION_AND:
    case EXPRESSION_OR:
        if (leftType == DATA_TYPE_BOOLEAN && rightType == DATA_TYPE_BOOLEAN) {
            return DATA_TYPE_BOOLEAN;
        }
        else {
            return -1;
        }
    case EXPRESSION_NOT:
        if (typeExpression(expression->right) == DATA_TYPE_BOOLEAN) {
            return DATA_TYPE_BOOLEAN;
        }
        else {
            return -1;
        }
    case EXPRESSION_FACTOR:
        return typeFactor(expression->factor);
    default:
        return -1;
    }
}

int typeFactor(Factor *factor) {
    switch (factor->type) {
        case FACTOR_EXPRESSION:
            return typeExpression(factor->expression);
        case FACTOR_CONSTANT:
            return typeConstant(factor->constant);
        default:
            return -1;
    }
}


int typeConstant(Constant *constant) {
    switch (constant->type) {
    case CONSTANT_INTEGER:
        return typeInteger(constant->value);
    case CONSTANT_BOOLEAN:
        return DATA_TYPE_BOOLEAN;
    case CONSTANT_STRING:
        return DATA_TYPE_STRING;
    case CONSTANT_ADDRESS:
        return DATA_TYPE_ADDRESS;
    case CONSTANT_SCIENTIFIC_NOTATION:
        if (constant->string[0] == '-')
            return DATA_TYPE_INT;
        else
            return DATA_TYPE_UINT;
    case CONSTANT_VARIABLE:
        return typeAssignable(constant->variable);
    default:
        return -1;
    }
}

int typeAssignable(Assignable *assignable) {
    int typeArrayIndex = typeExpression(assignable->arrayIndex);
    switch (assignable->type) {
    case ASSIGNABLE_VARIABLE:
        return typeVariable(assignable->identifier);
    case ASSIGNABLE_ARRAY:
        if (typeArrayIndex == -1 || typeArrayIndex != DATA_TYPE_UINT)
            return -1;
        return typeVariable(assignable->identifier);
    default:
        return -1;
    }
}

int typeVariable(char *identifier) {
    return getSymbolType(identifier);
}

// check if it is string (scientificc notation), uint or int
int typeInteger(int value) {
   if (value < 0) {
       return DATA_TYPE_INT;
   } else {
       return DATA_TYPE_UINT;
   }
}

