#include <stdarg.h>
#include "../support/logger.h"
#include "../support/shared.h"
#include "generator.h"

/**
 * Implementación de "generator.h".
 */

FILE *outputFile = NULL;
int indentationLevel = 0;
int indentationSize = 4;
boolean indentUsingSpaces = true;
boolean indent = false;

static void output(const char *format, ...) {
	va_list args;
	va_start(args, format);

	// Adjust indentation level
	if (format[0] == '}') --indentationLevel;
	else if (format[0] == '{') ++indentationLevel;

	// Add indentation
	if (indent) {
		for (int i = 0; i < indentationLevel; ++i) {
			for (int j = 0; j < indentationSize; ++j) {
				fprintf(outputFile, "%s", indentUsingSpaces ? " " : "\t");
			}
		}
	}

	vfprintf(outputFile, format, args);

	// Indent next line
	if (format[strlen(format) - 1] == '\n') indent = true;
	else indent = false;

	va_end(args);
}

static void generateProgram(Program *program);
static void generateContractInstructions(ContractInstructions *instructions);
static void generateContractInstruction(ContractInstruction *instruction);
static void generateDecorators(Decorators *decorators);
static void generateVariableDefinition(VariableDefinition *definition);
static void generateFunctionDefinition(FunctionDefinition *definition);
static void generateParameterDefinition(ParameterDefinition *definition);
static void generateParameters(Parameters *params);
static void generateDataType(DataType *dataType);
static void generateExpression(Expression *expression);
static void generateFactor(Factor *factor);
static void generateConstant(Constant *constant);
static void generateAssignable(Assignable *assignable);

void Generator(FILE *out, int indentSize, boolean indentSpaces) {
	LogInfo("Generating output...");
	outputFile = out;
	indentationSize = indentSize;
	indentUsingSpaces = indentSpaces;
	generateProgram(state.program);
}

static void generateProgram(Program *program) {
	output("contract %s ", program->contract->identifier);
	output("{\n");
	generateContractInstructions(program->contract->block->instructions);
	output("}\n");
}

static void generateContractInstructions(ContractInstructions *instructions) {
	if (instructions->type == CONTRACT_INSTRUCTIONS_MULTIPLE) {
		generateContractInstructions(instructions->instructions);
		generateContractInstruction(instructions->instruction);
	}
	// else: CONTRACT_INSTRUCTIONS_EMPTY, do nothing
}

static void generateContractInstruction(ContractInstruction *instruction) {
	switch (instruction->type) {
		case STATE_VARIABLE_DECLARATION:
			generateDecorators(instruction->variableDecorators);
			generateVariableDefinition(instruction->variableDefinition);
			break;
		case FUNCTION_DECLARATION:
			generateFunctionDefinition(instruction->functionDefinition);
			break;
		case EVENT_DECLARATION:
			output("event %s", instruction->eventIdentifier);
			generateParameterDefinition(instruction->eventParams);
			output(";\n");
			break;
	}
}

static void generateDecorators(Decorators *decorators) {

}

static void generateVariableDefinition(VariableDefinition *definition) {

}

static void generateFunctionDefinition(FunctionDefinition *definition) {

}

static void generateParameterDefinition(ParameterDefinition *definition) {
	output("(");

	if (definition->type == PARAMETERS_DEFINITION_WITH_PARAMS)
		generateParameters(definition->parameters);

	output(")");
}

static void generateParameters(Parameters *params) {
	if (params->type == PARAMETERS_MULTIPLE) {
		generateParameters(params->parameters);
		output(", ");
	}
	generateDataType(params->dataType);
	output(" %s", params->identifier);
}

static void generateDataType(DataType *dataType) {
	if (dataType->type == DATA_TYPE_DYNAMIC_ARRAY) {
		generateDataType(dataType->dataType);
		output("[]");
	} else if (dataType->type == DATA_TYPE_STATIC_ARRAY) {
		generateDataType(dataType->dataType);
		output("[");
		generateExpression(dataType->expression);
		output("]");
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
			output("\"%s\"", constant->string);
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
