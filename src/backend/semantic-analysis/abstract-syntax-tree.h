#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

typedef struct Program{
	struct ContractDefinition *contract;
} Program;

typedef struct ContractDefinition{
	char *identifier;
	struct ContractBlock *block;
} ContractDefinition;

typedef struct ContractBlock{
	struct ContractInstructions *instructions;
} ContractBlock;

typedef struct FunctionBlock{
	struct FunctionInstructions *instructions;
} FunctionBlock;

typedef enum {
	NO_ELSE,
	WITH_ELSE,
} ConditionalType;

typedef struct Conditional{
	ConditionalType type;
	struct Expression *condition;
	struct FunctionBlock *ifBlock;
	struct FunctionBlock *elseBlock;
} Conditional;

typedef enum {
	CONTRACT_INSTRUCTIONS_MULTIPLE,
	CONTRACT_INSTRUCTIONS_EMPTY,
} ContractInstructionsType;

typedef struct ContractInstructions {
	ContractInstructionsType type;
	struct ContractInstructions *instructions;
	struct ContractInstruction *instruction;
} ContractInstructions;

typedef enum {
	STATE_VARIABLE_DECLARATION,
	FUNCTION_DECLARATION,
	EVENT_DECLARATION,
} ContractInstructionType;

typedef struct ContractInstruction{
	ContractInstructionType type;
	struct Decorators *variableDecorators;
	struct VariableDefinition *variableDefinition;
	struct FunctionDefinition *functionDefinition;
	char *eventIdentifier;
	struct ParameterDefinition *eventParams;
} ContractInstruction;

typedef enum {
	FUNCTION_INSTRUCTIONS_MULTIPLE,
	FUNCTION_INSTRUCTIONS_EMPTY,
} FunctionInstructionsType;

typedef struct FunctionInstructions {
	FunctionInstructionsType type;
	struct FunctionInstructions *instructions;
	struct FunctionInstruction *instruction;
} FunctionInstructions;

typedef enum {
	FUNCTION_INSTRUCTION_VARIABLE_DEFINITION,
	FUNCTION_INSTRUCTION_CONDITIONAL,
	FUNCTION_INSTRUCTION_FUNCTION_CALL,
	FUNCTION_INSTRUCTION_MEMBER_CALL,
	FUNCTION_INSTRUCTION_EMIT_EVENT,
	FUNCTION_INSTRUCTION_ASSIGNMENT,
	FUNCTION_INSTRUCTION_MATH_ASSIGNMENT,
	FUNCTION_INSTRUCTION_LOOP,
} FunctionInstructionType;

typedef struct FunctionInstruction{
	FunctionInstructionType type;
	struct VariableDefinition *variableDefinition;
	struct Conditional *conditional;
	struct FunctionCall *functionCall;
	struct MemberCall *memberCall;
	char *eventIdentifier;
	struct Arguments *eventArgs;
	struct Assignment *assignment;
	struct MathAssignment *mathAssignment;
	struct Loop *loop;
} FunctionInstruction;

typedef struct Loop{
	struct LoopInitialization *loopInitialization;
	struct LoopCondition *loopCondition;
	struct LoopIteration *loopIteration;
	struct FunctionBlock *functionBlock;
} Loop;

typedef enum {
	LOOP_INITIALIZATION_VARIABLE_DEFINITION,
	LOOP_INITIALIZATION_ASSIGNMENT,
	LOOP_INITIALIZATION_MATH_ASSIGNMENT,
	LOOP_INITIALIZATION_EMPTY,
} LoopInitializationType;

typedef struct LoopInitialization{
	LoopInitializationType type;
	struct VariableDefinition *variable;
	struct Assignment *assignment;
	struct MathAssignment *mathAssignment;
} LoopInitialization;

typedef enum {
	LOOP_CONDITION_CONDITIONAL,
	LOOP_CONDITION_EMPTY,
} LoopConditionType;

typedef struct LoopCondition{
	LoopConditionType type;
	struct Expression *condition;
} LoopCondition;

typedef enum {
	LOOP_ITERATION_ASSIGNMENT,
	LOOP_ITERATION_MATH_ASSIGNMENT,
	LOOP_ITERATION_EMPTY,
} LoopIterationType;

typedef struct LoopIteration{
	LoopIterationType type;
	struct Assignment *assignment;
	struct MathAssignment *mathAssignment;
} LoopIteration;

typedef enum {
	ASSIGNABLE_VARIABLE,
	ASSIGNABLE_ARRAY,
} AssignableType;

typedef struct Assignable{
	AssignableType type;
	struct Expression *arrayIndex;
	char *identifier;
} Assignable;

typedef enum {
	ASSIGNMENT_EXPRESSION,
	ASSIGNMENT_ARRAY_INITIALIZATION,
	ASSIGNMENT_FUNCTION_CALL,
} AssignmentType;

typedef struct Assignment{
	AssignmentType type;
	struct Assignable *assignable;
	struct Expression *expression;
	struct Arguments *arrayElements;
	struct FunctionCall *functionCall;
} Assignment;

