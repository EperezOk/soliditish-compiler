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
	int contract_instructions;
	int function_instructions;
	int contract_instruction;
	int function_instruction;
	int arguments;
	int data_type;
	int argument_definition;
	int function_definition;
	int expression;
	int factor;
	int constant;
	const char *initialization;

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
%token <token> ERC20
%token <token> EOL
%token <token> EQ

// IDs y tipos de los tokens terminales generados desde Flex.
%token <token> ADD
%token <token> SUB
%token <token> MUL
%token <token> DIV

%token <token> OPEN_CURLY_BRACKET
%token <token> CLOSE_CURLY_BRACKET
%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS
%token <token> COMMA

// Tipos de dato
%token <string> IDENTIFIER
%token <string> ADDRESS
%token <integer> INTEGER

// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program
%type <contract_definition> contract_definition
%type <contract_block> contract_block
%type <function_block> function_block
%type <contract_instructions> contract_instructions
%type <function_instructions> function_instructions
%type <contract_instruction> contract_instruction
%type <function_instruction> function_instruction
%type <arguments> arguments
%type <data_type> data_type
%type <function_definition> function_definition
%type <argument_definition> argument_definition
%type <initialization> initialization
%type <expression> expression
%type <factor> factor
%type <constant> constant

// Reglas de asociatividad y precedencia (de menor a mayor).
%left ADD SUB
%left MUL DIV

// El símbolo inicial de la gramatica.
%start program

%%

program: contract_definition													{ $$ = ProgramGrammarAction($1); }
	;

contract_definition: CONTRACT IDENTIFIER contract_block							{ $$ = ContractDefinitionGrammarAction($2); }
	;

contract_block: OPEN_CURLY_BRACKET contract_instructions CLOSE_CURLY_BRACKET 	{ $$ = BlockGrammarAction($2);}
	;

function_block: OPEN_CURLY_BRACKET function_instructions CLOSE_CURLY_BRACKET	{ $$ = BlockGrammarAction($2); }
	;

contract_instructions: contract_instructions contract_instruction				{ $$ = InstructionsGrammarAction($1, $2); } 
	|																			{ $$ = EmptyInstructionGrammarAction(); }
	;

contract_instruction: data_type IDENTIFIER initialization EOL					{ $$ = VariableDefinitionGrammarAction($1, $2, $3); }
	| function_definition														{ $$ = FunctionInstructionGrammarAction(); }
	;

function_instructions: function_instructions function_instruction				{ $$ = InstructionsGrammarAction($1, $2); }
	|																			{ $$ = EmptyInstructionGrammarAction(); }
	;

function_instruction: data_type IDENTIFIER initialization EOL					{ $$ = VariableDefinitionGrammarAction($1, $2, $3); }
	;

data_type: ERC20																{ $$ = ERC20DefinitionGrammarAction(); }
	;

initialization: EQ IDENTIFIER													{ $$ = InitializationGrammarAction($2); }
	| EQ ADDRESS																{ $$ = InitializationGrammarAction($2);}
	|																			{ $$ = EmptyInitializationGrammarAction();}
	;

function_definition: FUNCTION IDENTIFIER argument_definition function_block		{ $$ = FunctionDefinitionGrammarAction($2);}
	;

argument_definition: OPEN_PARENTHESIS CLOSE_PARENTHESIS							{ $$ = EmptyArgumentListGrammarAction();}
	| OPEN_PARENTHESIS arguments CLOSE_PARENTHESIS								{ $$ = ArgumentListGrammarAction();}
	;

arguments: arguments COMMA data_type IDENTIFIER									{ $$ = ArgumentDefinitionGrammarAction();}
	| data_type IDENTIFIER														{ $$ = ArgumentDefinitionGrammarAction();}
	;

expression: expression[left] ADD expression[right]								{ $$ = AdditionExpressionGrammarAction($left, $right); }
	| expression[left] SUB expression[right]									{ $$ = SubtractionExpressionGrammarAction($left, $right); }
	| expression[left] MUL expression[right]									{ $$ = MultiplicationExpressionGrammarAction($left, $right); }
	| expression[left] DIV expression[right]									{ $$ = DivisionExpressionGrammarAction($left, $right); }
	| factor																	{ $$ = FactorExpressionGrammarAction($1); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS							{ $$ = ExpressionFactorGrammarAction($2); }
	| constant																	{ $$ = ConstantFactorGrammarAction($1); }
	;

constant: INTEGER																{ $$ = IntegerConstantGrammarAction($1); }
	;

%%
