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
%x COMMENT

/* Patrones reutilizables. */
digit			[0-9]
character       [a-zA-Z]
endline			[\n]
whitespace		[ \f\n\r\t\v]

%%

"/*"										{ BEGIN(COMMENT); BeginCommentPatternAction(); }
<COMMENT>.									{ IgnoredPatternAction(yytext, yyleng); }
<COMMENT>{endline}+							{ IgnoredPatternAction(yytext, yyleng); }
<COMMENT>"*/"								{ EndCommentPatternAction(); BEGIN(INITIAL); }

"contract"								    { return KeywordPatternAction(yytext, CONTRACT); }

"+"											{ return AdditionOperatorPatternAction(); }
"-"											{ return SubtractionOperatorPatternAction(); }
"*"											{ return MultiplicationOperatorPatternAction(); }
"/"											{ return DivisionOperatorPatternAction(); }

"{"											{ return OpenCurlyBracketPatternAction(); }
"}"											{ return CloseCurlyBracketPatternAction(); }
"("											{ return OpenParenthesisPatternAction(); }
")"											{ return CloseParenthesisPatternAction(); }

{digit}+									{ return IntegerPatternAction(yytext, yyleng); }

({character}|_)({character}|_|{digit})*	            { return IdentifierPatternAction(yytext, yyleng); }

{whitespace}+								{ IgnoredPatternAction(yytext, yyleng); }

.											{ return UnknownPatternAction(yytext, yyleng); }

%%
