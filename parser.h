// WHEN UPDATING THIS FILE, EXECUTE: make clean
#include <stdio.h>

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

// ++++++++ DATA TYPE +++++++++
typedef enum
{
    typeInt,
    typeStr,
    typeFloat,
    typeBool
} datatypeEnum;

// general type
typedef struct
{
    datatypeEnum type;
    union
    {
        int value; /* value of constant */
        char *str;
        float fvalue;
        // int bvalue; // TODO should this be int or char?, can we just use value?
    };
} Object;

// ++++++++ OPERATIONS +++++++++
/* operators */
typedef struct
{
    int oper;                  /* operator */
    int nops;                  /* number of operands */
    struct nodeTypeTag *op[1]; /* operands, extended at runtime */
} oprNodeType;

// ++++++++ VARIABLE NAME LIST +++++++++

typedef struct VarName
{
    char *name;
    struct VarName *next;
} VarName;

typedef struct VarNameList
{
    struct VarName *head;
} VarNameList;

// ++++++++ EXPRESSIONS LIST +++++++++

typedef struct Expr
{
    struct nodeTypeTag *operations;
    struct Expr *next;
} Expr;

typedef struct ExprList
{
    struct Expr *head;
} ExprList;

// ++++++++ NODES (OPERATIONS, CONSTANTS, IDENTIFIERS) +++++++++

typedef struct
{
    char *varname;
} idNodeType;

// typeVal is for constants, typeId is for variables, typeOpr is for operators
typedef enum
{
    typeVal,
    typeId,
    typeOpr,
    typeVarNameList,
    typeExprList
} nodeEnum;

typedef struct nodeTypeTag
{
    nodeEnum type; /* type of node */

    union
    {
        Object val;                      /* constants */
        idNodeType id; /* identifiers */ // TODO can this be pointer to varNameList?
        oprNodeType opr;                 /* operators */
        // pointer to varNameList, to create it:
        VarNameList *varNameList;
    };
} nodeType;

// ++++++++ SYMBOL TABLE +++++++++
// declare nodeTypeTag here so that it can be used in Symbol
typedef enum
{
    typeVar,
    typeFunc,
    typeConst,
    typeEnum
} symbolType;

typedef struct Symbol
{
    char *name;
    symbolType type;
    union
    {
        Object value;
        // parameters
        char *paramNames;
        Object *params;
        struct nodeTypeTag *operations;
    };
    struct Symbol *next;
    int declaration_line;

} Symbol;

typedef struct
{
    struct Symbol *head;
    struct _symbolTable *next;
    int id;

} _symbolTable;

typedef struct
{
    struct _symbolTable *currentTable;

} SymbolTable;

// ++++++++ GLOBAL VARIABLES +++++++++
extern int sym[26];
extern SymbolTable *symbolTable;
