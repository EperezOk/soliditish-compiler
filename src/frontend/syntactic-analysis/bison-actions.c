#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../backend/domain-specific/calculator.h"
#include "../../backend/semantic-analysis/symbol-table.h"
#include "../../backend/support/logger.h"
#include "bison-actions.h"

#define MAX_ERROR_LENGTH 150

/**
 * Implementación de "bison-actions.h".
 */

char ERR_MSG[MAX_ERROR_LENGTH];

void addError(int errorLength) {
	if (state.errorCount == MAX_ERRORS) return;
	state.errors[state.errorCount] = malloc(sizeof(char) * (errorLength + 13));
	sprintf(
		state.errors[state.errorCount],
		"Line %d: %s.", yylineno, ERR_MSG
	);
	state.errorCount++;
}

/**
* Esta función se ejecuta cada vez que se emite un error de sintaxis.
*/
void yyerror(const char * string) {
	LogErrorRaw("[ERROR] Message: '%s', due to '", string);
	for (int i = 0; i < yyleng; ++i) {
		switch (yytext[i]) {
			case '\n':
				LogErrorRaw("\\n");
			default:
				LogErrorRaw("%c", yytext[i]);
		}
	}
	LogErrorRaw("' (length = %d, line %d).\n\n", yyleng, yylineno);
}

/**
* Esta acción se corresponde con el no-terminal que representa el símbolo
* inicial de la gramática, y por ende, es el último en ser ejecutado, lo que
* indica que efectivamente el programa de entrada se pudo generar con esta
* gramática, o lo que es lo mismo, que el programa pertenece al lenguaje.
*/
Program *ProgramGrammarAction(struct ContractDefinition *contract) {
	Program *program = calloc(1, sizeof(Program));
	program->contract = contract;
	state.program = program;
	/*
	* "state" es una variable global que almacena el estado del compilador,
	* cuyo campo "succeed" indica si la compilación fue o no exitosa, la cual
	* es utilizada en la función "main".
	*/
	state.succeed = state.errorCount == 0;
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
	contractInstructions->type = instructions == NULL ? CONTRACT_INSTRUCTIONS_EMPTY : CONTRACT_INSTRUCTIONS_MULTIPLE;
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
	if (symbolExists(eventIdentifier))
		addError(sprintf(ERR_MSG, "`%s` already exists", eventIdentifier));
	else
		insertSymbol(eventIdentifier);

	ContractInstruction *contractInstruction = calloc(1, sizeof(ContractInstruction));
	contractInstruction->type = EVENT_DECLARATION;
	contractInstruction->eventIdentifier = eventIdentifier;
	contractInstruction->eventParams = eventParams;
	return contractInstruction;
}

FunctionInstructions *FunctionInstructionsGrammarAction(FunctionInstructions *instructions, FunctionInstruction *instruction) {
	FunctionInstructions *functionInstructions = calloc(1, sizeof(FunctionInstructions));
	functionInstructions->type = instructions == NULL ? FUNCTION_INSTRUCTIONS_EMPTY : FUNCTION_INSTRUCTIONS_MULTIPLE;
	functionInstructions->instructions = instructions;
	functionInstructions->instruction = instruction;
	return functionInstructions;
}

FunctionInstruction *VariableDefinitionFunctionInstructionGrammarAction(VariableDefinition *variableDefinition) {
	FunctionInstruction *functionInstruction = calloc(1, sizeof(FunctionInstruction));
	functionInstruction->type = FUNCTION_INSTRUCTION_VARIABLE_DEFINITION;
	functionInstruction->variableDefinition = variableDefinition;
	return functionInstruction;	
}

FunctionInstruction *ConditionalFunctionInstructionGrammarAction(Conditional *conditional) {
	FunctionInstruction *functionInstruction = calloc(1, sizeof(FunctionInstruction));
	functionInstruction->type = FUNCTION_INSTRUCTION_CONDITIONAL;
	functionInstruction->conditional = conditional;
	return functionInstruction;	
}

FunctionInstruction *FunctionCallFunctionInstructionGrammarAction(FunctionCall *functionCall) {
	FunctionInstruction *functionInstruction = calloc(1, sizeof(FunctionInstruction));
	functionInstruction->type = FUNCTION_INSTRUCTION_FUNCTION_CALL;
	functionInstruction->functionCall = functionCall;
	return functionInstruction;	
}

