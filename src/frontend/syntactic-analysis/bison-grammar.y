%{

#include "bison-actions.h"

%}

// Tipos de dato utilizados en las variables semánticas ($$, $1, $2, etc.).
%union {
	// No-terminales
	Program *program;
	ContractDefinition *contract_definition;
	ContractBlock *contract_block;
	FunctionBlock *function_block;
	Conditional *conditional;
	ContractInstructions *contract_instructions;
	ContractInstruction *contract_instruction;
	FunctionInstructions *function_instructions;
	FunctionInstruction *function_instruction;
	Loop *loop;
	LoopInitialization *loop_initialization;
	LoopCondition *loop_condition;
	LoopIteration *loop_iteration;
	Assignable *assignable;
	Assignment *assignment;
	MathAssignment *math_assignment;
	MathAssignmentOperator *math_assignment_operator;
	FunctionCall *function_call;
	Arguments *arguments;
	MemberCall *member_call;
	VariableDefinition *variable_definition;
	DataType *data_type;
	FunctionDefinition *function_definition;
	Decorators *decorators;
	ParameterDefinition *parameter_definition;
	Parameters *parameters;
	Expression *expression;
	Factor *factor;
	Constant *constant;
	
	// Terminales
	token token;
	int integer;
	char *string;
}

// Un token que jamás debe ser usado en la gramática.
%token <token> ERROR

// Keywords
%token <token> CONTRACT
%token <token> FUNCTION
%token <token> EVENT
%token <token> EMIT

// IDs y tipos de los tokens terminales generados desde Flex.
%token <string> IDENTIFIER
%token <token> ADD
%token <token> SUB
%token <token> MUL
%token <token> DIV
%token <token> MOD
%token <token> EXP
%token <token> INC
%token <token> ADD_EQ
%token <token> DEC
%token <token> SUB_EQ
%token <token> MUL_EQ
%token <token> DIV_EQ
%token <token> MOD_EQ

// Comparadores
%token <token> IS_EQ
%token <token> NEQ
%token <token> LT
%token <token> LTE
%token <token> GT
%token <token> GTE

// Operadores logicos
%token <token> AND
%token <token> OR
%token <token> NOT

// Control de flujo
%token <token> IF
%token <token> ELSE
%token <token> FOR

%token <token> OPEN_CURLY_BRACKET
%token <token> CLOSE_CURLY_BRACKET
%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS
%token <token> OPEN_SQUARE_BRACKET
%token <token> CLOSE_SQUARE_BRACKET
%token <token> SEMI // semi-colon
%token <token> EQ
%token <token> COMMA
%token <token> DOT

// Tipos de dato
%token <token> T_ERC20
%token <token> T_ERC721
%token <token> T_BYTES
%token <token> T_STRING
%token <token> T_BOOLEAN
%token <token> T_ADDRESS
%token <token> T_UINT
%token <token> T_INT

%token <string> DECORATOR

// Constantes
%token <string> ADDRESS
%token <integer> BOOLEAN
%token <string> STRING
%token <integer> INTEGER
%token <string> SCIENTIFIC_NOTATION

// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program
%type <contract_definition> contract_definition
%type <contract_block> contract_block
%type <function_block> function_block
%type <conditional> conditional
%type <contract_instructions> contract_instructions
%type <function_instructions> function_instructions
%type <function_call> function_call
%type <member_call> member_call
%type <arguments> arguments
%type <contract_instruction> contract_instruction
%type <function_instruction> function_instruction
%type <variable_definition> variable_definition
%type <loop> loop
%type <loop_initialization> loop_initialization
%type <loop_condition> loop_condition
%type <loop_iteration> loop_iteration
%type <assignable> assignable
%type <assignment> assignment
%type <math_assignment> math_assignment
%type <math_assignment_operator> math_assignment_operator
%type <parameters> parameters
%type <data_type> data_type
%type <decorators> decorators
%type <function_definition> function_definition
%type <parameter_definition> parameter_definition
%type <expression> expression
%type <factor> factor
%type <constant> constant

// Reglas de asociatividad y precedencia (de menor a mayor).
%left IS_EQ NEQ LT LTE GT GTE
%left AND OR
%right NOT
%left ADD SUB // menor precedencia
%left MUL DIV
%left MOD
%right EXP	  // mayor precedencia

// El símbolo inicial de la gramatica.
%start program

%%

program: contract_definition													{ $$ = ProgramGrammarAction($1); }
	;

contract_definition: CONTRACT IDENTIFIER contract_block							{ $$ = ContractDefinitionGrammarAction($2, $3); }
	;

