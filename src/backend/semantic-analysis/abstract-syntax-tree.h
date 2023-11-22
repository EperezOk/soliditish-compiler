#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER


// Nodes types
typedef struct Program Program;
typedef struct ContractDefinition ContractDefinition;
typedef struct ContractBlock ContractBlock;
typedef struct FunctionBlock FunctionBlock;
typedef struct Conditional Conditional;
typedef struct ContractInstructions ContractInstructions;
typedef struct ContractInstruction ContractInstruction;
typedef struct FunctionInstructions FunctionInstructions;
typedef struct FunctionInstruction FunctionInstruction;
typedef struct Loop Loop;
typedef struct LoopInitialization LoopInitialization;
typedef struct LoopCondition LoopCondition;
typedef struct LoopIteration LoopIteration;
typedef struct Assignable Assignable;
typedef struct Assignment Assignment;
typedef struct MathAssignment MathAssignment;
typedef struct MathAssignmentOperator MathAssignmentOperator;
typedef struct FunctionCall FunctionCall;
typedef struct Arguments Arguments;
typedef struct MemberCall MemberCall;
typedef struct VariableDefinition VariableDefinition;
typedef struct DataType DataType;
typedef struct FunctionDefinition FunctionDefinition;
typedef struct Decorators Decorators;
typedef struct ParameterDefinition ParameterDefinition;
typedef struct Parameters Parameters;
typedef struct Expression Expression;
typedef struct Factor Factor;
typedef struct Constant Constant;

// Nodes

struct Program{
	ContractDefinition *contract;
};

struct ContractDefinition{
	char *identifier;
	ContractBlock *block;
};

struct ContractBlock{
	ContractInstructions *instructions;
};

typedef enum {
	CONTRACT_INSTRUCTIONS_MULTIPLE,
	CONTRACT_INSTRUCTIONS_EMPTY,
} ContractInstructionsType;

struct ContractInstructions {
	ContractInstructionsType type;
	ContractInstructions *instructions;
	ContractInstruction *instruction;
};

typedef enum {
	STATE_VARIABLE_DECLARATION,
	FUNCTION_DECLARATION,
	EVENT_DECLARATION,
} ContractInstructionType;

struct ContractInstruction{
	ContractInstructionType type;
	Decorators *variableDecorators;
	VariableDefinition *variableDefinition;
	FunctionDefinition *functionDefinition;
	char *eventIdentifier;
	ParameterDefinition *eventParams;
};

typedef enum {
	DECORATORS_WITH_DECORATORS,
	DECORATORS_NO_DECORATORS
} DecoratorsType;

struct Decorators {
	DecoratorsType type;
	Decorators *decorators;
	char *decorator;
};

struct FunctionBlock{
	FunctionInstructions *instructions;
};

typedef enum {
	CONDITIONAL_NO_ELSE,
	CONDITIONAL_WITH_ELSE,
} ConditionalType;

struct Conditional{
	ConditionalType type;
	Expression *condition;
	FunctionBlock *ifBlock;
	FunctionBlock *elseBlock;
};


typedef enum {
	FUNCTION_INSTRUCTIONS_MULTIPLE,
	FUNCTION_INSTRUCTIONS_EMPTY,
} FunctionInstructionsType;

struct FunctionInstructions {
	FunctionInstructionsType type;
	FunctionInstructions *instructions;
	FunctionInstruction *instruction;
};

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

struct FunctionInstruction{
	FunctionInstructionType type;
	VariableDefinition *variableDefinition;
	Conditional *conditional;
	FunctionCall *functionCall;
	MemberCall *memberCall;
	char *eventIdentifier;
	Arguments *eventArgs;
	Assignment *assignment;
	MathAssignment *mathAssignment;
	Loop *loop;
};

struct Loop{
	LoopInitialization *loopInitialization;
	LoopCondition *loopCondition;
	LoopIteration *loopIteration;
	FunctionBlock *functionBlock;
};

typedef enum {
	LOOP_INITIALIZATION_VARIABLE_DEFINITION,
	LOOP_INITIALIZATION_ASSIGNMENT,
	LOOP_INITIALIZATION_MATH_ASSIGNMENT,
	LOOP_INITIALIZATION_EMPTY,
} LoopInitializationType;

struct LoopInitialization{
	LoopInitializationType type;
	VariableDefinition *variable;
	Assignment *assignment;
	MathAssignment *mathAssignment;
};

typedef enum {
	LOOP_CONDITION_CONDITIONAL,
	LOOP_CONDITION_EMPTY,
} LoopConditionType;

struct LoopCondition{
	LoopConditionType type;
	struct Expression *condition;
};

