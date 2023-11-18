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
Program *ProgramGrammarAction(ContractDefinition *contract);

// Expresión.
ContractDefinition *ContractDefinitionGrammarAction(char *identifier, ContractBlock *block);
ContractBlock *ContractBlockGrammarAction(ContractInstructions *instructions);
FunctionBlock *FunctionBlockGrammarAction(FunctionInstructions *instructions);
Conditional *ConditionalGrammarAction(Expression *condition, FunctionBlock *ifBlock, FunctionBlock *elseBlock);
ContractInstructions *ContractInstructionsGrammarAction(ContractInstructions *instructions, ContractInstruction *instruction);

ContractInstruction *StateVariableContractInstructionGrammarAction(Decorators *variableDecorators, VariableDefinition *variableDefinition);
ContractInstruction *FunctionDefinitionContractInstructionGrammarAction(FunctionDefinition *functionDefinition);
ContractInstruction *EventDefinitionContractInstructionGrammarAction(char *eventIdentifier, ParameterDefinition *eventParams);

FunctionInstructions *FunctionInstructionsGrammarAction(FunctionInstructions *instructions, FunctionInstruction *instruction);

FunctionInstruction *VariableDefinitionFunctionInstructionGrammarAction(VariableDefinition *variableDefinition);
FunctionInstruction *ConditionalFunctionInstructionGrammarAction(Conditional *conditional);
FunctionInstruction *FunctionCallFunctionInstructionGrammarAction(FunctionCall *functionCall);
FunctionInstruction *MemberCallFunctionInstructionGrammarAction(MemberCall *memberCall);
FunctionInstruction *EmitEventFunctionInstructionGrammarAction(char *eventIdentifier, Arguments *eventArgs);
FunctionInstruction *AssignmentFunctionInstructionGrammarAction(Assignment *assignment);
FunctionInstruction *MathAssignmentFunctionInstructionGrammarAction(MathAssignment *mathAssignment);
FunctionInstruction *LoopFunctionInstructionGrammarAction(Loop *loop);

Loop * LoopGrammarAction(LoopInitialization *loopInitialization, LoopCondition *loopCondition, LoopIteration *loopIteration, FunctionBlock *functionBlock);


int InstructionsGrammarAction(int instructions, int instruction);
int VariableDefinitionGrammarAction(int dataType, char *name, char *value);
int FunctionInstructionGrammarAction();
int FunctionDefinitionGrammarAction(char *functionName);

int EmptyArgumentListGrammarAction();
int ArgumentListGrammarAction();
int ArgumentDefinitionGrammarAction();

// Instruction
DataType *DataTypeSimpleGrammarAction(DataTypeType dataTypeType);
DataType *DataTypeArrayGrammarAction(DataType *dataType, Expression *expression);

int EmptyInstructionGrammarAction();

Expression *AdditionExpressionGrammarAction(Expression *leftExpression, Expression *rightExpression);
int SubtractionExpressionGrammarAction(int leftValue, int rightValue);
int MultiplicationExpressionGrammarAction(int leftValue, int rightValue);
int DivisionExpressionGrammarAction(int leftValue, int rightValue);
int ModuloExpressionGrammarAction(int leftValue, int rightValue);
int ExponentiationExpressionGrammarAction(int leftValue, int rightValue);
int EqualityExpressionGrammarAction(int leftValue, int rightValue);
int InequalityExpressionGrammarAction(int leftValue, int rightValue);
int LessThanExpressionGrammarAction(int leftValue, int rightValue);
int LessThanOrEqualExpressionGrammarAction(int leftValue, int rightValue);
int GreaterThanExpressionGrammarAction(int leftValue, int rightValue);
int GreaterThanOrEqualExpressionGrammarAction(int leftValue, int rightValue);
int AndExpressionGrammarAction(int leftValue, int rightValue);
int OrExpressionGrammarAction(int leftValue, int rightValue);
int NotExpressionGrammarAction(int value);
int FactorExpressionGrammarAction(int value);

// Factores.
int ExpressionFactorGrammarAction(int value);
int ConstantFactorGrammarAction(int value);

// Constantes.
Constant *StringConstantGrammarAction(ConstantType type, char *str);
Constant *IntegerConstantGrammarAction(ConstantType type, int value);
Constant *AssignableConstantGrammarAction(Assignable *variable);

#endif