typedef enum {
	MATH_ASSIGNMENT_OPERATOR,
	MATH_ASSIGNMENT_INCREMENT,
	MATH_ASSIGNMENT_DECREMENT,
} MathAssignmentType;

typedef struct MathAssignment{
	MathAssignmentType type;
	struct Assignable *variable;
	struct MathAssignmentOperator *operator;
	struct Expression *expression;
} MathAssignment;

typedef enum {
	MATH_ASSIGNMENT_OP_ADD_EQUAL,
	MATH_ASSIGNMENT_OP_SUBTRACT_EQUAL,
	MATH_ASSIGNMENT_OP_MULTIPLY_EQUAL,
	MATH_ASSIGNMENT_OP_DIVIDE_EQUAL,
	MATH_ASSIGNMENT_OP_MODULO_EQUAL,
} MathAssignmentOperatorType;

typedef struct MathAssignmentOperator{
	MathAssignmentOperatorType type;
} MathAssignmentOperator;

typedef enum {
	FUNCTION_CALL_NO_ARGS,
	FUNCTION_CALL_WITH_ARGS
} FunctionCallType;

typedef struct FunctionCall{
	FunctionCallType type;
	struct Arguments *arguments;
	char *identifier;
} FunctionCall;

typedef enum {
	ARGUMENTS_MULTIPLE,
	ARGUMENTS_SINGLE
} ArgumentsType;

typedef struct Arguments {
	ArgumentsType type;
	struct Arguments *arguments;
	struct Expression *expression;
} Arguments;

typedef struct MemberCall{
	struct Assignable *instance;
	struct FunctionCall *method;
} MemberCall;

typedef enum VariableDefinitionType{
	VARIABLE_DEFINITION_DECLARATION,
	VARIABLE_DEFINITION_INITIALIZATION
} VariableDefinitionType;

typedef struct VariableDefinition{
	VariableDefinitionType type;
	struct DataType *dataType;
	struct Expression *expression;
	struct FunctionCall *functionCall;
	char *identifier;
} VariableDefinition;

typedef enum {
	DATA_TYPE_ERC20,
	DATA_TYPE_ERC721,
	DATA_TYPE_BYTES,
	DATA_TYPE_STRING,
	DATA_TYPE_BOOLEAN,
	DATA_TYPE_ADDRESS,
	DATA_TYPE_UINT,
	DATA_TYPE_INT,
	DATA_TYPE_DYNAMIC_ARRAY,
	DATA_TYPE_STATIC_ARRAY
} DataTypeType;

typedef struct DataType {
	DataTypeType type;
	struct DataType *dataType;
	struct Expression *expression;
} DataType;

typedef struct FunctionDefinition{
	struct Decorators *decorators;
	struct ParameterDefinition *parameterDefinition;
	struct FunctionBlock *functionBlock;
	char *identifier;
} FunctionDefinition;

typedef enum {
	DECORATORS_WITH_DECORATORS,
	DECORATORS_NO_DECORATORS
} DecoratorsType;

typedef struct Decorators {
	DecoratorsType type;
	struct Decorators *decorators;
	char *decorator;
} Decorators;

typedef enum {
	PARAMETERS_DEFINITION_NO_PARAMS,
	PARAMETERS_DEFINITION_WITH_PARAMS
} ParameterDefinitionType;

typedef struct ParameterDefinition{
	ParameterDefinitionType type;
	struct Parameters *parameters;
} ParameterDefinition;

typedef enum {
	PARAMETERS_MULTIPLE,
	PARAMETERS_SINGLE
} ParametersType;

typedef struct Parameters {
	ParametersType type;
	struct Parameters *parameters;
	struct DataType *dataType;
	char *identifier;
} Parameters;

typedef enum {
	EXPRESSION_ADDITION,
	EXPRESSION_SUBTRACTION,
	EXPRESSION_MULTIPLICATION,
	EXPRESSION_DIVISION,
	EXPRESSION_MODULO,
	EXPRESSION_EXPONENTIATION,
	EXPRESSION_EQUALITY,
	EXPRESSION_INEQUALITY,
	EXPRESSION_LESS_THAN,
	EXPRESSION_LESS_THAN_OR_EQUAL,
	EXPRESSION_GREATER_THAN,
	EXPRESSION_GREATER_THAN_OR_EQUAL,
	EXPRESSION_AND,
	EXPRESSION_OR,
	EXPRESSION_NOT,
	EXPRESSION_FACTOR
} ExpressionType;

typedef struct Expression {
	ExpressionType type;
	struct Expression *left;
	struct Expression *right;
	struct Factor *factor;
} Expression;

typedef enum {
	FACTOR_EXPRESSION,
	FACTOR_CONSTANT
} FactorType;

typedef struct Factor {
	FactorType type;
	struct Expression *expression;
	struct Constant *constant;
} Factor;

typedef enum {
	CONSTANT_INTEGER,
	CONSTANT_VARIABLE,
	CONSTANT_BOOLEAN,
	CONSTANT_STRING,
	CONSTANT_ADDRESS,
	CONSTANT_SCIENTIFIC_NOTATION
} ConstantType;

typedef struct Constant{
	ConstantType type;
	struct Assignable *variable;
	int value;
	char *string;
} Constant;

#endif
