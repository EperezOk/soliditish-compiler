#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../backend/domain-specific/calculator.h"
#include "../../backend/support/logger.h"
#include "bison-actions.h"

/**
 * Implementación de "bison-actions.h".
 */

/**
* Esta función se ejecuta cada vez que se emite un error de sintaxis.
*/
void yyerror(const char * string) {
	LogErrorRaw("[ERROR] Mensaje: '%s', debido a '", string);
	for (int i = 0; i < yyleng; ++i) {
		switch (yytext[i]) {
			case '\n':
				LogErrorRaw("\\n");
			default:
				LogErrorRaw("%c", yytext[i]);
		}
	}
	LogErrorRaw("' (length = %d, linea %d).\n\n", yyleng, yylineno);
}

/**
* Esta acción se corresponde con el no-terminal que representa el símbolo
* inicial de la gramática, y por ende, es el último en ser ejecutado, lo que
* indica que efectivamente el programa de entrada se pudo generar con esta
* gramática, o lo que es lo mismo, que el programa pertenece al lenguaje.
*/
Program *ProgramGrammarAction(ContractDefinition *contract) {
	Program *program = calloc(1, sizeof(Program));
	program->contract = contract;
	state.program = program;
	/*
	* "state" es una variable global que almacena el estado del compilador,
	* cuyo campo "succeed" indica si la compilación fue o no exitosa, la cual
	* es utilizada en la función "main".
	*/
	state.succeed = true;
	return program;
}

ContractDefinition *ContractDefinitionGrammarAction(char *identifier, ContractBlock *block) {
	ContractDefinition *contractDefinition = calloc(1, sizeof(ContractDefinition));
	contractDefinition->identifier = identifier;
	contractDefinition->block = block;
	return contractDefinition;
}

ContractBlock *ContractBlockGrammarAction(ContractInstructions *instructions) {
	ContractBlock *contractBlock = calloc(1, sizeof(ContractBlock));
	contractBlock->instructions = instructions;
	return contractBlock;
}

FunctionBlock *FunctionBlockGrammarAction(FunctionInstructions *instructions) {
	FunctionBlock *functionBlock = calloc(1, sizeof(FunctionBlock));
	functionBlock->instructions = instructions;
	return functionBlock;
}

Conditional *ConditionalGrammarAction(Expression *condition, FunctionBlock *ifBlock, FunctionBlock *elseBlock) {
	Conditional *conditional = calloc(1, sizeof(Conditional));
	conditional->type = elseBlock == NULL ? NO_ELSE : WITH_ELSE;
	conditional->condition = condition;
	conditional->ifBlock = ifBlock;
	conditional->elseBlock = elseBlock;
	return conditional;
}

ContractInstructions *ContractInstructionsGrammarAction(ContractInstructions *instructions, ContractInstruction *instruction) {
	ContractInstructions *contractInstructions = calloc(1, sizeof(ContractInstructions));
	contractInstructions->type = instructions == NULL ? EMPTY : MULTIPLE;
	contractInstructions->instructions = instructions;
	contractInstructions->instruction = instruction;
	return contractInstructions;
}

ContractInstruction *StateVariableContractInstructionGrammarAction(Decorators *variableDecorators, VariableDefinition *variableDefinition) {
	ContractInstruction *contractInstruction = calloc(1, sizeof(ContractInstruction));
	contractInstruction->type = STATE_VARIABLE_DECLARATION;
	contractInstruction->variableDecorators = variableDecorators;
	contractInstruction->variableDefinition = variableDefinition;
	return contractInstruction;
}

ContractInstruction *FunctionDefinitionContractInstructionGrammarAction(FunctionDefinition *functionDefinition) {
	ContractInstruction *contractInstruction = calloc(1, sizeof(ContractInstruction));
	contractInstruction->type = FUNCTION_DECLARATION;
	contractInstruction->functionDefinition = functionDefinition;
	return contractInstruction;
}

ContractInstruction *EventDefinitionContractInstructionGrammarAction(char *eventIdentifier, ParameterDefinition *eventParams) {
	ContractInstruction *contractInstruction = calloc(1, sizeof(ContractInstruction));
	contractInstruction->type = EVENT_DECLARATION;
	contractInstruction->eventIdentifier = eventIdentifier;
	contractInstruction->eventParams = eventParams;
	return contractInstruction;
}