typedef enum {
	LOOP_ITERATION_ASSIGNMENT,
	LOOP_ITERATION_MATH_ASSIGNMENT,
	LOOP_ITERATION_EMPTY,
} LoopIterationType;

struct LoopIteration{
	LoopIterationType type;
	Assignment *assignment;
	MathAssignment *mathAssignment;
};

typedef enum {
	ASSIGNABLE_VARIABLE,
	ASSIGNABLE_ARRAY,
} AssignableType;

struct Assignable{
	AssignableType type;
	Expression *arrayIndex;
	char *identifier;
};

typedef enum {
	ASSIGNMENT_EXPRESSION,
	ASSIGNMENT_ARRAY_INITIALIZATION,
	ASSIGNMENT_FUNCTION_CALL,
} AssignmentType;

struct Assignment{
	AssignmentType type;
	Assignable *assignable;
	Expression *expression;
	Arguments *arrayElements;
	FunctionCall *functionCall;
};

typedef enum {
	MATH_ASSIGNMENT_OPERATOR,
	MATH_ASSIGNMENT_INCREMENT,
	MATH_ASSIGNMENT_DECREMENT,
} MathAssignmentType;

struct MathAssignment{
	MathAssignmentType type;
	Assignable *variable;
	MathAssignmentOperator *operator;
	Expression *expression;
};

typedef enum {
	MATH_ASSIGNMENT_OP_ADD_EQUAL,
	MATH_ASSIGNMENT_OP_SUBTRACT_EQUAL,
	MATH_ASSIGNMENT_OP_MULTIPLY_EQUAL,
	MATH_ASSIGNMENT_OP_DIVIDE_EQUAL,
	MATH_ASSIGNMENT_OP_MODULO_EQUAL,
} MathAssignmentOperatorType;

struct MathAssignmentOperator{
	MathAssignmentOperatorType type;
};

typedef enum {
	FUNCTION_CALL_NO_ARGS,
	FUNCTION_CALL_WITH_ARGS,
	BUILT_IN_LOG,
	BUILT_IN_CREATE_PROXY_TO,
} FunctionCallType;

struct FunctionCall{
	FunctionCallType type;
	Arguments *arguments;
	char *identifier;
};

typedef enum {
	ARGUMENTS_MULTIPLE,
	ARGUMENTS_SINGLE,
	ARGUMENTS_EMPTY
} ArgumentsType;

struct Arguments {
	ArgumentsType type;
	Arguments *arguments;
	Expression *expression;
};

struct MemberCall{
	Assignable *instance;
	FunctionCall *method;
};

typedef enum VariableDefinitionType {
	VARIABLE_DEFINITION_DECLARATION,
	VARIABLE_DEFINITION_INIT_EXPRESSION,
	VARIABLE_DEFINITION_INIT_FUNCTION_CALL,
} VariableDefinitionType;

struct VariableDefinition{
	VariableDefinitionType type;
	DataType *dataType;
	Expression *expression;
	FunctionCall *functionCall;
	char *identifier;
};

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
	DATA_TYPE_STATIC_ARRAY,
	DATA_TYPE_EVENT,
	DATA_TYPE_FUNCTION
} DataTypeType;

struct DataType {
	DataTypeType type;
	DataType *dataType;
	Expression *expression;
};

struct FunctionDefinition{
	Decorators *decorators;
	ParameterDefinition *parameterDefinition;
	FunctionBlock *functionBlock;
	char *identifier;
};

typedef enum {
	PARAMETERS_DEFINITION_NO_PARAMS,
	PARAMETERS_DEFINITION_WITH_PARAMS
} ParameterDefinitionType;

struct ParameterDefinition{
	ParameterDefinitionType type;
	Parameters *parameters;
};

typedef enum {
	PARAMETERS_MULTIPLE,
	PARAMETERS_SINGLE
} ParametersType;

struct Parameters {
	ParametersType type;
	Parameters *parameters;
	DataType *dataType;
	char *identifier;
};

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

struct Expression {
	ExpressionType type;
	Expression *left;
	Expression *right;
	Factor *factor;
};

typedef enum {
	FACTOR_EXPRESSION,
	FACTOR_CONSTANT
} FactorType;

struct Factor {
	FactorType type;
	Expression *expression;
	Constant *constant;
};

typedef enum {
	CONSTANT_INTEGER,
	CONSTANT_VARIABLE,
	CONSTANT_BOOLEAN,
	CONSTANT_STRING,
	CONSTANT_ADDRESS,
	CONSTANT_SCIENTIFIC_NOTATION
} ConstantType;

struct Constant{
	ConstantType type;
	Assignable *variable;
	int value;
	char *string;
};

#endif
