#include "get-type.h"

int typeExpression(Expression *expression) {
    switch (expression->type) {
    case EXPRESSION_ADDITION:
    case EXPRESSION_SUBTRACTION:
    case EXPRESSION_MULTIPLICATION:
    case EXPRESSION_DIVISION:
    case EXPRESSION_MODULO:
        if (typeExpression(expression->left) == DATA_TYPE_UINT && typeExpression(expression->right) == DATA_TYPE_UINT) {
            return DATA_TYPE_UINT;
        }
        else if (typeExpression(expression->left) == DATA_TYPE_INT && typeExpression(expression->right) == DATA_TYPE_INT) {
            return DATA_TYPE_INT;
        }
        else {
            return -1;
        }
    case EXPRESSION_EQUALITY:
    case EXPRESSION_INEQUALITY:
        int left_type = typeExpression(expression->left);
        int right_type = typeExpression(expression->right);
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
        int left_type = typeExpression(expression->left);
        int right_type = typeExpression(expression->right);
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
        if (typeExpression(expression->left) == DATA_TYPE_BOOLEAN && typeExpression(expression->right) == DATA_TYPE_BOOLEAN) {
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
        typeVariable(assignable->identifier);
    case ASSIGNABLE_ARRAY:
        typeExpression(assignable->arrayIndex);
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

