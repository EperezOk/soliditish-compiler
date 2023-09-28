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

"ERC20"                                     { return KeywordPatternAction(yytext, ERC20); }

"+"											{ return AdditionOperatorPatternAction(); }
"-"											{ return SubtractionOperatorPatternAction(); }
"*"											{ return MultiplicationOperatorPatternAction(); }
"/"											{ return DivisionOperatorPatternAction(); }

"{"											{ return OpenCurlyBracketPatternAction(); }
"}"											{ return CloseCurlyBracketPatternAction(); }
"("											{ return OpenParenthesisPatternAction(); }
")"											{ return CloseParenthesisPatternAction(); }
","                                         { return CommaPatternAction(); }

{digit}+									{ return IntegerPatternAction(yytext, yyleng); }

({character}|_)({character}|_|{digit})*	    { return StringValuePatternAction(yytext, yyleng); }

{address}                                   { return StringValuePatternAction(yytext, yyleng); }

"="                                         { return KeywordPatternAction(yytext, EQ); }

";"                                         { return KeywordPatternAction(yytext, EOL); }

{whitespace}+								{ IgnoredPatternAction(yytext, yyleng); }

.											{ return UnknownPatternAction(yytext, yyleng); }

%%
