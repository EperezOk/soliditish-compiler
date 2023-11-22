#include <string.h>
#include "type-checking.h"
#include "../domain-specific/builtins.h"

int typeAssignment(Assignment *assignment) {
    int assignable = typeAssignable(assignment->assignable);
    int expression = typeExpression(assignment->expression);
    switch (assignment->type) {
        case ASSIGNMENT_EXPRESSION:
            if (assignable == expression || (assignable == DATA_TYPE_ERC20 && expression == DATA_TYPE_ADDRESS)
                                        || (assignable == DATA_TYPE_ERC721 && expression == DATA_TYPE_ADDRESS)
                                        || (assignable == DATA_TYPE_ADDRESS && expression == DATA_TYPE_ERC20)
                                        || (assignable == DATA_TYPE_ADDRESS && expression == DATA_TYPE_ERC721)
                                        || (assignable == DATA_TYPE_BYTES && expression == DATA_TYPE_STRING)) {
                return assignable;
            } else {
                return -1;
            }
	    case ASSIGNMENT_ARRAY_INITIALIZATION:
            // TODO: check that all elements are of type assignable
            return 0;
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
                    return DATA_TYPE_INT;
                case DATA_TYPE_UINT:
                    return DATA_TYPE_UINT;
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
        case DATA_TYPE_FUNCTION:
            return DATA_TYPE_FUNCTION;
        case DATA_TYPE_ERC20:
        case DATA_TYPE_ERC721:
            if (strcmp(memberCall->method->identifier, "transfer") == 0)
                return DATA_TYPE_VOID;
            else
                return -1;

        default:
            return -1;
    }
}

int typeVariableDefinition(VariableDefinition *variableDefinition) {
    if(variableDefinition->expression == NULL && variableDefinition->functionCall == NULL)
        return variableDefinition->dataType->type;

    int expression = typeExpression(variableDefinition->expression);

    if (variableDefinition->dataType->type == expression)
        return variableDefinition->dataType->type;

    switch (variableDefinition->dataType->type) {
        case DATA_TYPE_ADDRESS:
            if (expression == DATA_TYPE_ERC20 || expression == DATA_TYPE_ERC721)
                return variableDefinition->dataType->type;
            else 
                return -1;
        case DATA_TYPE_ERC20:
        case DATA_TYPE_ERC721:
            if (expression == DATA_TYPE_ADDRESS)
                return variableDefinition->dataType->type;
            else 
                return -1;
        case DATA_TYPE_BYTES:
            if (expression == DATA_TYPE_STRING)
                return variableDefinition->dataType->type;
        default:
            return -1;
    }
}

int typeExpression(Expression *expression) {
    if (expression == NULL) return 0;

    int left_type = typeExpression(expression->left);
    int right_type = typeExpression(expression->right);
    switch (expression->type) {
    case EXPRESSION_ADDITION:
    case EXPRESSION_SUBTRACTION:
    case EXPRESSION_MULTIPLICATION:
    case EXPRESSION_DIVISION:
    case EXPRESSION_MODULO:
        if (left_type == DATA_TYPE_UINT && right_type == DATA_TYPE_UINT) {
            return DATA_TYPE_UINT;
        }
        else if (left_type == DATA_TYPE_INT && right_type == DATA_TYPE_INT) {
            return DATA_TYPE_INT;
        }
        else {
            return -1;
        }
    case EXPRESSION_EQUALITY:
    case EXPRESSION_INEQUALITY:
        if (left_type == right_type) {
            return DATA_TYPE_BOOLEAN;
        }
        else {
            return -1;
        }
    case EXPRESSION_LESS_THAN:
    case EXPRESSION_LESS_THAN_OR_EQUAL:
    case EXPRESSION_GREATER_THAN:
    case EXPRESSION_GREATER_THAN_OR_EQUAL:
        if (left_type == right_type) {
            if (left_type == DATA_TYPE_UINT || left_type == DATA_TYPE_INT) {
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
        if (left_type == DATA_TYPE_BOOLEAN && right_type == DATA_TYPE_BOOLEAN) {
            return DATA_TYPE_BOOLEAN;
        }
        else {
            return -1;
        }
    case EXPRESSION_NOT:
        if (typeExpression(expression) == DATA_TYPE_BOOLEAN) {
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
        return typeInteger(constant->value);
    case CONSTANT_VARIABLE:
        return typeAssignable(constant->variable);
    default:
        return -1;
    }
}

int typeAssignable(Assignable *assignable) {
    switch (assignable->type) {
    case ASSIGNABLE_VARIABLE:
        return typeVariable(assignable->identifier);
    case ASSIGNABLE_ARRAY:
        return typeExpression(assignable->arrayIndex);
    default:
        return -1;
    }
}

int typeVariable(char *identifier) {
    SymbolTableEntry *symbol;
    HASH_FIND_STR(state.symbolTable, identifier, symbol);
    if (symbol != NULL) {
        return symbol->type;
    }
    else {
        return -1;
    }
}

// check if it is string (scientificc notation), uint or int
int typeInteger(int value) {
   if (value < 0) {
       return DATA_TYPE_INT;
   } else {
       return DATA_TYPE_UINT;
   }
}

// TODO: CHECK IF THIS WORKS WITH SCIENTIFIC NOTATION
int typeScientificNotation(char *string) {
    // convert from scientific notation to int
    long long int number;
    // Use strtoll() to convert scientific notation string to a long long int
    number = strtoll(string, NULL, 10);
    return typeInteger((int)number);
}