FunctionInstruction *MemberCallFunctionInstructionGrammarAction(MemberCall *memberCall) {
	FunctionInstruction *functionInstruction = calloc(1, sizeof(FunctionInstruction));
	functionInstruction->type = FUNCTION_INSTRUCTION_MEMBER_CALL;
	functionInstruction->memberCall = memberCall;
	return functionInstruction;	
}

FunctionInstruction *EmitEventFunctionInstructionGrammarAction(char *eventIdentifier, Arguments *eventArgs) {
	if (!symbolExists(eventIdentifier))
		addError(sprintf(ERR_MSG, "Event `%s` does not exist", eventIdentifier));

	FunctionInstruction *functionInstruction = calloc(1, sizeof(FunctionInstruction));
	functionInstruction->type = FUNCTION_INSTRUCTION_EMIT_EVENT;
	functionInstruction->eventIdentifier = eventIdentifier;
	functionInstruction->eventArgs = eventArgs;
	return functionInstruction;	
}

FunctionInstruction *AssignmentFunctionInstructionGrammarAction(Assignment *assignment) {
	FunctionInstruction *functionInstruction = calloc(1, sizeof(FunctionInstruction));
	functionInstruction->type = FUNCTION_INSTRUCTION_ASSIGNMENT;
	functionInstruction->assignment = assignment;
	return functionInstruction;	
}

FunctionInstruction *MathAssignmentFunctionInstructionGrammarAction(MathAssignment *mathAssignment) {
	FunctionInstruction *functionInstruction = calloc(1, sizeof(FunctionInstruction));
	functionInstruction->type = FUNCTION_INSTRUCTION_MATH_ASSIGNMENT;
	functionInstruction->mathAssignment = mathAssignment;
	return functionInstruction;	
}