contract_block: OPEN_CURLY_BRACKET contract_instructions CLOSE_CURLY_BRACKET 	{ $$ = ContractBlockGrammarAction($2); }
	;

function_block: OPEN_CURLY_BRACKET function_instructions CLOSE_CURLY_BRACKET	{ $$ = FunctionBlockGrammarAction($2); }
	;

conditional: IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS function_block	{ $$ = ConditionalGrammarAction($3, $5, NULL); }
	| IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS function_block
		ELSE function_block														{ $$ = ConditionalGrammarAction($3, $5, $7); }
	;

contract_instructions: contract_instructions contract_instruction				{ $$ = ContractInstructionsGrammarAction($1, $2); } 
	| %empty																	{ $$ = ContractInstructionsGrammarAction(NULL, NULL); }
	;

contract_instruction: decorators variable_definition SEMI						{ $$ = StateVariableContractInstructionGrammarAction($1, $2); }
	| function_definition														{ $$ = FunctionDefinitionContractInstructionGrammarAction($1); }
	| EVENT IDENTIFIER parameter_definition SEMI								{ $$ = EventDefinitionContractInstructionGrammarAction($2, $3); }
	;

function_instructions: function_instructions function_instruction				{ $$ = FunctionInstructionsGrammarAction($1, $2); }
	| %empty																	{ $$ = FunctionInstructionsGrammarAction(NULL, NULL); }
	;

function_instruction: variable_definition SEMI									{ $$ = VariableDefinitionFunctionInstructionGrammarAction($1); }
	| conditional																{ $$ = ConditionalFunctionInstructionGrammarAction($1); }
	| function_call SEMI														{ $$ = FunctionCallFunctionInstructionGrammarAction($1); }
	| member_call SEMI															{ $$ = MemberCallFunctionInstructionGrammarAction($1); }
	| EMIT IDENTIFIER arguments SEMI											{ $$ = EmitEventFunctionInstructionGrammarAction($2, $3); }
	| assignment SEMI															{ $$ = AssignmentFunctionInstructionGrammarAction($1); }
	| math_assignment SEMI														{ $$ = MathAssignmentFunctionInstructionGrammarAction($1); }
	| loop																		{ $$ = LoopFunctionInstructionGrammarAction($1); }
	;

loop: FOR OPEN_PARENTHESIS loop_initialization SEMI loop_condition 
		SEMI loop_iteration CLOSE_PARENTHESIS function_block					{ $$ = LoopGrammarAction($3, $5, $7, $9); }
	;

loop_initialization: variable_definition										{ $$ = 0; }
	| assignment																{ $$ = 0; }
	| math_assignment															{ $$ = 0; }
	| %empty																	{ $$ = 0; }
	;

loop_condition: expression														{ $$ = 0; }
	| %empty																	{ $$ = 0; }
	;

loop_iteration: assignment														{ $$ = 0; }
	| math_assignment															{ $$ = 0; }
	| %empty																	{ $$ = 0; }
	;

assignable: IDENTIFIER															{ $$ = 0; }
	| IDENTIFIER OPEN_SQUARE_BRACKET expression CLOSE_SQUARE_BRACKET			{ $$ = 0; }

assignment: assignable EQ expression											{ $$ = 0; /*VariableDefinitionGrammarAction($1, $2, $4);*/ }
	| assignable EQ OPEN_SQUARE_BRACKET arguments CLOSE_SQUARE_BRACKET			{ $$ = 0; }
	| assignable EQ function_call												{ $$ = 0; }
	;

math_assignment: assignable math_assignment_operator expression					{ $$ = 0; }
	| assignable INC 															{ $$ = 0; }
	| assignable DEC															{ $$ = 0; }
	;

math_assignment_operator: ADD_EQ												{ $$ = 0; }
	| SUB_EQ																	{ $$ = 0; }
	| MUL_EQ																	{ $$ = 0; }
	| DIV_EQ																	{ $$ = 0; }
	| MOD_EQ																	{ $$ = 0; }
	;

function_call: IDENTIFIER OPEN_PARENTHESIS CLOSE_PARENTHESIS					{ $$ = 0; }
	| IDENTIFIER OPEN_PARENTHESIS arguments CLOSE_PARENTHESIS					{ $$ = 0; }
	;

arguments: arguments COMMA expression											{ $$ = 0; }
	| expression																{ $$ = 0; }
	;

member_call: assignable DOT function_call										{ $$ = 0; }
	;

variable_definition: data_type IDENTIFIER										{ $$ = 0; }
	| data_type assignment														{ $$ = 0; }
	;

