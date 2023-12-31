#include <stdarg.h>
#include "../support/logger.h"
#include "../support/shared.h"
#include "generator.h"

/**
 * Implementación de "generator.h".
 */

extern FILE *outputFile;
extern int indentationSize;
extern boolean indentUsingSpaces;
extern boolean indentOutput;

int indentationLevel = 0;
boolean indentNextOutput = false;

static void includeDependencies(Program *program);

static void generateProgram(Program *program);
static void generateContractInstructions(ContractInstructions *instructions);
static void generateContractInstruction(ContractInstruction *instruction);
static void generateVariableDefinition(Decorators *decorators, VariableDefinition *definition);
static void generateFunctionCall(FunctionCall *functionCall);
static void generateFunctionDefinition(FunctionDefinition *definition);
static void generateFunctionBlock(FunctionBlock *block);
static void generateFunctionInstructions(FunctionInstructions *instructions);
static void generateFunctionInstruction(FunctionInstruction *instruction);
static void generateParameterDefinition(ParameterDefinition *definition, boolean addMemoryLoc);
static void generateParameters(Parameters *params, boolean addMemoryLoc);
static void generateDataType(DataType *dataType);
static void generateExpression(Expression *expression);
static void generateFactor(Factor *factor);
static void generateConstant(Constant *constant);
static void generateAssignable(Assignable *assignable);
static void generateArguments(Arguments *arguments);
static void generateConditional(Conditional *conditional);
static void generateMemberCall(MemberCall *memberCall);
static void generateLoop(Loop *loop);
static void generateLoopInitialization(LoopInitialization *loopInit);
static void generateLoopCondition(LoopCondition *condition);
static void generateLoopIteration(LoopIteration *loopIteration);
static void generateAssignment(Assignment *assignment);
static void generateMathAssignment(MathAssignment *mathAssignment);
static void generateMathAssignmentOperator(MathAssignmentOperator *operator);

void Generator() {
	LogInfo("Generating output...");
	includeDependencies(state.program);
	generateProgram(state.program);
}

void applyIndentation(char firstChar, char lastChar) {
	// Adjust indentation level
	if (firstChar == '}') --indentationLevel;
	else if (firstChar == '{') ++indentationLevel;

	// Add indentation
	if (indentNextOutput) {
		for (int i = 0; i < indentationLevel; ++i) {
			for (int j = 0; j < indentationSize; ++j) {
				fprintf(outputFile, "%s", indentUsingSpaces ? " " : "\t");
			}
		}
	}

	// Indent next line
	if (lastChar == '\n') indentNextOutput = true;
	else indentNextOutput = false;
}

static void output(const char *format, ...) {
	va_list args;
	va_start(args, format);

	if (indentOutput) applyIndentation(format[0], format[strlen(format) - 1]);

	vfprintf(outputFile, format, args);

	va_end(args);
}

static void includeDependencies(Program *program) {
	// License and pragma
	output("// SPDX-License-Identifier: MIT\n");
	output("pragma solidity ^0.8.0;\n\n");

	// Add imports
	output("import \"@openzeppelin/contracts/utils/ReentrancyGuard.sol\";\n"); // public functions are nonReentrant
	if (state.libraries[LIBRARY_ERC20]) output("import \"@openzeppelin/contracts/token/ERC20/IERC20.sol\";\n");
	if (state.libraries[LIBRARY_ERC721]) output("import \"@openzeppelin/contracts/token/ERC721/IERC721.sol\";\n");
	if (state.libraries[LIBRARY_CLONES]) output("import \"@openzeppelin/contracts/proxy/Clones.sol\";\n");
	if (state.libraries[LIBRARY_CONSOLE]) output("import \"forge-std/console.sol\";\n");

	output("\n");
}

static void generateProgram(Program *program) {
	output("contract %s is ReentrancyGuard ", program->contract->identifier);
	output("{");
	generateContractInstructions(program->contract->block->instructions);
	output("}\n");
}

static void generateContractInstructions(ContractInstructions *instructions) {
	if (instructions->type == CONTRACT_INSTRUCTIONS_MULTIPLE) {
		generateContractInstructions(instructions->instructions);
		output("\n");
		generateContractInstruction(instructions->instruction);
	}
	// else: CONTRACT_INSTRUCTIONS_EMPTY, do nothing
}

static void generateContractInstruction(ContractInstruction *instruction) {
	switch (instruction->type) {
		case STATE_VARIABLE_DECLARATION:
			generateVariableDefinition(instruction->variableDecorators, instruction->variableDefinition);
			output(";\n");
			break;
		case FUNCTION_DECLARATION:
			generateFunctionDefinition(instruction->functionDefinition);
			break;
		case EVENT_DECLARATION:
			output("event %s", instruction->eventIdentifier);
			generateParameterDefinition(instruction->eventParams, false);
			output(";\n");
			break;
	}
}

