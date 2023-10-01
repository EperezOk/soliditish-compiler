%{

#include "bison-actions.h"

%}

// Tipos de dato utilizados en las variables semánticas ($$, $1, $2, etc.).
%union {
	// No-terminales (backend).
	/*
	Program program;
	Expression expression;
	Factor factor;
	Constant constant;
	...
	*/

	// No-terminales (frontend).
	int program;
	int contract_definition;
	int contract_block;
	int function_block;
	int conditional;
	int contract_instructions;
	int function_instructions;
	int contract_instruction;
	int function_instruction;
	int function_call;
	int arguments;
	int variable_definition;
	int parameters;
	int data_type;
	int parameter_definition;
	int function_definition;
	int expression;
	int factor;
	int constant;

	// Terminales.
	token token;
	int integer;
	char *string;
}

// Un token que jamás debe ser usado en la gramática.
%token <token> ERROR

// Keywords
%token <token> CONTRACT
%token <token> FUNCTION

// IDs y tipos de los tokens terminales generados desde Flex.
%token <string> IDENTIFIER
%token <token> ADD
%token <token> SUB
%token <token> MUL
%token <token> DIV
%token <token> MOD
%token <token> EXP

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

%token <token> OPEN_CURLY_BRACKET
%token <token> CLOSE_CURLY_BRACKET
%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS
%token <token> OPEN_SQUARE_BRACKET
%token <token> CLOSE_SQUARE_BRACKET
%token <token> EOL
%token <token> EQ
%token <token> COMMA

// Tipos de dato
%token <token> T_ERC20
%token <token> T_ERC721
%token <token> T_BYTES
%token <token> T_STRING
%token <token> T_BOOLEAN
%token <token> T_ADDRESS
%token <token> T_UINT
%token <token> T_INT

// Constantes
%token <string> ADDRESS
%token <string> BOOLEAN
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
%type <arguments> arguments
%type <contract_instruction> contract_instruction
%type <function_instruction> function_instruction
%type <variable_definition> variable_definition
%type <parameters> parameters
%type <data_type> data_type
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

contract_definition: CONTRACT IDENTIFIER contract_block							{ $$ = ContractDefinitionGrammarAction($2); }
	;

contract_block: OPEN_CURLY_BRACKET contract_instructions CLOSE_CURLY_BRACKET 	{ $$ = BlockGrammarAction($2); }
	;

function_block: OPEN_CURLY_BRACKET function_instructions CLOSE_CURLY_BRACKET	{ $$ = BlockGrammarAction($2); }
	;

conditional: IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS function_block	{ $$ = 0; }
	| IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS function_block
		ELSE function_block														{ $$ = 0; }
	;

contract_instructions: contract_instructions contract_instruction				{ $$ = InstructionsGrammarAction($1, $2); } 
	|																			{ $$ = EmptyInstructionGrammarAction(); }
	;

contract_instruction: variable_definition EOL									{ $$ = 0; }
	| function_definition														{ $$ = FunctionInstructionGrammarAction(); }
	;

function_instructions: function_instructions function_instruction				{ $$ = InstructionsGrammarAction($1, $2); }
	|																			{ $$ = EmptyInstructionGrammarAction(); }
	;

function_instruction: variable_definition EOL									{ $$ = 0; }
	| conditional																{ $$ = 0; }
	| function_call EOL															{ $$ = 0; }
	;

function_call: IDENTIFIER OPEN_PARENTHESIS CLOSE_PARENTHESIS					{ $$ = 0; }
	| IDENTIFIER OPEN_PARENTHESIS arguments CLOSE_PARENTHESIS					{ $$ = 0; }
	;

arguments: arguments COMMA expression											{ $$ = 0; }
	| expression																{ $$ = 0; }
	;

variable_definition: data_type IDENTIFIER										{ $$ = 0; }
	| data_type IDENTIFIER EQ expression										{ $$ = 0; /*VariableDefinitionGrammarAction($1, $2, $4);*/ }
	/* | data_type IDENTIFIER EQ `new uint[](numerical_expression)`				{ $$ = 0; } */
	| data_type IDENTIFIER EQ function_call										{ $$ = 0; }
	;

data_type: T_ERC20																{ $$ = ERC20DefinitionGrammarAction(); }
	| T_ERC721																	{ $$ = 0; }
	| T_BYTES																	{ $$ = 0; }
	| T_STRING																	{ $$ = 0; }
	| T_BOOLEAN																	{ $$ = 0; }
	| T_ADDRESS																	{ $$ = 0; }
	| T_UINT																	{ $$ = 0; }
	| T_INT																		{ $$ = 0; }
	| data_type OPEN_SQUARE_BRACKET CLOSE_SQUARE_BRACKET						{ $$ = 0; }
	| data_type OPEN_SQUARE_BRACKET INTEGER CLOSE_SQUARE_BRACKET				{ $$ = 0; }
	;

function_definition: FUNCTION IDENTIFIER parameter_definition function_block		{ $$ = FunctionDefinitionGrammarAction($2); }
	;

parameter_definition: OPEN_PARENTHESIS CLOSE_PARENTHESIS							{ $$ = EmptyArgumentListGrammarAction(); }
	| OPEN_PARENTHESIS parameters CLOSE_PARENTHESIS								{ $$ = ArgumentListGrammarAction(); }
	;

parameters: parameters COMMA data_type IDENTIFIER									{ $$ = ArgumentDefinitionGrammarAction(); }
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

constant: INTEGER																{ $$ = IntegerConstantGrammarAction($1); }
	| IDENTIFIER																{ $$ = 0; }
	| ADDRESS																	{ $$ = 0; }
	| BOOLEAN																	{ $$ = 0; }
	| STRING																	{ $$ = 0; }
	| SCIENTIFIC_NOTATION														{ $$ = 0; }
	;

%%
