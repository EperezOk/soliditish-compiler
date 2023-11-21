#include <stdio.h>

#include "backend/code-generation/generator.h"
#include "backend/support/logger.h"
#include "backend/support/shared.h"
#include "frontend/syntactic-analysis/bison-parser.h"

// Estado de la aplicación.
CompilerState state;

// Punto de entrada principal del compilador.
const int main(const int argumentCount, const char ** arguments) {
	// Inicializar estado de la aplicación.
	state.program = NULL;
	state.succeed = false;
	state.symbolTable = NULL;

	// Mostrar parámetros recibidos por consola.
	for (int i = 0; i < argumentCount; ++i) {
		LogInfo("Argument %d: '%s'", i, arguments[i]);
	}

	// Compilar el programa de entrada.
	LogInfo("Compiling...\n");
	const int result = yyparse(); // lee de stdin y escribe en stdout
	switch (result) {
		case 0:
			// La variable "succeed" es la que setea Bison al identificar el símbolo
			// inicial de la gramática satisfactoriamente.
			if (state.succeed) {
				LogInfo("Compilation successful.");
				Generator(1);
			}
			else {
				LogError("Found %d compilation errors:", state.errorCount);
				freeSymbolTable();
				for (int i = 0; i < state.errorCount; ++i) {
					LogError("%s", state.errors[i]);
					free(state.errors[i]);
				}
				return -1;
			}
			break;
		case 1:
			LogError("[Bison] Syntax error.");
			break;
		case 2:
			LogError("[Bison] Out of memory.");
			break;
		default:
			LogError("[Bison] Unknown error (code %d).", result);
	}

	LogInfo("Freeing up memory...");
	freeSymbolTable();
	
	return result;
}