static void generateVariableDefinition(Decorators *decorators, VariableDefinition *definition) {
	generateDataType(definition->dataType);

	if (decorators == NULL) {
		switch (definition->dataType->type) {
			case DATA_TYPE_BYTES:
			case DATA_TYPE_STRING:
			case DATA_TYPE_ARRAY:
				output(" memory");
				break;
		}
	}

	// State variables will be internal by default, if not specified otherwise with a decorator
	while (decorators != NULL && decorators->decorator != NULL) {
		output(" %s", decorators->decorator);
		decorators = decorators->decorators;
	}

	output(" %s", definition->identifier);

	if (definition->dataType->type == DATA_TYPE_ARRAY) {
		output(" = new ");
		generateDataType(definition->dataType);
		output("(");
		generateExpression(definition->dataType->expression); // array size
		output(")");
	}

	if (definition->type != VARIABLE_DEFINITION_DECLARATION) {
		output(" = ");
		if (definition->dataType->type == DATA_TYPE_ERC20) output("IERC20(");
		if (definition->dataType->type == DATA_TYPE_ERC721) output("IERC721(");
	}
	
	switch(definition->type) {
		case VARIABLE_DEFINITION_INIT_EXPRESSION:
			generateExpression(definition->expression);
			break;
		case VARIABLE_DEFINITION_INIT_FUNCTION_CALL:
			generateFunctionCall(definition->functionCall);
			break;
	}

	if (definition->type != VARIABLE_DEFINITION_DECLARATION) {
		if (definition->dataType->type == DATA_TYPE_ERC20) output(")");
		if (definition->dataType->type == DATA_TYPE_ERC721) output(")");
	}
}

static void generateFunctionCall(FunctionCall *functionCall) {
	switch (functionCall->type)	{
		case FUNCTION_CALL_NO_ARGS:
			output("%s()", functionCall->identifier);
			break;
		case FUNCTION_CALL_WITH_ARGS:
			output("%s(", functionCall->identifier);
			generateArguments(functionCall->arguments);
			output(")");
			break;
		case BUILT_IN_TRANSFER_ETH: {
			Arguments *amountArg = functionCall->arguments;
			Arguments *addressArg = amountArg->arguments;

			char *address;

			if (addressArg->expression->factor->constant->type == CONSTANT_ADDRESS)
				address = addressArg->expression->factor->constant->string;
			else
				address = addressArg->expression->factor->constant->variable->identifier;

			output("(bool s, ) = ");

			if (amountArg->expression->factor->constant->type == CONSTANT_INTEGER) {
				int amount = amountArg->expression->factor->constant->value;
				output("address(%s).call{value: %d}(\"\");\n", address, amount);
			}
			else if (amountArg->expression->factor->constant->type == CONSTANT_SCIENTIFIC_NOTATION) {
				char *amount = amountArg->expression->factor->constant->string;
				output("address(%s).call{value: %s}(\"\");\n", address, amount);
			}

			output("require(s, \"ETH transfer failed\")");
			break;
		}
		case BUILT_IN_BALANCE:
			output("address(");
			generateArguments(functionCall->arguments);
			output(").balance");
			break;
		case BUILT_IN_LOG:
			output("console.log(");
			generateArguments(functionCall->arguments);
			output(")");
			break;
		case BUILT_IN_CREATE_PROXY_TO:
			output("Clones.clone(address(");
			generateArguments(functionCall->arguments);
			output("))");
			break;
	}
}

static void generateArguments(Arguments *arguments) {
	switch (arguments->type) {
		case ARGUMENTS_MULTIPLE:
			generateArguments(arguments->arguments);
			output(", ");
			generateExpression(arguments->expression);
			break;
		case ARGUMENTS_SINGLE:
			generateExpression(arguments->expression);
			break;
		case ARGUMENTS_EMPTY:
			break;
	}
}

static void generateFunctionDefinition(FunctionDefinition *function) {
	output("function %s", function->identifier);
	generateParameterDefinition(function->parameterDefinition, true);

	boolean isPublic = false;

	Decorators *decorators = function->decorators;
	if (decorators->decorator == NULL) {
		// If no decorator is specified, the function will be internal by default
		output(" internal");
	}
	while (decorators->decorator != NULL) {
		if (strcmp(decorators->decorator, "public") == 0) isPublic = true;
		output(" %s", decorators->decorator);
		decorators = decorators->decorators;
	}

	if (isPublic) output(" nonReentrant");

	output(" ");
	generateFunctionBlock(function->functionBlock);
}

static void generateParameterDefinition(ParameterDefinition *definition, boolean addMemoryLoc) {
	output("(");

	if (definition->type == PARAMETERS_DEFINITION_WITH_PARAMS)
		generateParameters(definition->parameters, addMemoryLoc);

	output(")");
}