FunctionInstructions *FunctionInstructionsGrammarAction(FunctionInstructions *instructions, FunctionInstruction *instruction) {
	FunctionInstructions *functionInstructions = calloc(1, sizeof(FunctionInstructions));
	functionInstructions->type = instructions == NULL ? EMPTY : MULTIPLE;
	functionInstructions->instructions = instructions;
	functionInstructions->instruction = instruction;
	return functionInstructions;
}

FunctionInstruction *VariableDefinitionFunctionInstructionGrammarAction(VariableDefinition *variableDefinition) {
	FunctionInstruction *functionInstruction = calloc(1, sizeof(FunctionInstruction));
	functionInstruction->type = VARIABLE_DEFINITION;
	functionInstruction->variableDefinition = variableDefinition;
	return functionInstruction;	
}

FunctionInstruction *ConditionalFunctionInstructionGrammarAction(Conditional *conditional) {
	FunctionInstruction *functionInstruction = calloc(1, sizeof(FunctionInstruction));
	functionInstruction->type = CONDITIONAL;
	functionInstruction->conditional = conditional;
	return functionInstruction;	
}

FunctionInstruction *FunctionCallFunctionInstructionGrammarAction(FunctionCall *functionCall) {
	FunctionInstruction *functionInstruction = calloc(1, sizeof(FunctionInstruction));
	functionInstruction->type = FUNCTION_CALL;
	functionInstruction->functionCall = functionCall;
	return functionInstruction;	
}

FunctionInstruction *MemberCallFunctionInstructionGrammarAction(MemberCall *memberCall) {
	FunctionInstruction *functionInstruction = calloc(1, sizeof(FunctionInstruction));
	functionInstruction->type = MEMBER_CALL;
	functionInstruction->memberCall = memberCall;
	return functionInstruction;	
}

FunctionInstruction *EmitEventFunctionInstructionGrammarAction(char *eventIdentifier, Arguments *eventArgs) {
	FunctionInstruction *functionInstruction = calloc(1, sizeof(FunctionInstruction));
	functionInstruction->type = EMIT_EVENT;
	functionInstruction->eventIdentifier = eventIdentifier;
	functionInstruction->eventArgs = eventArgs;
	return functionInstruction;	
}

FunctionInstruction *AssignmentFunctionInstructionGrammarAction(Assignment *assignment) {
	FunctionInstruction *functionInstruction = calloc(1, sizeof(FunctionInstruction));
	functionInstruction->type = ASSIGNMENT;
	functionInstruction->assignment = assignment;
	return functionInstruction;	
}

FunctionInstruction *MathAssignmentFunctionInstructionGrammarAction(MathAssignment *mathAssignment) {
	FunctionInstruction *functionInstruction = calloc(1, sizeof(FunctionInstruction));
	functionInstruction->type = MATH_ASSIGNMENT;
	functionInstruction->mathAssignment = mathAssignment;
	return functionInstruction;	
}

FunctionInstruction *LoopFunctionInstructionGrammarAction(Loop *loop) {
	FunctionInstruction *functionInstruction = calloc(1, sizeof(FunctionInstruction));
	functionInstruction->type = LOOP;
	functionInstruction->loop = loop;
	return functionInstruction;	
}

Loop *LoopGrammarAction(LoopInitialization *loopInitialization, LoopCondition *loopCondition, LoopIteration *loopIteration, FunctionBlock *functionBlock) {
	Loop *loop = calloc(1, sizeof(Loop));
	loop->loopInitialization = loopInitialization;
	loop->loopCondition = loopCondition;
	loop->loopIteration = loopIteration;
	loop->functionBlock = functionBlock;
	return loop;	
}


int InstructionsGrammarAction(int instructions, int instruction) {
	LogDebug("[Bison] InstructionsGrammarAction(%d, %d)", instructions, instruction);
	return 0;
}

DataType *DataTypeSimpleGrammarAction(DataTypeType type) {
	DataType *dataType = calloc(1, sizeof(DataType));
	dataType->type = type;
	return dataType;
}

DataType *DataTypeArrayGrammarAction(DataType *dataType, Expression *expression) {
	DataType *arrayDataType = calloc(1, sizeof(DataType));	
	arrayDataType->type = expression == NULL ? DYNAMIC_ARRAY : STATIC_ARRAY;
	arrayDataType->dataType = dataType;
	return arrayDataType;
}

int EmptyInstructionGrammarAction() {
	LogDebug("[Bison] EmptyInstructionsGrammarAction");
	return 0;
}

