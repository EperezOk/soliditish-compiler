%{

#include "flex-actions.h"

%}

/* Obliga a crear una regla explícita para manejar lexemas desconocidos. */
%option nodefault

/* Escanea un único archivo y se detiene. */
%option noyywrap

/* Dispone una variable global "yylineno" con el número de línea actual. */
%option yylineno

/* Contextos de Flex. */
%x BLOCK_COMMENT
%x LINE_COMMENT

/* Patrones reutilizables. */
digit			[0-9]
character       [a-zA-Z]
endline			[\n]
whitespace		[ \f\n\r\t\v]
address         0x[a-fA-F0-9]{40}
boolean         (true|false)
string          \"[^"]*\"

%%

"/*"										{ BEGIN(BLOCK_COMMENT); BeginCommentPatternAction(); }
<BLOCK_COMMENT>.						    { IgnoredPatternAction(yytext, yyleng); }
<BLOCK_COMMENT>{endline}+					{ IgnoredPatternAction(yytext, yyleng); }
<BLOCK_COMMENT>"*/"							{ EndCommentPatternAction(); BEGIN(INITIAL); }

"//"										{ BEGIN(LINE_COMMENT); BeginCommentPatternAction(); }
<LINE_COMMENT>.							    { IgnoredPatternAction(yytext, yyleng); }
<LINE_COMMENT>{endline}					    { EndCommentPatternAction(); BEGIN(INITIAL); }

"contract"								    { return TokenPatternAction(yytext, CONTRACT); }
"function"								    { return TokenPatternAction(yytext, FUNCTION); }

"ERC20"                                     { return TokenPatternAction(yytext, T_ERC20); }
"ERC721"                                    { return TokenPatternAction(yytext, T_ERC721); }
"bytes"                                     { return TokenPatternAction(yytext, T_BYTES); }
"string"                                    { return TokenPatternAction(yytext, T_STRING); }
"bool"                                      { return TokenPatternAction(yytext, T_BOOLEAN); }
"address"                                   { return TokenPatternAction(yytext, T_ADDRESS); }
"uint"                                      { return TokenPatternAction(yytext, T_UINT); }
"int"                                       { return TokenPatternAction(yytext, T_INT); }

"+"											{ return TokenPatternAction(yytext, ADD); }
"-"											{ return TokenPatternAction(yytext, SUB); }
"*"											{ return TokenPatternAction(yytext, MUL); }
"/"											{ return TokenPatternAction(yytext, DIV); }
"%"											{ return TokenPatternAction(yytext, MOD); }
"**"										{ return TokenPatternAction(yytext, EXP); }

"=="										{ return TokenPatternAction(yytext, IS_EQ); }
"!="										{ return TokenPatternAction(yytext, NEQ); }
"<"											{ return TokenPatternAction(yytext, LT); }
"<="										{ return TokenPatternAction(yytext, LTE); }
">"											{ return TokenPatternAction(yytext, GT); }
">="										{ return TokenPatternAction(yytext, GTE); }

"&&"										{ return TokenPatternAction(yytext, AND); }
"||"										{ return TokenPatternAction(yytext, OR); }
"!"											{ return TokenPatternAction(yytext, NOT); }

"{"											{ return TokenPatternAction(yytext, OPEN_CURLY_BRACKET); }
"}"											{ return TokenPatternAction(yytext, CLOSE_CURLY_BRACKET); }
"("											{ return TokenPatternAction(yytext, OPEN_PARENTHESIS); }
")"											{ return TokenPatternAction(yytext, CLOSE_PARENTHESIS); }
"["											{ return TokenPatternAction(yytext, OPEN_SQUARE_BRACKET); }
"]"											{ return TokenPatternAction(yytext, CLOSE_SQUARE_BRACKET); }
","                                         { return TokenPatternAction(yytext, COMMA); }

-?{digit}+									{ return IntegerPatternAction(yytext, yyleng); }

{boolean}                                   { return StringValuePatternAction(yytext, yyleng, BOOLEAN); }

({character}|_)({character}|_|{digit})*	    { return StringValuePatternAction(yytext, yyleng, IDENTIFIER); }

{address}                                   { return StringValuePatternAction(yytext, yyleng, ADDRESS); }

{string}                                    { return StringValuePatternAction(yytext, yyleng, STRING); }

"="                                         { return TokenPatternAction(yytext, EQ); }

";"                                         { return TokenPatternAction(yytext, EOL); }

{whitespace}+								{ IgnoredPatternAction(yytext, yyleng); }

.											{ return UnknownPatternAction(yytext, yyleng); }

%%