static void generateFunctionBlock(FunctionBlock *block) {
	output("{\n");
	generateFunctionInstructions(block->instructions);
	output("}\n");
}

static void generateFunctionInstructions(FunctionInstructions *instructions) {
	if (instructions->type == FUNCTION_INSTRUCTIONS_MULTIPLE) {
		generateFunctionInstructions(instructions->instructions);
		generateFunctionInstruction(instructions->instruction);
	}
	// else: FUNCTION_INSTRUCTIONS_EMPTY, do nothing
}

static void generateFunctionInstruction(FunctionInstruction *instruction) {
	switch (instruction->type) {
		case FUNCTION_INSTRUCTION_VARIABLE_DEFINITION:
			generateVariableDefinition(NULL, instruction->variableDefinition);
			output(";\n");
			break;
		case FUNCTION_INSTRUCTION_CONDITIONAL:
			generateConditional(instruction->conditional);
			break;
		case FUNCTION_INSTRUCTION_FUNCTION_CALL:
			generateFunctionCall(instruction->functionCall);
			output(";\n");
			break;
		case FUNCTION_INSTRUCTION_MEMBER_CALL:
			generateMemberCall(instruction->memberCall);
			output(";\n");
			break;
		case FUNCTION_INSTRUCTION_EMIT_EVENT:
			output("emit %s(", instruction->eventIdentifier);
			generateArguments(instruction->eventArgs);
			output(");\n");
			break;
		case FUNCTION_INSTRUCTION_ASSIGNMENT:
			generateAssignment(instruction->assignment);
			output(";\n");
			break;
		case FUNCTION_INSTRUCTION_MATH_ASSIGNMENT:
			generateMathAssignment(instruction->mathAssignment);
			output(";\n");
			break;
		case FUNCTION_INSTRUCTION_LOOP:
			generateLoop(instruction->loop);
			break;
	}
}

static void generateConditional(Conditional *conditional) {
	output("if (");
	generateExpression(conditional->condition);
	output(") ");
	generateFunctionBlock(conditional->ifBlock);
	if (conditional->type == CONDITIONAL_WITH_ELSE) {
		output("else ");
		generateFunctionBlock(conditional->elseBlock);
	}
}

static void generateMemberCall(MemberCall *memberCall) {
	generateAssignable(memberCall->instance);
	output(".");
	generateFunctionCall(memberCall->method);
}

static void generateLoop(Loop *loop) {
	output("for (");
	generateLoopInitialization(loop->loopInitialization);
	output("; ");
	generateLoopCondition(loop->loopCondition);
	output("; ");
	generateLoopIteration(loop->loopIteration);
	output(") ");
	generateFunctionBlock(loop->functionBlock);
}

static void generateLoopInitialization(LoopInitialization *loopInit) {
	switch (loopInit->type) {
		case LOOP_INITIALIZATION_VARIABLE_DEFINITION:
			generateVariableDefinition(NULL, loopInit->variable);
			break;
		case LOOP_INITIALIZATION_ASSIGNMENT:
			generateAssignment(loopInit->assignment);
			break;
		case LOOP_INITIALIZATION_MATH_ASSIGNMENT:
			generateMathAssignment(loopInit->mathAssignment);
			break;
		case LOOP_INITIALIZATION_EMPTY:
			break;
	}
}

static void generateLoopCondition(LoopCondition *condition) {
	if (condition->type == LOOP_CONDITION_CONDITIONAL)
		generateExpression(condition->condition);
}

static void generateLoopIteration(LoopIteration *loopIteration) {
	switch (loopIteration->type) {
		case LOOP_ITERATION_ASSIGNMENT:
			generateAssignment(loopIteration->assignment);
			break;
		case LOOP_ITERATION_MATH_ASSIGNMENT:
			generateMathAssignment(loopIteration->mathAssignment);
			break;
		case LOOP_ITERATION_EMPTY:
			break;
	}
}

static void generateAssignment(Assignment *assignment) {
	generateAssignable(assignment->assignable);
	output(" = ");
	switch (assignment->type) {
		case ASSIGNMENT_EXPRESSION:
			generateExpression(assignment->expression);
			break;
		case ASSIGNMENT_FUNCTION_CALL:
			generateFunctionCall(assignment->functionCall);
			break;
	}
}

static void generateMathAssignment(MathAssignment *mathAssignment) {
	generateAssignable(mathAssignment->variable);
	switch (mathAssignment->type) {
		case MATH_ASSIGNMENT_OPERATOR:
			generateMathAssignmentOperator(mathAssignment->operator);
			generateExpression(mathAssignment->expression);
			break;
		case MATH_ASSIGNMENT_INCREMENT:
			output("++");
			break;
		case MATH_ASSIGNMENT_DECREMENT:
			output("--");
			break;
	}
}

