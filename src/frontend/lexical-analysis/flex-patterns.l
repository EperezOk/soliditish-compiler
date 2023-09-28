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
<LINE_COMMENT>\n							{ EndCommentPatternAction(); BEGIN(INITIAL); }

"contract"								    { return KeywordPatternAction(yytext, CONTRACT); }
"function"								    { return KeywordPatternAction(yytext, FUNCTION); }

"ERC20"                                     { return KeywordPatternAction(yytext, T_ERC20); }
"ERC721"                                    { return KeywordPatternAction(yytext, T_ERC721); }
"bytes"                                     { return KeywordPatternAction(yytext, T_BYTES); }
"string"                                    { return KeywordPatternAction(yytext, T_STRING); }
"bool"                                   { return KeywordPatternAction(yytext, T_BOOLEAN); }
"address"                                   { return KeywordPatternAction(yytext, T_ADDRESS); }
"uint"                                      { return KeywordPatternAction(yytext, T_UINT); }
"int"                                       { return KeywordPatternAction(yytext, T_INT); }

"+"											{ return AdditionOperatorPatternAction(); }
"-"											{ return SubtractionOperatorPatternAction(); }
"*"											{ return MultiplicationOperatorPatternAction(); }
"/"											{ return DivisionOperatorPatternAction(); }

"{"											{ return OpenCurlyBracketPatternAction(); }
"}"											{ return CloseCurlyBracketPatternAction(); }
"("											{ return OpenParenthesisPatternAction(); }
")"											{ return CloseParenthesisPatternAction(); }
"["											{ return OpenSquareBracketPatternAction(); }
"]"											{ return CloseSquareBracketPatternAction(); }
","                                         { return CommaPatternAction(); }

-?{digit}+									{ return IntegerPatternAction(yytext, yyleng); }

({character}|_)({character}|_|{digit})*	    { return StringValuePatternAction(yytext, yyleng, IDENTIFIER); }

{address}                                   { return StringValuePatternAction(yytext, yyleng, ADDRESS); }

{boolean}                                   { return StringValuePatternAction(yytext, yyleng, BOOLEAN); }

{string}                                    { return StringValuePatternAction(yytext, yyleng, STRING); }

"="                                         { return KeywordPatternAction(yytext, EQ); }

";"                                         { return KeywordPatternAction(yytext, EOL); }

{whitespace}+								{ IgnoredPatternAction(yytext, yyleng); }

.											{ return UnknownPatternAction(yytext, yyleng); }

%%
