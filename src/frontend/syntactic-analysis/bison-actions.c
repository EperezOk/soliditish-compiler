#include <stdio.h>
#include <string.h>

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
int ProgramGrammarAction(const int value) {
	LogDebug("[Bison] ProgramGrammarAction(%d)", value);
	/*
	* "state" es una variable global que almacena el estado del compilador,
	* cuyo campo "succeed" indica si la compilación fue o no exitosa, la cual
	* es utilizada en la función "main".
	*/
	state.succeed = true;
	/*
	* Por otro lado, "result" contiene el resultado de aplicar el análisis
	* sintáctico mediante Bison, y almacenar el nood raíz del AST construido
	* en esta variable. Para el ejemplo de la calculadora, no hay AST porque
	* la expresión se computa on-the-fly, y es la razón por la cual esta
	* variable es un simple entero, en lugar de un nodo.
	*/
	state.result = value;
	return value;
}

int ContractDefinitionGrammarAction(const char *contractName) {
	LogDebug("[Bison] ContractDefinitionGrammarAction(%s)", contractName);
	return 0;
}

int BlockGrammarAction(const int value) {
	LogDebug("[Bison] BlockGrammarAction(%d)", value);
	return 0;
}

int InstructionsGrammarAction(const int instructions, const int instruction) {
	LogDebug("[Bison] InstructionsGrammarAction(%d, %d)", instructions, instruction);
	return 0;
}

int ERC20DefinitionGrammarAction() {
	LogDebug("[Bison] ERC20DefinitionGrammarAction()");
	return 0;
}

int EmptyInstructionGrammarAction() {
	LogDebug("[Bison] EmptyInstructionsGrammarAction");
	return 0;
}

int VariableDefinitionGrammarAction(const int dataType, const char *name, const char *value) {
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

int AdditionExpressionGrammarAction(const int leftValue, const int rightValue) {
	LogDebug("[Bison] AdditionExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return Add(leftValue, rightValue);
}

int SubtractionExpressionGrammarAction(const int leftValue, const int rightValue) {
	LogDebug("[Bison] SubtractionExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return Subtract(leftValue, rightValue);
}

int MultiplicationExpressionGrammarAction(const int leftValue, const int rightValue) {
	LogDebug("[Bison] MultiplicationExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return Multiply(leftValue, rightValue);
}

int DivisionExpressionGrammarAction(const int leftValue, const int rightValue) {
	LogDebug("[Bison] DivisionExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return Divide(leftValue, rightValue);
}

int ModuloExpressionGrammarAction(const int leftValue, const int rightValue) {
	LogDebug("[Bison] ModuloExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return 0;
}

int ExponentiationExpressionGrammarAction(const int leftValue, const int rightValue) {
	LogDebug("[Bison] ExponentiationExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return 0;
}

int EqualityExpressionGrammarAction(const int leftValue, const int rightValue) {
	LogDebug("[Bison] EqualityExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return 0;
}
int InequalityExpressionGrammarAction(const int leftValue, const int rightValue) {
	LogDebug("[Bison] InequalityExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return 0;
}

int LessThanExpressionGrammarAction(const int leftValue, const int rightValue) {
	LogDebug("[Bison] LessThanExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return 0;
}

int LessThanOrEqualExpressionGrammarAction(const int leftValue, const int rightValue) {
	LogDebug("[Bison] LessThanOrEqualExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return 0;
}

int GreaterThanExpressionGrammarAction(const int leftValue, const int rightValue) {
	LogDebug("[Bison] GreaterThanExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return 0;
}

int GreaterThanOrEqualExpressionGrammarAction(const int leftValue, const int rightValue) {
	LogDebug("[Bison] GreaterThanOrEqualExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return 0;
}

int AndExpressionGrammarAction(const int leftValue, const int rightValue) {
	LogDebug("[Bison] AndExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return 0;
}

int OrExpressionGrammarAction(const int leftValue, const int rightValue) {
	LogDebug("[Bison] OrExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return 0;
}

int NotExpressionGrammarAction(const int value) {
	LogDebug("[Bison] NotExpressionGrammarAction(%d)", value);
	return 0;
}

int FactorExpressionGrammarAction(const int value) {
	LogDebug("[Bison] FactorExpressionGrammarAction(%d)", value);
	return value;
}

int ExpressionFactorGrammarAction(const int value) {
	LogDebug("[Bison] ExpressionFactorGrammarAction(%d)", value);
	return value;
}

int ConstantFactorGrammarAction(const int value) {
	LogDebug("[Bison] ConstantFactorGrammarAction(%d)", value);
	return value;
}

int IntegerConstantGrammarAction(const int value) {
	LogDebug("[Bison] IntegerConstantGrammarAction(%d)", value);
	return value;
}
