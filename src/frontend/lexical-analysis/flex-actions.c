#include <stdlib.h>
#include <string.h>

#include "../../backend/support/logger.h"
#include "flex-actions.h"

/**
 * Implementación de "flex-actions.h".
 *
 * Cada función debe realizar 2 operaciones básicas para lograr el streaming
 * de tokens hacia Bison:
 *
 *	1) Computar los atributos del token y almacenarlos donde sea conveniente
 *		(en la tabla de símbolos, en "yylval", o en ambos).
 *	2) Retornar el token que identifica el terminal identificado.
 *
 * Bison utilizará el token retornado en el punto (2) para matchear dicho
 * terminal en la gramática. Por otro lado, el valor almacenado en "yylval" es
 * el que Bison proveerá como valor semántico al realizar una reducción
 * (mediante $1, $2, $3, etc.).
 */

char * copyLexeme(const char * lexeme, const int length) {
	char * lexemeCopy = (char *) calloc(length + 1, sizeof(char));
	strncpy(lexemeCopy, lexeme, length);
	return lexemeCopy;
}

void BeginCommentPatternAction() {
	LogDebug("[Flex] [COMMENT] BeginCommentPatternAction............................");
}

void EndCommentPatternAction() {
	LogDebug("[Flex] [COMMENT] EndCommentPatternAction..............................");
}

token KeywordPatternAction(const char * lexeme, token token) {
	LogDebug("[Flex] KeywordPatternAction: '%s' with %d.\n", lexeme, token);
	yylval.token = token;
	return token;
}

token IdentifierPatternAction(const char * lexeme, const int length) {
	LogDebug("[Flex] IdentifierPatternAction: '%s' (length = %d).", lexeme, length);
	yylval.string = strndup(lexeme, length);
	return IDENTIFIER;
}

token AdditionOperatorPatternAction() {
	LogDebug("[Flex] AdditionOperatorPatternAction: '+'.");
	yylval.token = ADD;
	return ADD;
}

token OpenCurlyBracketPatternAction() {
	LogDebug("[Flex] OpenCurlyBracketPatternAction: '{'.");
	yylval.token = OPEN_CURLY_BRACKET;
	return OPEN_CURLY_BRACKET;
}

token CloseCurlyBracketPatternAction() {
	LogDebug("[Flex] CloseCurlyBracketPatternAction: '}'.");
	yylval.token = CLOSE_CURLY_BRACKET;
	return CLOSE_CURLY_BRACKET;
}
token OpenParenthesisPatternAction() {
	LogDebug("[Flex] OpenParenthesisPatternAction: '('.");
	yylval.token = OPEN_PARENTHESIS;
	return OPEN_PARENTHESIS;
}

token CloseParenthesisPatternAction() {
	LogDebug("[Flex] CloseParenthesisPatternAction: ')'.");
	yylval.token = CLOSE_PARENTHESIS;
	return CLOSE_PARENTHESIS;
}

token DivisionOperatorPatternAction() {
	LogDebug("[Flex] DivisionOperatorPatternAction: '/'.");
	yylval.token = DIV;
	return DIV;
}

token IntegerPatternAction(const char * lexeme, const int length) {
	LogDebug("[Flex] IntegerPatternAction: '%s' (length = %d).", lexeme, length);
	char * lexemeCopy = copyLexeme(lexeme, length);
	yylval.integer = atoi(lexemeCopy);
	free(lexemeCopy);
	return INTEGER;
}

token MultiplicationOperatorPatternAction() {
	LogDebug("[Flex] MultiplicationOperatorPatternAction: '*'.");
	yylval.token = MUL;
	return MUL;
}


token SubtractionOperatorPatternAction() {
	LogDebug("[Flex] SubtractionOperatorPatternAction: '-'.");
	yylval.token = SUB;
	return SUB;
}

token UnknownPatternAction(const char * lexeme, const int length) {
	char * lexemeCopy = copyLexeme(lexeme, length);
	LogDebug("[Flex] UnknownPatternAction: '%s' (length = %d).", lexemeCopy, length);
	free(lexemeCopy);
	yylval.token = ERROR;
	// Al emitir este token, el compilador aborta la ejecución.
	return ERROR;
}

void IgnoredPatternAction(const char * lexeme, const int length) {
	char * lexemeCopy = copyLexeme(lexeme, length);
	LogRaw("[DEBUG] [Flex] IgnoredPatternAction: '");
	LogText(lexemeCopy, length);
	LogRaw("' (length = %d).\n", length);
	free(lexemeCopy);
	// Como no debe hacer nada con el patrón, solo se loguea en consola.
	// No se emite ningún token.
}
