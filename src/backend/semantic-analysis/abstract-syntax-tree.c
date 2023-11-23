#include <stdlib.h>
#include "abstract-syntax-tree.h"

static void freeContractDefinition(ContractDefinition *contractDefinition);
static void freeContractBlock(ContractBlock *contractBlock);
static void freeContractInstructions(ContractInstructions *contractInstructions);
static void freeContractInstruction(ContractInstruction *contractInstruction);
static void freeDecorators(Decorators *decorators);
static void freeFunctionDefinition(FunctionDefinition *functionDefinition);
static void freeParameterDefinition(ParameterDefinition *parameterDefinition);
static void freeParameters(Parameters *parameters);
static void freeFunctionBlock(FunctionBlock *functionBlock);
static void freeFunctionInstructions(FunctionInstructions *functionInstructions);
static void freeFunctionInstruction(FunctionInstruction *functionInstruction);
static void freeFunctionCall(FunctionCall *functionCall);
static void freeArguments(Arguments *arguments);
static void freeMemberCall(MemberCall *memberCall);
static void freeVariableDefinition(VariableDefinition *variableDefinition);
static void freeConditional(Conditional *conditional);
static void freeLoop(Loop *loop);
static void freeLoopInitialization(LoopInitialization *loopInitialization);
static void freeLoopCondition(LoopCondition *loopCondition);
static void freeLoopIteration(LoopIteration *loopIteration);
static void freeAssignable(Assignable *assignable);
static void freeAssignment(Assignment *assignment);
static void freeMathAssignment(MathAssignment *mathAssignment);
static void freeMathAssignmentOperator(MathAssignmentOperator *mathAssignmentOperator);
static void freeDataType(DataType *dataType);
static void freeExpression(Expression *expression);
static void freeFactor(Factor *factor);
static void freeConstant(Constant *constant);
static void freeIdentifier(char *identifier);

void freeAST(Program *program) {
    if (program == NULL) return;
    freeContractDefinition(program->contract);
    free(program);
}

static void freeContractDefinition(ContractDefinition *contractDefinition) {
    if (contractDefinition == NULL) return;
    freeIdentifier(contractDefinition->identifier);
    freeContractBlock(contractDefinition->block);
    free(contractDefinition);
}

static void freeContractBlock(ContractBlock *contractBlock) {
    if (contractBlock == NULL) return;
    freeContractInstructions(contractBlock->instructions);
    free(contractBlock);
}

static void freeContractInstructions(ContractInstructions *contractInstructions) {
    if (contractInstructions == NULL) return;
    freeContractInstructions(contractInstructions->instructions);
    freeContractInstruction(contractInstructions->instruction);
    free(contractInstructions);
}

static void freeContractInstruction(ContractInstruction *contractInstruction) {
    if (contractInstruction == NULL) return;
    freeFunctionDefinition(contractInstruction->functionDefinition);
    freeDecorators(contractInstruction->variableDecorators);
    freeVariableDefinition(contractInstruction->variableDefinition);
    freeIdentifier(contractInstruction->eventIdentifier);
    freeParameterDefinition(contractInstruction->eventParams);
    free(contractInstruction);
}

static void freeDecorators(Decorators *decorators) {
    if (decorators == NULL) return;
    freeDecorators(decorators->decorators);
    freeIdentifier(decorators->decorator);
    free(decorators);
}

static void freeFunctionDefinition(FunctionDefinition *functionDefinition) {
    if (functionDefinition == NULL) return;
    freeParameterDefinition(functionDefinition->parameterDefinition);
    freeFunctionBlock(functionDefinition->functionBlock);
    freeIdentifier(functionDefinition->identifier);
    free(functionDefinition);
}

static void freeParameterDefinition(ParameterDefinition *parameterDefinition) {
    if (parameterDefinition == NULL) return;
    freeParameters(parameterDefinition->parameters);
    free(parameterDefinition);
}

static void freeParameters(Parameters *parameters) {
    if (parameters == NULL) return;
    freeParameters(parameters->parameters);
    freeDataType(parameters->dataType);
    freeIdentifier(parameters->identifier);
    free(parameters);
}

static void freeFunctionBlock(FunctionBlock *functionBlock) {
    if (functionBlock == NULL) return;
    freeFunctionInstructions(functionBlock->instructions);
    free(functionBlock);
}

static void freeFunctionInstructions(FunctionInstructions *functionInstructions) {
    if (functionInstructions == NULL) return;
    freeFunctionInstructions(functionInstructions->instructions);
    freeFunctionInstruction(functionInstructions->instruction);
    free(functionInstructions);
}

