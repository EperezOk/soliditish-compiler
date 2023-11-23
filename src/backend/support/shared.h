#ifndef SHARED_HEADER
#define SHARED_HEADER

#include <stdio.h>
#include "../semantic-analysis/uthash.h"

#include "../semantic-analysis/abstract-syntax-tree.h"


#define MAX_ERRORS 100
#define MAX_IMPORTS 20

// Descriptor del archivo de entrada que utiliza Bison.
extern FILE * yyin;

// Descriptor del archivo de salida que utiliza Bison.
extern FILE * yyout;

// La cantidad de caracteres en el lexema "yytext". Se debe leer en las
// acciones de Flex, no en Bison.
extern int yyleng;

// Variable global que contiene el número de la línea analizada.
extern int yylineno;

// Token actual en el tope de la pila del analizador Bison.
extern char * yytext;

// Función global de manejo de errores en Bison.
extern void yyerror(const char * string);

// Función global del analizador léxico Flex.
extern int yylex(void);

// Función global del analizador sintáctico Bison.
extern int yyparse(void);

// El tipo de los tokens emitidos por Flex.
typedef int token;

typedef enum {
	false = 0,
	true = 1
} boolean;

typedef enum {
	SYMBOL_TYPE_VARIABLE,
	SYMBOL_TYPE_ARRAY,
	SYMBOL_TYPE_FUNCTION,
	SYMBOL_TYPE_EVENT
} SymbolType;

typedef struct SymbolTableEntry {
    char *identifier;  // hash key
    // Add more fields here if needed
    DataTypeType type;
	SymbolType symbolType;
    UT_hash_handle hh; // makes this structure hashable
} SymbolTableEntry;

typedef enum {
	LIBRARY_ERC20,
	LIBRARY_ERC721,
	LIBRARY_CLONES,
	LIBRARY_CONSOLE
} Library;

// Estado global de toda la aplicación.
typedef struct {
	// Indica si la compilación tuvo problemas hasta el momento.
	boolean succeed;

	// El nodo raíz del AST (se usará cuando se implemente el backend).
	Program *program;

	// Agregar lo que sea necesario para el compilador.
	// Agregar una pila para manipular scopes.
	// Agregar una tabla de símbolos.
	// ...
	SymbolTableEntry *symbolTable;

	// Agregar un array para guardar los errores.
	char *errors[MAX_ERRORS];
	int errorCount;

	boolean libraries[MAX_IMPORTS];

	// Agregar una pila para manipular scopes.
} CompilerState;

// El estado se define e inicializa en el archivo "main.c".
extern CompilerState state;

#endif
