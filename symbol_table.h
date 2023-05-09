#include "parser.h"
// ++++++++ Main Functions Declaration ++++++++++
// these functions are used mainly from the compiler

void printSymbolTable(SymbolTable *s);

/**
 * Create a new scope symbol table, insert it into grand symbol table
 * make the current table its child, then set it as current table
 * @param st Grand symbol table
 * @return The new symbol table it made
 */
_symbolTable *createScope(SymbolTable *st);

/**
 * Create a new scope symbol table, insert it into grand symbol table
 * make the current table its child, then set it as current table
 * @param st Grand symbol table
 * @return The new symbol table it made
 * TODO: to be completed soon
 */
_symbolTable *deleteScope(SymbolTable *st);

/**
 * Get the symbol with a specific name in the scope list till the parent
 * @param s Symbol table
 * @param name The symbol name
 * @returns The symbol entry, otherwise null
 */
Symbol *getSymbol(SymbolTable *s, char *name);

/**
 * Insert a new symbol in the current table
 * modify the symbol if it's already exist
 * @param s The new symbol
 * @param st The grand symbol table
 * @return The new symbol add or modified
 */
Symbol *insertSymbol(Symbol *s, SymbolTable *st);

// +++++++ Support Functions Declaration ++++++++++
// these functions are used from the main functions

void printSubSymbolTable(_symbolTable *symbol_table, FILE *f);

/**
 * This function searchs for a given entry name in
 * the current scope symbol table
 * @param st Current scope symbol table
 * @param name Name of the entry
 * @returns pointer to entry if exists, null otherwise
 */
Symbol *getSymbolFromSymbolTable(_symbolTable *st, char *name);