data_type: T_ERC20																{ $$ = DataTypeSimpleGrammarAction(DATA_TYPE_ERC20); }
	| T_ERC721																	{ $$ = DataTypeSimpleGrammarAction(DATA_TYPE_ERC721); }
	| T_BYTES																	{ $$ = DataTypeSimpleGrammarAction(DATA_TYPE_BYTES); }
	| T_STRING																	{ $$ = DataTypeSimpleGrammarAction(DATA_TYPE_STRING); }
	| T_BOOLEAN																	{ $$ = DataTypeSimpleGrammarAction(DATA_TYPE_BOOLEAN); }
	| T_ADDRESS																	{ $$ = DataTypeSimpleGrammarAction(DATA_TYPE_ADDRESS); }
	| T_UINT																	{ $$ = DataTypeSimpleGrammarAction(DATA_TYPE_UINT); }
	| T_INT																		{ $$ = DataTypeSimpleGrammarAction(DATA_TYPE_INT); }
	| data_type OPEN_SQUARE_BRACKET CLOSE_SQUARE_BRACKET						{ $$ = DataTypeArrayGrammarAction($1, NULL); }
	| data_type OPEN_SQUARE_BRACKET expression CLOSE_SQUARE_BRACKET				{ $$ = DataTypeArrayGrammarAction($1, $3); }
	;

function_definition: decorators 
		FUNCTION IDENTIFIER parameter_definition function_block					{ $$ = FunctionDefinitionGrammarAction($3); }
	;

decorators: DECORATOR decorators												{ $$ = DecoratorGrammarAction($1); }
	| %empty																	{ $$ = 0; }
	;

parameter_definition: OPEN_PARENTHESIS CLOSE_PARENTHESIS						{ $$ = EmptyArgumentListGrammarAction(); }
	| OPEN_PARENTHESIS parameters CLOSE_PARENTHESIS								{ $$ = ArgumentListGrammarAction(); }
	;

parameters: parameters COMMA data_type IDENTIFIER								{ $$ = ArgumentDefinitionGrammarAction(); }
	| data_type IDENTIFIER														{ $$ = ArgumentDefinitionGrammarAction(); }
	;

expression: expression[left] ADD expression[right]								{ $$ = AdditionExpressionGrammarAction($left, $right); }
	| expression[left] SUB expression[right]									{ $$ = SubtractionExpressionGrammarAction($left, $right); }
	| expression[left] MUL expression[right]									{ $$ = MultiplicationExpressionGrammarAction($left, $right); }
	| expression[left] DIV expression[right]									{ $$ = DivisionExpressionGrammarAction($left, $right); }
	| expression[left] MOD expression[right]									{ $$ = ModuloExpressionGrammarAction($left, $right); }
	| expression[left] EXP expression[right]									{ $$ = ExponentiationExpressionGrammarAction($left, $right); }
	| expression[left] IS_EQ expression[right]									{ $$ = EqualityExpressionGrammarAction($left, $right); }
	| expression[left] NEQ expression[right]									{ $$ = InequalityExpressionGrammarAction($left, $right); }
	| expression[left] LT expression[right]										{ $$ = LessThanExpressionGrammarAction($left, $right); }
	| expression[left] LTE expression[right]									{ $$ = LessThanOrEqualExpressionGrammarAction($left, $right); }
	| expression[left] GT expression[right]										{ $$ = GreaterThanExpressionGrammarAction($left, $right); }
	| expression[left] GTE expression[right]									{ $$ = GreaterThanOrEqualExpressionGrammarAction($left, $right); }
	| expression[left] AND expression[right]									{ $$ = AndExpressionGrammarAction($left, $right); }
	| expression[left] OR expression[right]										{ $$ = OrExpressionGrammarAction($left, $right); }
	| NOT expression															{ $$ = NotExpressionGrammarAction($2); }
	| factor																	{ $$ = FactorExpressionGrammarAction($1); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS							{ $$ = ExpressionFactorGrammarAction($2); }
	| constant																	{ $$ = ConstantFactorGrammarAction($1); }
	;

constant: INTEGER																{ $$ = IntegerConstantGrammarAction(INTEGER, $1); }
	| assignable																{ $$ = AssignableConstantGrammarAction($1); }
	| ADDRESS																	{ $$ = StringConstantGrammarAction(ADDRESS, $1); }
	| BOOLEAN																	{ $$ = IntegerConstantGrammarAction(BOOLEAN, $1); }
	| STRING																	{ $$ = StringConstantGrammarAction(STRING, $1); }
	| SCIENTIFIC_NOTATION														{ $$ = StringConstantGrammarAction(SCIENTIFIC_NOTATION, $1); }
	;

%%
