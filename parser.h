// WHEN UPDATING THIS FILE, EXECUTE: make clean
#include <stdio.h>

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

typedef struct functionOpr
{
    VarNameList *paramNames;
    struct nodeTypeTag *operations;

} functionOpr;

typedef enum
{
    typeVar,
    typeFunc,
    typeConst,
    typeEnum,
    typeParam
} symbolType;

typedef struct Symbol
{
    char *name;
    symbolType statement_type;

    // in case of function symbol
    // data type will be the return type
    datatypeEnum data_type;

    // in case of function symbol
    // it'll be the number of arguments
    int num_args;

    int is_used;
    int declaration_line;
    struct Symbol *next;

} Symbol;

typedef struct
{
    struct Symbol *head;
    // parent pointer
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