int VariableDefinitionGrammarAction(int dataType, char *name, char *value) {
	LogDebug("[Bison] VariableDefinitionGrammarAction(%d, %s, %s)", dataType, name, value);
	return 0;
}

int FunctionInstructionGrammarAction() {
	LogDebug("[Bison] FunctionInstructionGrammarAction");
	return 0;
}

int FunctionDefinitionGrammarAction(char *functionName) {
	LogDebug("[Bison] FunctionDefinitionGrammarAction(%s)", functionName);
	return 0;
}

int EmptyArgumentListGrammarAction() {
	LogDebug("[Bison] EmptyArgumentListGrammarAction");
	return 0;
}

int ArgumentListGrammarAction() {
	LogDebug("[Bison] ArgumentListGrammarAction");
	return 0;
}

int ArgumentDefinitionGrammarAction() {
	LogDebug("[Bison] ArgumentDefinitionGrammarAction");
	return 0;
}

Expression *AdditionExpressionGrammarAction(Expression *leftExpression, Expression *rightExpression) {
	Expression *expression = calloc(1, sizeof(Expression));
	expression->type = ADDITION;
	expression->leftExpression = leftExpression;
	expression->rightExpression = rightExpression;
	return expression;
}

int SubtractionExpressionGrammarAction(int leftValue, int rightValue) {
	LogDebug("[Bison] SubtractionExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return Subtract(leftValue, rightValue);
}

int MultiplicationExpressionGrammarAction(int leftValue, int rightValue) {
	LogDebug("[Bison] MultiplicationExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return Multiply(leftValue, rightValue);
}

int DivisionExpressionGrammarAction(int leftValue, int rightValue) {
	LogDebug("[Bison] DivisionExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return Divide(leftValue, rightValue);
}

int ModuloExpressionGrammarAction(int leftValue, int rightValue) {
	LogDebug("[Bison] ModuloExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return 0;
}

int ExponentiationExpressionGrammarAction(int leftValue, int rightValue) {
	LogDebug("[Bison] ExponentiationExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return 0;
}

int EqualityExpressionGrammarAction(int leftValue, int rightValue) {
	LogDebug("[Bison] EqualityExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return 0;
}
int InequalityExpressionGrammarAction(int leftValue, int rightValue) {
	LogDebug("[Bison] InequalityExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return 0;
}

int LessThanExpressionGrammarAction(int leftValue, int rightValue) {
	LogDebug("[Bison] LessThanExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return 0;
}

int LessThanOrEqualExpressionGrammarAction(int leftValue, int rightValue) {
	LogDebug("[Bison] LessThanOrEqualExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return 0;
}

int GreaterThanExpressionGrammarAction(int leftValue, int rightValue) {
	LogDebug("[Bison] GreaterThanExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return 0;
}

int GreaterThanOrEqualExpressionGrammarAction(int leftValue, int rightValue) {
	LogDebug("[Bison] GreaterThanOrEqualExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return 0;
}

int AndExpressionGrammarAction(int leftValue, int rightValue) {
	LogDebug("[Bison] AndExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return 0;
}

int OrExpressionGrammarAction(int leftValue, int rightValue) {
	LogDebug("[Bison] OrExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return 0;
}

int NotExpressionGrammarAction(int value) {
	LogDebug("[Bison] NotExpressionGrammarAction(%d)", value);
	return 0;
}

int FactorExpressionGrammarAction(int value) {
	LogDebug("[Bison] FactorExpressionGrammarAction(%d)", value);
	return value;
}

int ExpressionFactorGrammarAction(int value) {
	LogDebug("[Bison] ExpressionFactorGrammarAction(%d)", value);
	return value;
}

int ConstantFactorGrammarAction(int value) {
	LogDebug("[Bison] ConstantFactorGrammarAction(%d)", value);
	return value;
}

Constant *StringConstantGrammarAction(ConstantType type, char *str) {
	Constant *constant = calloc(1, sizeof(Constant));
	constant->type = type;
	constant->string = str;
	return constant;
}

Constant *IntegerConstantGrammarAction(ConstantType type, int value) {
	Constant *constant = calloc(1, sizeof(Constant));
	constant->type = type;
	constant->value = value;
	return constant;
}

Constant *AssignableConstantGrammarAction(Assignable *variable) {
	Constant *constant = calloc(1, sizeof(Constant));
	constant->type = VARIABLE;
	constant->variable = variable;
	return constant;
}