static void freeFunctionInstruction(FunctionInstruction *functionInstruction) {
    if (functionInstruction == NULL) return;
    freeFunctionCall(functionInstruction->functionCall);
    freeMemberCall(functionInstruction->memberCall);
    freeVariableDefinition(functionInstruction->variableDefinition);
    freeConditional(functionInstruction->conditional);
    freeLoop(functionInstruction->loop);
    freeAssignment(functionInstruction->assignment);
    freeMathAssignment(functionInstruction->mathAssignment);
    free(functionInstruction);
}

static void freeFunctionCall(FunctionCall *functionCall) {
    if (functionCall == NULL) return;
    freeArguments(functionCall->arguments);
    freeIdentifier(functionCall->identifier);
    free(functionCall);
}

static void freeArguments(Arguments *arguments) {
    if (arguments == NULL) return;
    freeArguments(arguments->arguments);
    freeExpression(arguments->expression);
    free(arguments);
}

static void freeMemberCall(MemberCall *memberCall) {
    if (memberCall == NULL) return;
    freeAssignable(memberCall->instance);
    freeFunctionCall(memberCall->method);
    free(memberCall);
}

static void freeVariableDefinition(VariableDefinition *variableDefinition) {
    if (variableDefinition == NULL) return;
    freeDataType(variableDefinition->dataType);
    freeExpression(variableDefinition->expression);
    freeFunctionCall(variableDefinition->functionCall);
    freeIdentifier(variableDefinition->identifier);
    free(variableDefinition);
}

static void freeConditional(Conditional *conditional) {
    if (conditional == NULL) return;
    freeExpression(conditional->condition);
    freeFunctionBlock(conditional->ifBlock);
    freeFunctionBlock(conditional->elseBlock);
    free(conditional);
}

static void freeLoop(Loop *loop) {
    if (loop == NULL) return;
    freeLoopInitialization(loop->loopInitialization);
    freeLoopCondition(loop->loopCondition);
    freeLoopIteration(loop->loopIteration);
    freeFunctionBlock(loop->functionBlock);
    free(loop);
}

static void freeLoopInitialization(LoopInitialization *loopInitialization) {
    if (loopInitialization == NULL) return;
    freeVariableDefinition(loopInitialization->variable);
    freeAssignment(loopInitialization->assignment);
    freeMathAssignment(loopInitialization->mathAssignment);
    free(loopInitialization);
}

static void freeLoopCondition(LoopCondition *loopCondition) {
    if (loopCondition == NULL) return;
    freeExpression(loopCondition->condition);
    free(loopCondition);
}

static void freeLoopIteration(LoopIteration *loopIteration) {
    if (loopIteration == NULL) return;
    freeAssignment(loopIteration->assignment);
    freeMathAssignment(loopIteration->mathAssignment);
    free(loopIteration);
}

static void freeAssignable(Assignable *assignable) {
    freeExpression(assignable->arrayIndex);
    freeIdentifier(assignable->identifier);
    free(assignable);
}

static void freeAssignment(Assignment *assignment) {
    if (assignment == NULL) return;
    freeAssignable(assignment->assignable);
    freeExpression(assignment->expression);
    freeFunctionCall(assignment->functionCall);
    free(assignment);
}

static void freeMathAssignment(MathAssignment *mathAssignment) {
    if (mathAssignment == NULL) return;
    freeAssignable(mathAssignment->variable);
    freeExpression(mathAssignment->expression);
    freeMathAssignmentOperator(mathAssignment->operator);
    free(mathAssignment);
}

static void freeMathAssignmentOperator(MathAssignmentOperator *mathAssignmentOperator) {
    if (mathAssignmentOperator == NULL) return;
    free(mathAssignmentOperator);
}

static void freeDataType(DataType *dataType) {
    if (dataType == NULL) return;
    freeDataType(dataType->dataType);
    freeExpression(dataType->expression);
    free(dataType);
}

static void freeExpression(Expression *expression) {
    if (expression == NULL) return;
    freeExpression(expression->left);
    freeExpression(expression->right);
    freeFactor(expression->factor);
    free(expression);
}

static void freeFactor(Factor *factor) {
    if (factor == NULL) return;
    freeExpression(factor->expression);
    freeConstant(factor->constant);
    free(factor);
}

static void freeConstant(Constant *constant) {
    if (constant == NULL) return;
    freeAssignable(constant->variable);
    freeIdentifier(constant->string);
    free(constant);
}

static void freeIdentifier(char *identifier) {
    if (identifier == NULL) return;
    free(identifier);
}
