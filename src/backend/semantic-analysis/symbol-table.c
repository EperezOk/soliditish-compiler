#include <stdlib.h>
#include "./symbol-table.h"

boolean symbolExists(char *identifier) {
    boolean res = false;
    SymbolTableEntry *symbol;

    HASH_FIND_STR(state.symbolTable, identifier, symbol);

    if (symbol != NULL) res = true;

    return res;
}

void insertSymbol(char *identifier, DataTypeType type) {
    SymbolTableEntry *symbol;

    if (!symbolExists(identifier)) {
        symbol = malloc(sizeof(SymbolTableEntry));
        symbol->identifier = identifier;
        symbol->type = type;
        HASH_ADD_STR(state.symbolTable, identifier, symbol);
    }
}

void removeSymbol(char *identifier) {
    SymbolTableEntry *symbol;

    HASH_FIND_STR(state.symbolTable, identifier, symbol);

    if (symbol != NULL) {
        HASH_DEL(state.symbolTable, symbol);
        free(symbol);
    }
}

void freeSymbolTable() {
    SymbolTableEntry *symbol, *tmp;

    HASH_ITER(hh, state.symbolTable, symbol, tmp) {
        HASH_DEL(state.symbolTable, symbol);
        free(symbol);
    }
}
