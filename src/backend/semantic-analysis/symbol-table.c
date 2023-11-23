#include <stdlib.h>
#include "./symbol-table.h"

boolean symbolExists(char *identifier) {
    boolean res = false;
    SymbolTableEntry *symbol;

    HASH_FIND_STR(state.symbolTable, identifier, symbol);

    if (symbol != NULL) res = true;

    return res;
}

void insertSymbol(char *identifier, DataTypeType type, SymbolType symbolType) {
    SymbolTableEntry *symbol;

    if (!symbolExists(identifier)) {
        symbol = malloc(sizeof(SymbolTableEntry));
        symbol->identifier = identifier;
        symbol->type = type;
        symbol->symbolType = symbolType;
        HASH_ADD_STR(state.symbolTable, identifier, symbol);
        
		if (type == DATA_TYPE_ERC20) state.libraries[LIBRARY_ERC20] = true;
		if (type == DATA_TYPE_ERC721) state.libraries[LIBRARY_ERC721] = true;
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

DataTypeType getSymbolDataType(char *identifier) {
    SymbolTableEntry *symbol;
    HASH_FIND_STR(state.symbolTable, identifier, symbol);
    if (symbol != NULL) {
        return symbol->type;
    }
    else {
        return -1;
    }
}

SymbolType getSymbolType(char *identifier) {
    SymbolTableEntry *symbol;
    HASH_FIND_STR(state.symbolTable, identifier, symbol);
    if (symbol != NULL) {
        return symbol->symbolType;
    }
    else {
        return -1;
    }
}
