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
	int block;
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

// IDs y tipos de los tokens terminales generados desde Flex.
%token <token> ADD
%token <token> SUB
%token <token> MUL
%token <token> DIV

%token <token> OPEN_CURLY_BRACKET
%token <token> CLOSE_CURLY_BRACKET
%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS

// Tipos de dato
%token <string> IDENTIFIER
%token <integer> INTEGER

// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program
%type <contract_definition> contract_definition
%type <block> block
%type <expression> expression
%type <factor> factor
%type <constant> constant

// Reglas de asociatividad y precedencia (de menor a mayor).
%left ADD SUB
%left MUL DIV

// El símbolo inicial de la gramatica.
%start program

%%

program: expression													{ $$ = ProgramGrammarAction($1); }
	| contract_definition											{ $$ = ProgramGrammarAction($1); }
	;

contract_definition: CONTRACT IDENTIFIER block						{ $$ = ContractDefinitionGrammarAction($2); }
	;

block: OPEN_CURLY_BRACKET CLOSE_CURLY_BRACKET						{ $$ = BlockGrammarAction(0); }
	| OPEN_CURLY_BRACKET expression CLOSE_CURLY_BRACKET				{ $$ = BlockGrammarAction($2); }
	;

expression: expression[left] ADD expression[right]					{ $$ = AdditionExpressionGrammarAction($left, $right); }
	| expression[left] SUB expression[right]						{ $$ = SubtractionExpressionGrammarAction($left, $right); }
	| expression[left] MUL expression[right]						{ $$ = MultiplicationExpressionGrammarAction($left, $right); }
	| expression[left] DIV expression[right]						{ $$ = DivisionExpressionGrammarAction($left, $right); }
	| factor														{ $$ = FactorExpressionGrammarAction($1); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS				{ $$ = ExpressionFactorGrammarAction($2); }
	| constant														{ $$ = ConstantFactorGrammarAction($1); }
	;

constant: INTEGER													{ $$ = IntegerConstantGrammarAction($1); }
	;

%%
