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

Program *ProgramGrammarAction(ContractDefinition *contract);

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

LoopInitialization *LoopInitVarDefGrammarAction(VariableDefinition *variable);
LoopInitialization *LoopInitAssignmentGrammarAction(Assignment *assignment);
LoopInitialization *LoopInitMathAssignmentGrammarAction(MathAssignment *assignment);

LoopCondition *LoopConditionGrammarAction(Expression *expression);

LoopIteration *LoopIterationAssignmentGrammarAction(Assignment *assignment);
LoopIteration *LoopIterationMathAssignmentGrammarAction(MathAssignment *mathAssignment);

Assignable *AssignableGrammarAction(char *identifier, Expression *arrayIndex);

Assignment *AssignmentExpressionGrammarAction(Assignable *assignable, Expression *expression);
Assignment *AssignmentFunctionCallGrammarAction(Assignable *assignable, FunctionCall *functionCall);

MathAssignment *MathAssignmentGrammarAction(Assignable *variable, MathAssignmentOperator *operator, Expression *expression);
MathAssignment *IncDecGrammarAction(Assignable *variable, MathAssignmentType type);

MathAssignmentOperator *MathAssignmentOperatorGrammarAction(MathAssignmentOperatorType type);

FunctionCall *FunctionCallGrammarAction(char *identifier, Arguments *arguments);

Arguments *ArgumentsGrammarAction(Arguments *arguments, Expression *expression);
Arguments *ArgumentsEmptyGrammarAction();

MemberCall *MemberCallGrammarAction(Assignable *instance, FunctionCall *method);

VariableDefinition *VariableDefExpressionGrammarAction(DataType *dataType, char *identifier, Expression *expression);
VariableDefinition *VariableDefFunctionCallGrammarAction(DataType *dataType, char *identifier, FunctionCall *functionCall);
VariableDefinition *VariableDefinitionGrammarAction(DataType *dataType, char *identifier);

DataType *DataTypeSimpleGrammarAction(DataTypeType dataTypeType);
DataType *DataTypeArrayGrammarAction(DataType *dataType, Expression *expression);

FunctionDefinition *FunctionDefinitionGrammarAction(Decorators *dec, char *id, ParameterDefinition *pd, FunctionBlock *fb);

Decorators *DecoratorsGrammarAction(char *decorator, Decorators *decorators);

ParameterDefinition *ParameterDefinitionGrammarAction(Parameters *parameters);
Parameters *ParametersGrammarAction(Parameters *parameters, DataType *dataType, char *identifier);

Expression *ExpressionGrammarAction(ExpressionType type, Expression *leftExpression, Expression *rightExpression);
Expression *FactorExpressionGrammarAction(Factor *factor);

Factor *ExpressionFactorGrammarAction(Expression *expression);
Factor *ConstantFactorGrammarAction(Constant *constant);

Constant *StringConstantGrammarAction(ConstantType type, char *str);
Constant *IntegerConstantGrammarAction(ConstantType type, int value);
Constant *AssignableConstantGrammarAction(Assignable *variable);

#endif
