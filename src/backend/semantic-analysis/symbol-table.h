#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "uthash.h"

typedef enum {
	false = 0,
	true = 1
} boolean;

typedef struct SymbolTableEntry {
    char *identifier;  // hash key
    // Add more fields here if needed

    UT_hash_handle hh; // makes this structure hashable
} SymbolTableEntry;

boolean symbolExists(char *identifier);

void insertSymbol(char *identifier);

void freeSymbolTable();

#endif
