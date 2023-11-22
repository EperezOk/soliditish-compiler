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

"contract"								    { return CONTRACT; }
"function"								    { return FUNCTION; }

"event"                                     { return EVENT; }
"emit"                                      { return EMIT; }

"if"                                        { return IF; }
"else"                                      { return ELSE; }
"for"                                       { return FOR; }

"ERC20"                                     { return TokenPatternAction(yytext, T_ERC20); }
"ERC721"                                    { return TokenPatternAction(yytext, T_ERC721); }
"bytes"                                     { return TokenPatternAction(yytext, T_BYTES); }
"string"                                    { return TokenPatternAction(yytext, T_STRING); }
"bool"                                      { return TokenPatternAction(yytext, T_BOOLEAN); }
"address"                                   { return TokenPatternAction(yytext, T_ADDRESS); }
"uint"                                      { return TokenPatternAction(yytext, T_UINT); }
"int"                                       { return TokenPatternAction(yytext, T_INT); }

"+"											{ return ADD; }
"-"											{ return SUB; }
"*"											{ return MUL; }
"/"											{ return DIV; }
"%"											{ return MOD; }
"**"										{ return EXP; }

"=="										{ return EQ_EQ; }
"!="										{ return NEQ }
"<"											{ return LT }
"<="										{ return LTE }
">"											{ return GT }
">="										{ return GTE }

"++"                                        { return INC; }
"--"                                        { return DEC; }
"+="                                        { return ADD_EQ; }
"-="                                        { return SUB_EQ; }
"*="                                        { return MUL_EQ; }
"/="                                        { return DIV_EQ; }
"%="                                        { return MOD_EQ; }

"&&"										{ return AND; }
"||"										{ return OR; }
"!"											{ return NOT; }

"{"											{ return OPEN_CURLY_BRACKET; }
"}"											{ return CLOSE_CURLY_BRACKET; }
"("											{ return OPEN_PARENTHESIS; }
")"											{ return CLOSE_PARENTHESIS; }
"["											{ return OPEN_SQUARE_BRACKET; }
"]"											{ return CLOSE_SQUARE_BRACKET; }
","                                         { return COMMA; }
"."                                         { return DOT; }

@{character}+                               { return DecoratorPatternAction(yytext, yyleng); }

-?{digit}*\.?{digit}+[eE]-?{digit}+         { return StringValuePatternAction(yytext, yyleng, SCIENTIFIC_NOTATION); }
-?{digit}+									{ return IntegerPatternAction(yytext, yyleng); }

{boolean}                                   { return StringValuePatternAction(yytext, yyleng, BOOLEAN); }

({character}|_)({character}|_|{digit})*	    { return StringValuePatternAction(yytext, yyleng, IDENTIFIER); }

{address}                                   { return StringValuePatternAction(yytext, yyleng, ADDRESS); }

{string}                                    { return StringValuePatternAction(yytext, yyleng, STRING); }

"="                                         { return EQ; }

";"                                         { return SEMI; }

{whitespace}+								{ IgnoredPatternAction(yytext, yyleng); }

.											{ return UnknownPatternAction(yytext, yyleng); }

%%