static void generateMathAssignmentOperator(MathAssignmentOperator *operator) {
	switch (operator->type) {
		case MATH_ASSIGNMENT_OP_ADD_EQUAL:
			output(" += ");
			break;
		case MATH_ASSIGNMENT_OP_SUBTRACT_EQUAL:
			output(" -= ");
			break;
		case MATH_ASSIGNMENT_OP_MULTIPLY_EQUAL:
			output(" *= ");
			break;
		case MATH_ASSIGNMENT_OP_DIVIDE_EQUAL:
			output(" /= ");
			break;
		case MATH_ASSIGNMENT_OP_MODULO_EQUAL:
			output(" %%= ");
			break;
	}
}

static void generateParameters(Parameters *params, boolean addMemoryLoc) {
	if (params->type == PARAMETERS_MULTIPLE) {
		generateParameters(params->parameters, addMemoryLoc);
		output(", ");
	}
	generateDataType(params->dataType);

	if (addMemoryLoc) {
		switch (params->dataType->type) {
			case DATA_TYPE_BYTES:
			case DATA_TYPE_STRING:
			case DATA_TYPE_ARRAY:
				output(" memory");
				break;
		}
	}

	output(" %s", params->identifier);
}

static void generateDataType(DataType *dataType) {
	if (dataType->type == DATA_TYPE_ARRAY) {
		generateDataType(dataType->dataType);
		output("[]");
	} else {
		switch (dataType->type) {
			case DATA_TYPE_ERC20:
				output("IERC20");
				break;
			case DATA_TYPE_ERC721:
				output("IERC721");
				break;
			case DATA_TYPE_BYTES:
				output("bytes");
				break;
			case DATA_TYPE_STRING:
				output("string");
				break;
			case DATA_TYPE_BOOLEAN:
				output("bool");
				break;
			case DATA_TYPE_ADDRESS:
				output("address");
				break;
			case DATA_TYPE_UINT:
				output("uint");
				break;
			case DATA_TYPE_INT:
				output("int");
				break;
		}
	}
}

static void generateExpression(Expression *expression) {
	if (expression->type != EXPRESSION_NOT && expression->type != EXPRESSION_FACTOR)
		generateExpression(expression->left);
	
	switch (expression->type) {
		case EXPRESSION_ADDITION:
			output(" + ");
			break;
		case EXPRESSION_SUBTRACTION:
			output(" - ");
			break;
		case EXPRESSION_MULTIPLICATION:
			output(" * ");
			break;
		case EXPRESSION_DIVISION:
			output(" / ");
			break;
		case EXPRESSION_MODULO:
			output(" %% ");
			break;
		case EXPRESSION_EQUALITY:
			output(" == ");
			break;
		case EXPRESSION_INEQUALITY:
			output(" != ");
			break;
		case EXPRESSION_LESS_THAN:
			output(" < ");
			break;
		case EXPRESSION_LESS_THAN_OR_EQUAL:
			output(" <= ");
			break;
		case EXPRESSION_GREATER_THAN:
			output(" > ");
			break;
		case EXPRESSION_GREATER_THAN_OR_EQUAL:
			output(" >= ");
			break;
		case EXPRESSION_AND:
			output(" && ");
			break;
		case EXPRESSION_OR:
			output(" || ");
			break;
		case EXPRESSION_NOT:
			output("!");
			break;
		case EXPRESSION_FACTOR:
			generateFactor(expression->factor);
			break;
	}

	if (expression->type != EXPRESSION_FACTOR)
		generateExpression(expression->right);
}

static void generateFactor(Factor *factor) {
	switch (factor->type) {
		case FACTOR_EXPRESSION:
			output("(");
			generateExpression(factor->expression);
			output(")");
			break;
		case FACTOR_CONSTANT:
			generateConstant(factor->constant);
			break;
	}
}

static void generateConstant(Constant *constant) {
	switch (constant->type) {
		case CONSTANT_INTEGER:
			output("%d", constant->value);
			break;
		case CONSTANT_VARIABLE:
			generateAssignable(constant->variable);
			break;
		case CONSTANT_BOOLEAN:
			output("%s", constant->value ? "true" : "false");
			break;
		case CONSTANT_STRING:
			output("%s", constant->string);
			break;
		case CONSTANT_ADDRESS:
		case CONSTANT_SCIENTIFIC_NOTATION:
			output("%s", constant->string);
			break;
	}
}

static void generateAssignable(Assignable *assignable) {
	output("%s", assignable->identifier);
	if (assignable->type == ASSIGNABLE_ARRAY) {
		output("[");
		generateExpression(assignable->arrayIndex);
		output("]");
	}
}