FunctionInstruction *LoopFunctionInstructionGrammarAction(Loop *loop) {
	FunctionInstruction *functionInstruction = calloc(1, sizeof(FunctionInstruction));
	functionInstruction->type = FUNCTION_INSTRUCTION_LOOP;
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

LoopInitialization *LoopInitVarDefGrammarAction(VariableDefinition *variable) {
	LoopInitialization *loopInitialization = calloc(1, sizeof(LoopInitialization));
	loopInitialization->type = LOOP_INITIALIZATION_VARIABLE_DEFINITION;
	loopInitialization->variable = variable;
	return loopInitialization;
}

LoopInitialization *LoopInitAssignmentGrammarAction(Assignment *assignment) {
	LoopInitialization *loopInitialization = calloc(1, sizeof(LoopInitialization));
	loopInitialization->type = LOOP_INITIALIZATION_ASSIGNMENT;
	loopInitialization->assignment = assignment;
	return loopInitialization;
}

LoopInitialization *LoopInitMathAssignmentGrammarAction(MathAssignment *assignment) {
	LoopInitialization *loopInitialization = calloc(1, sizeof(LoopInitialization));
	loopInitialization->type = LOOP_INITIALIZATION_MATH_ASSIGNMENT;
	loopInitialization->mathAssignment = assignment;
	return loopInitialization;
}

LoopCondition *LoopConditionGrammarAction(Expression *condition) {
	LoopCondition *loopCondition = calloc(1, sizeof(LoopCondition));
	loopCondition->type = condition == NULL ? LOOP_CONDITION_EMPTY : LOOP_CONDITION_CONDITIONAL;
	loopCondition->condition = condition;
	return loopCondition;
}

LoopIteration *LoopIterationAssignmentGrammarAction(Assignment *assignment) {
	LoopIteration *loopIteration = calloc(1, sizeof(LoopIteration));
	loopIteration->type = assignment == NULL ? LOOP_ITERATION_EMPTY : LOOP_ITERATION_ASSIGNMENT;
	loopIteration->assignment = assignment;
	return loopIteration;
}

LoopIteration *LoopIterationMathAssignmentGrammarAction(MathAssignment *mathAssignment) {
	LoopIteration *loopIteration = calloc(1, sizeof(LoopIteration));
	loopIteration->type = LOOP_ITERATION_MATH_ASSIGNMENT;
	loopIteration->mathAssignment = mathAssignment;
	return loopIteration;
}

MathAssignment *IncDecGrammarAction(Assignable *variable, MathAssignmentType type) {
	MathAssignment *mathAssignment = calloc(1, sizeof(MathAssignment));
	mathAssignment->type = type;
	mathAssignment->variable = variable;
	return mathAssignment;
}

Assignable *AssignableGrammarAction(char *identifier, Expression *arrayIndex) {
	if (!symbolExists(identifier))
		addError(sprintf(ERR_MSG, "Variable `%s` does not exist", identifier));

	Assignable *assignable = calloc(1, sizeof(Assignable));
	assignable->type = arrayIndex == NULL ? ASSIGNABLE_VARIABLE : ASSIGNABLE_ARRAY;
	assignable->identifier = identifier;
	assignable->arrayIndex = arrayIndex;
	return assignable;
}

Assignment *AssignmentExpressionGrammarAction(Assignable *assignable, Expression *expression) {
	Assignment *assignment = calloc(1, sizeof(Assignment));
	assignment->type = ASSIGNMENT_EXPRESSION;
	assignment->expression = expression;
	return assignment;
}

Assignment *AssignmentArrayInitGrammarAction(Assignable *assignable, Arguments *arrayElements) {
	Assignment *assignment = calloc(1, sizeof(Assignment));
	assignment->type = ASSIGNMENT_ARRAY_INITIALIZATION;
	assignment->arrayElements = arrayElements;
	return assignment;
}

Assignment *AssignmentFunctionCallGrammarAction(Assignable *assignable, FunctionCall *functionCall) {
	Assignment *assignment = calloc(1, sizeof(Assignment));
	assignment->type = ASSIGNMENT_FUNCTION_CALL;
	assignment->functionCall = functionCall;
	return assignment;
}

MathAssignment *MathAssignmentGrammarAction(Assignable *variable, MathAssignmentOperator *operator, Expression *expression) {
	MathAssignment *mathAssignment = calloc(1, sizeof(MathAssignment));
	mathAssignment->type = MATH_ASSIGNMENT_OPERATOR;
	mathAssignment->variable = variable;
	mathAssignment->operator = operator;
	mathAssignment->expression = expression;
	return mathAssignment;
}

MathAssignmentOperator *MathAssignmentOperatorGrammarAction(MathAssignmentOperatorType type) {
	MathAssignmentOperator *mathAssignmentOperator = calloc(1, sizeof(MathAssignmentOperator));
	mathAssignmentOperator->type = type;
	return mathAssignmentOperator;
}

FunctionCall *FunctionCallGrammarAction(char *identifier, Arguments *arguments) {
	if (!symbolExists(identifier))
		addError(sprintf(ERR_MSG, "Function `%s` does not exist", identifier));

	FunctionCall *functionCall = calloc(1, sizeof(FunctionCall));
	functionCall->type = arguments == NULL ? FUNCTION_CALL_NO_ARGS : FUNCTION_CALL_WITH_ARGS;
	functionCall->identifier = identifier;
	functionCall->arguments = arguments;
	return functionCall;
}

Arguments *ArgumentsGrammarAction(Arguments *arguments, Expression *expression) {
	Arguments *args = calloc(1, sizeof(Arguments));
	args->type = arguments == NULL ? ARGUMENTS_SINGLE : ARGUMENTS_MULTIPLE;
	args->arguments = arguments;
	args->expression = expression;
	return args;
}

MemberCall *MemberCallGrammarAction(Assignable *instance, FunctionCall *method) {
	MemberCall *memberCall = calloc(1, sizeof(MemberCall));
	memberCall->instance = instance;
	memberCall->method = method;
	return memberCall;
}

VariableDefinition *VariableDefExpressionGrammarAction(DataType *dataType, char *identifier, Expression *expression) {
	if (symbolExists(identifier))
		addError(sprintf(ERR_MSG, "`%s` already exists", identifier));
	else
		insertSymbol(identifier);

	VariableDefinition *variableDefinition = calloc(1, sizeof(VariableDefinition));
	variableDefinition->type = VARIABLE_DEFINITION_INITIALIZATION;
	variableDefinition->dataType = dataType;
	variableDefinition->expression = expression;
	return variableDefinition;
}

VariableDefinition *VariableDefFunctionCallGrammarAction(DataType *dataType, char *identifier, FunctionCall *functionCall) {
	if (symbolExists(identifier))
		addError(sprintf(ERR_MSG, "`%s` already exists", identifier));
	else
		insertSymbol(identifier);

	VariableDefinition *variableDefinition = calloc(1, sizeof(VariableDefinition));
	variableDefinition->type = VARIABLE_DEFINITION_INITIALIZATION;
	variableDefinition->dataType = dataType;
	variableDefinition->functionCall = functionCall;
	return variableDefinition;
}

VariableDefinition *VariableDefinitionGrammarAction(DataType *dataType, char *identifier) {
	if (symbolExists(identifier))
		addError(sprintf(ERR_MSG, "`%s` already exists", identifier));
	else
		insertSymbol(identifier);

	VariableDefinition *variableDefinition = calloc(1, sizeof(VariableDefinition));
	variableDefinition->type = VARIABLE_DEFINITION_DECLARATION;
	variableDefinition->dataType = dataType;
	variableDefinition->identifier = identifier;
	return variableDefinition;
}

DataType *DataTypeSimpleGrammarAction(DataTypeType type) {
	DataType *dataType = calloc(1, sizeof(DataType));
	dataType->type = type;
	return dataType;
}

DataType *DataTypeArrayGrammarAction(DataType *dataType, Expression *expression) {
	DataType *arrayDataType = calloc(1, sizeof(DataType));	
	arrayDataType->type = expression == NULL ? DATA_TYPE_DYNAMIC_ARRAY : DATA_TYPE_STATIC_ARRAY;
	arrayDataType->dataType = dataType;
	arrayDataType->expression = expression;
	return arrayDataType;
}

FunctionDefinition *FunctionDefinitionGrammarAction(Decorators *dec, char *id, ParameterDefinition *pd, FunctionBlock *fb) {
	if (symbolExists(id))
		addError(sprintf(ERR_MSG, "`%s` already exists", id));
	else
		insertSymbol(id);

	FunctionDefinition *functionDefinition = calloc(1, sizeof(FunctionDefinition));
	functionDefinition->decorators = dec;
	functionDefinition->parameterDefinition = pd;
	functionDefinition->identifier = id;
	functionDefinition->functionBlock = fb;
	return functionDefinition;
}

Decorators *DecoratorsGrammarAction(char *decorator, Decorators *decorators) {
	Decorators *dec = calloc(1, sizeof(Decorators));
	dec->type = decorators == NULL ? DECORATORS_NO_DECORATORS : DECORATORS_WITH_DECORATORS;
	dec->decorators = decorators;
	dec->decorator = decorator;
	return dec;
}

ParameterDefinition *ParameterDefinitionGrammarAction(Parameters *parameters) {
	ParameterDefinition *params = calloc(1, sizeof(ParameterDefinition));
	params->type = parameters == NULL ? PARAMETERS_DEFINITION_NO_PARAMS : PARAMETERS_DEFINITION_WITH_PARAMS;
	params->parameters = parameters;
	return params;
}

Parameters *ParametersGrammarAction(Parameters *parameters, DataType *dataType, char *identifier) {
	Parameters *params = calloc(1, sizeof(Parameters));
	params->type = parameters == NULL ? PARAMETERS_SINGLE : PARAMETERS_MULTIPLE;
	params->parameters = parameters;
	params->dataType = dataType;
	params->identifier = identifier;
	return params;
}

Expression *ExpressionGrammarAction(ExpressionType type, Expression *leftExpression, Expression *rightExpression) {
	Expression *expression = calloc(1, sizeof(Expression));
	expression->type = type;
	expression->leftExpression = leftExpression;
	expression->rightExpression = rightExpression;
	return expression;
}

Expression *FactorExpressionGrammarAction(Factor *factor) {
	Expression *expression = calloc(1, sizeof(Expression));
	expression->type = EXPRESSION_FACTOR;
	expression->factor = factor;
	return expression;
}

Factor *ExpressionFactorGrammarAction(Expression *expression) {
	Factor *factor = calloc(1, sizeof(Factor));
	factor->type = FACTOR_EXPRESSION;
	factor->expression = expression;
	return factor;
}

Factor *ConstantFactorGrammarAction(Constant *constant) {
	Factor *factor = calloc(1, sizeof(Factor));
	factor->type = FACTOR_CONSTANT;
	factor->constant = constant;
	return factor;
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

Constant *AssignableConstantGrammarAction(struct Assignable *variable) {
	Constant *constant = calloc(1, sizeof(Constant));
	constant->type = CONSTANT_VARIABLE;
	constant->variable = variable;
	return constant;
}
