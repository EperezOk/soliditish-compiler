#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../backend/support/shared.h"
#include "../../backend/semantic-analysis/abstract-syntax-tree.h"

/**
 * Se definen las acciones a ejecutar sobre cada regla de producción de la
 * gramática. El objetivo de cada acción debe ser el de construir el nodo
 * adecuado que almacene la información requerida en el árbol de sintaxis
 * abstracta (i.e., el AST).
 */

// Programa.
int ProgramGrammarAction(const int value);

// Expresión.
int ContractDefinitionGrammarAction(const char *contractName);
int BlockGrammarAction(const int value);
int InstructionsGrammarAction(const int instructions, const int instruction);
int VariableDefinitionGrammarAction(const int dataType, const char *name, const char *value);
int FunctionInstructionGrammarAction();
int FunctionDefinitionGrammarAction(char *functionName);

int EmptyArgumentListGrammarAction();
int ArgumentListGrammarAction();
int ArgumentDefinitionGrammarAction();

// Instruction
int ERC20DefinitionGrammarAction();
int EmptyInstructionGrammarAction();

int AdditionExpressionGrammarAction(const int leftValue, const int rightValue);
int SubtractionExpressionGrammarAction(const int leftValue, const int rightValue);
int MultiplicationExpressionGrammarAction(const int leftValue, const int rightValue);
int DivisionExpressionGrammarAction(const int leftValue, const int rightValue);
int ModuloExpressionGrammarAction(const int leftValue, const int rightValue);
int ExponentiationExpressionGrammarAction(const int leftValue, const int rightValue);
int EqualityExpressionGrammarAction(const int leftValue, const int rightValue);
int InequalityExpressionGrammarAction(const int leftValue, const int rightValue);
int LessThanExpressionGrammarAction(const int leftValue, const int rightValue);
int LessThanOrEqualExpressionGrammarAction(const int leftValue, const int rightValue);
int GreaterThanExpressionGrammarAction(const int leftValue, const int rightValue);
int GreaterThanOrEqualExpressionGrammarAction(const int leftValue, const int rightValue);
int AndExpressionGrammarAction(const int leftValue, const int rightValue);
int OrExpressionGrammarAction(const int leftValue, const int rightValue);
int NotExpressionGrammarAction(const int value);
int FactorExpressionGrammarAction(const int value);

// Factores.
int ExpressionFactorGrammarAction(const int value);
int ConstantFactorGrammarAction(const int value);

// Constantes.
int IntegerConstantGrammarAction(const int value);

#endif
