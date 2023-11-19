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
ContractDefinition *ContractDefinitionGrammarAction(char *identifier, struct ContractBlock *block);
ContractBlock *ContractBlockGrammarAction(struct ContractInstructions *instructions);
FunctionBlock *FunctionBlockGrammarAction(struct FunctionInstructions *instructions);
Conditional *ConditionalGrammarAction(struct Expression *condition, struct FunctionBlock *ifBlock, struct FunctionBlock *elseBlock);
ContractInstructions *ContractInstructionsGrammarAction(struct ContractInstructions *instructions, struct ContractInstruction *instruction);

ContractInstruction *StateVariableContractInstructionGrammarAction(struct Decorators *variableDecorators, struct VariableDefinition *variableDefinition);
ContractInstruction *FunctionDefinitionContractInstructionGrammarAction(struct FunctionDefinition *functionDefinition);
ContractInstruction *EventDefinitionContractInstructionGrammarAction(char *eventIdentifier, struct ParameterDefinition *eventParams);

FunctionInstructions *FunctionInstructionsGrammarAction(struct FunctionInstructions *instructions, struct FunctionInstruction *instruction);

FunctionInstruction *VariableDefinitionFunctionInstructionGrammarAction(struct VariableDefinition *variableDefinition);
FunctionInstruction *ConditionalFunctionInstructionGrammarAction(struct Conditional *conditional);
FunctionInstruction *FunctionCallFunctionInstructionGrammarAction(struct FunctionCall *functionCall);
FunctionInstruction *MemberCallFunctionInstructionGrammarAction(struct MemberCall *memberCall);
FunctionInstruction *EmitEventFunctionInstructionGrammarAction(char *eventIdentifier, struct Arguments *eventArgs);
FunctionInstruction *AssignmentFunctionInstructionGrammarAction(struct Assignment *assignment);
FunctionInstruction *MathAssignmentFunctionInstructionGrammarAction(struct MathAssignment *mathAssignment);
FunctionInstruction *LoopFunctionInstructionGrammarAction(struct Loop *loop);

Loop * LoopGrammarAction(struct LoopInitialization *loopInitialization, struct LoopCondition *loopCondition, struct LoopIteration *loopIteration, struct FunctionBlock *functionBlock);


int InstructionsGrammarAction(int instructions, int instruction);
int VariableDefinitionGrammarAction(int dataType, char *name, char *value);
int FunctionInstructionGrammarAction();
int FunctionDefinitionGrammarAction(char *functionName);

int EmptyArgumentListGrammarAction();
int ArgumentListGrammarAction();
int ArgumentDefinitionGrammarAction();

// Instruction
DataType *DataTypeSimpleGrammarAction(DataTypeType dataTypeType);
DataType *DataTypeArrayGrammarAction(struct DataType *dataType, struct Expression *expression);

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
