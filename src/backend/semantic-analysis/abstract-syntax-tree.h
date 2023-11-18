#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

/**
* Se realiza este tipo de definiciones cuando el tipo de dato debe
* auto-referenciarse, como es el caso de una "Expression", que está compuesta
* de otras 2 expresiones.
*/
typedef struct Expression Expression;
typedef struct Parameters Parameters;
typedef struct DataType DataType;
typedef struct Decorators Decorators;
typedef struct Arguments Arguments;
typedef struct FunctionInstructions FunctionInstructions;
typedef struct ContractInstructions ContractInstructions;

typedef struct {
	ContractDefinition *contract;
} Program;

typedef struct {
	ContractBlock *block;
	char *identifier;
} ContractDefinition;

typedef struct {
	ContractInstructions *instructions;
} ContractBlock;

typedef struct {
	FunctionInstructions *instructions;
} FunctionBlock;

typedef enum {
	NO_ELSE,
	WITH_ELSE,
} ConditionalType;

typedef struct {
	ConditionalType type;
	Expression *condition;
	FunctionBlock *functionBlock;
	FunctionBlock *elseFunctionBlock;
} Conditional;

typedef enum {
	MULTIPLE,
	EMPTY,
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

typedef struct {
	ContractInstructionType type;
	Decorators *variableDecorators;
	VariableDefinition *variableDefinition;
	FunctionDefinition *functionDefinition;
	char *eventIdentifier;
	ParameterDefinition *eventParams;
} ContractInstruction;

typedef enum {
	MULTIPLE,
	EMPTY,
} FunctionInstructionsType;

struct FunctionInstructions {
	FunctionInstructionsType type;
	FunctionInstructions *instructions;
	FunctionInstruction *instruction;
};

typedef enum {
	VARIABLE_DEFINITION,
	CONDITIONAL,
	FUNCTION_CALL,
	MEMBER_CALL,
	EMIT_EVENT,
	ASSIGNMENT,
	MATH_ASSIGNMENT,
	LOOP,
} FunctionInstructionType;

typedef struct {
	FunctionInstructionType type;
	VariableDefinition *variableDefinition;
	Conditional *conditional;
	FunctionCall *functionCall;
	MemberCall *memberCall;
	Arguments *eventArgs;
	char *eventIdentifier;
	Assignment *assignment;
	MathAssignment *mathAssignment;
	Loop *loop;
} FunctionInstruction;

typedef struct {
	LoopInitialization *loopInitialization;
	LoopCondition *loopCondition;
	LoopIteration *loopIteration;
	FunctionBlock *functionBlock;
} Loop;

typedef enum {
	VARIABLE_DEFINITION,
	ASSIGNMENT,
	MATH_ASSIGNMENT,
	EMPTY,
} LoopInitializationType;

typedef struct {
	LoopInitializationType type;
	VariableDefinition *variable;
	Assignment *assignment;
	MathAssignment *mathAssignment;
} LoopInitialization;

typedef enum {
	CONDITIONAL,
	EMPTY,
} LoopConditionType;

typedef struct {
	LoopConditionType type;
	Expression *condition;
} LoopCondition;

typedef enum {
	ASSIGNMENT,
	MATH_ASSIGNMENT,
	EMPTY,
} LoopIterationType;

typedef struct {
	LoopIterationType type;
	Assignment *assignment;
	MathAssignment *mathAssignment;
} LoopIteration;

typedef enum {
	VARIABLE,
	ARRAY,
} AssignableType;

typedef struct {
	AssignableType type;
	Expression *expression;
	char *identifier;
} Assignable;

typedef enum {
	EXPRESSION,
	ARRAY_INITIALIZATION,
	FUNCTION_CALL,
} AssignmentType;

typedef struct {
	AssignmentType type;
	Assignable *assignable;
	Expression *expression;
	Arguments *arrayIndex;
	FunctionCall *functionCall;
} Assignment;

typedef enum {
	OPERATOR,
	INCREMENT,
	DECREMENT,
} MathAssignmentType;

typedef struct {
	MathAssignmentType type;
	Assignable *assignable;
	MathAssignmentOperator *operator;
	Expression *expression;
} MathAssignment;

typedef enum {
	ADD_EQUAL,
	SUBTRACT_EQUAL,
	MULTIPLY_EQUAL,
	DIVIDE_EQUAL,
	MODULO_EQUAL,
} MathAssignmentOperatorType;

typedef struct {
	MathAssignmentOperatorType type;
} MathAssignmentOperator;

typedef enum {
	NO_ARGS,
	WITH_ARGS
} FunctionCallType;

typedef struct {
	FunctionCallType type;
	Arguments *arguments;
	char *identifier;
} FunctionCall;

typedef enum {
	MULTIPLE,
	SINGLE
} ArgumentsType;

struct Arguments{
	ArgumentsType type;
	Arguments *arguments;
	Expression *expression;
};

typedef struct {
	Assignable *assignable;
	FunctionCall *method;
} MemberCall;

typedef enum {
	NOT_INITIALIZED,
	INITIALIZED
} VariableDefinitionType;

typedef struct {
	VariableDefinitionType type;
	DataType *dataType;
	Assignment *assignment;
	char *identifier;
} VariableDefinition;

typedef enum {
	ERC20,
	ERC721,
	BYTES,
	STRING,
	BOOLEAN,
	ADDRESS,
	UINT,
	INT,
	DYNAMIC_ARRAY,
	STATIC_ARRAY
} DataTypeType;

struct DataType {
	DataTypeType type;
	DataType *dataType;
	Expression *expression;
};

typedef struct {
	Decorators *decorators;
	ParameterDefinition *parameterDefinition;
	FunctionBlock *functionBlock;
	char *identifier;
} FunctionDefinition;

typedef enum {
	WITH_DECORATORS,
	NO_DECORATORS
} DecoratorsType;

typedef enum {
	PUBLIC,
	PRIVATE,
	REENTRANT
} DecoratorsValue;

struct Decorators {
	DecoratorsType type;
	Decorators *decorators;
	DecoratorsValue decorator;
};

typedef enum {
	NO_PARAMS,
	WITH_PARAMS
} ParameterDefinitionType;

typedef struct {
	ParameterDefinitionType type;
	Parameters *parameters;
} ParameterDefinition;

typedef enum {
	MULTIPLE,
	SINGLE
} ParametersType;

struct Parameters {
	ParametersType type;
	Parameters *parameters;
	DataType *type;
	char *identifier;
};

typedef enum {
	ADDITION,
	SUBTRACTION,
	MULTIPLICATION,
	DIVISION,
	MODULO,
	EXPONENTIATION,
	EQUALITY,
	INEQUALITY,
	LESS_THAN,
	LESS_THAN_OR_EQUAL,
	GREATER_THAN,
	GREATER_THAN_OR_EQUAL,
	AND,
	OR,
	NOT,
	FACTOR
} ExpressionType;

struct Expression {
	ExpressionType type;
	Expression *leftExpression;
	Expression *rightExpression;
	Factor *factor;
};

typedef enum {
	EXPRESSION,
	CONSTANT
} FactorType;

typedef struct {
	FactorType type;
	Expression *expression;
	Constant *constant;
} Factor;

typedef enum {
	INTEGER,
	ASSIGNABLE,
	ADDRESS,
	BOOLEAN,
	STRING,
	SCIENTIFIC_NOTATION
} ConstantType;

typedef struct {
	ConstantType type;
	Assignable *assignable;
	int value;
	char *string;
} Constant;

#endif
