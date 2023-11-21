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

void BeginCommentPatternAction() {}

void EndCommentPatternAction() {}

token TokenPatternAction(const char *lexeme, token token) {
	yylval.token = token;
	return token;
}

token StringValuePatternAction(const char *lexeme, const int length, token token) {
	yylval.string = strndup(lexeme, length);
	return token;
}

token BooleanPatternAction(const char *lexeme, const int length) {
	char *lexemeCopy = strndup(lexeme, length);
	yylval.integer = strncmp(lexemeCopy, "true", length) == 0; // 1 = true, 0 = false
	free(lexemeCopy);
	return BOOLEAN;
}

token IntegerPatternAction(const char *lexeme, const int length) {
	char *lexemeCopy = strndup(lexeme, length);
	yylval.integer = atoi(lexemeCopy);
	free(lexemeCopy);
	return INTEGER;
}

token UnknownPatternAction(const char *lexeme, const int length) {
	char *lexemeCopy = strndup(lexeme, length);
	free(lexemeCopy);
	yylval.token = ERROR;
	// Al emitir este token, el compilador aborta la ejecución.
	return ERROR;
}

void IgnoredPatternAction(const char *lexeme, const int length) {
	char *lexemeCopy = strndup(lexeme, length);
	free(lexemeCopy);
	// Como no debe hacer nada con el patrón, solo se loguea en consola.
	// No se emite ningún token.
}
