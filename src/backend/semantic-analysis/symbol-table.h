#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "uthash.h"

typedef enum {
	false = 0,
	true = 1
} boolean;

typedef enum {
    SYMBOL_UINT,
    SYMBOL_INT,
    SYMBOL_ADDRESS,
    SYMBOL_STRING,
    SYMBOL_BYTES,
    SYMBOL_BOOLEAN,
    SYMBOL_ERC20,
    SYMBOL_ERC721,
    SYMBOL_ARRAY_UINT,
    SYMBOL_ARRAY_INT,
    SYMBOL_ARRAY_ADDRESS,
    SYMBOL_ARRAY_STRING,
    SYMBOL_ARRAY_BYTES,
    SYMBOL_ARRAY_BOOLEAN,
    SYMBOL_ARRAY_ERC20,
    SYMBOL_ARRAY_ERC721
} VariableType;

typedef struct SymbolTableEntry {
    char *identifier;
    VariableType type;
    UT_hash_handle hh; // makes this structure hashable
} SymbolTableEntry;

boolean symbolExists(char *identifier);

void insertSymbol(char *identifier, VariableType type);

void freeSymbolTable(SymbolTableEntry *table);

#endif
