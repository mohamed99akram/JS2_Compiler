// WHEN UPDATING THIS FILE, EXECUTE: make clean

// ++++++++ DATA TYPE +++++++++
typedef enum {typeInt, typeStr} datatypeEnum;

// general type
typedef struct {
    datatypeEnum type;
    union {
        int value;              /* value of constant */
        char *str;
    };
} Object;

// ++++++++ OPERATIONS +++++++++
/* operators */
typedef struct {
    int oper;                   /* operator */
    int nops;                   /* number of operands */
    struct nodeTypeTag *op[1];	/* operands, extended at runtime */
} oprNodeType;


// ++++++++ SYMBOL TABLE +++++++++
// declare nodeTypeTag here so that it can be used in Symbol
typedef enum { typeVar, typeFunc, typeConst, typeEnum } symbolType;
typedef struct Symbol {
    char *name;
    symbolType type;
    union {
        Object value;
        // parameters
        char* paramNames;
        Object* params;
        struct nodeTypeTag *operations;
    };
    struct Symbol *next; // for linked list
} Symbol;

typedef struct SymbolTable {
    struct Symbol *head;
} SymbolTable;


// ++++++++ VARIABLE NAME LIST +++++++++

typedef struct VarName {
    char *name;
    struct VarName *next;
} VarName;

typedef struct VarNameList {
    struct VarName *head;
} VarNameList;

// ++++++++ EXPRESSIONS LIST +++++++++

typedef struct Expr {
    struct nodeTypeTag *operations;
    struct Expr *next;
} Expr;

typedef struct ExprList {
    struct Expr *head;
} ExprList;

// ++++++++ NODES (OPERATIONS, CONSTANTS, IDENTIFIERS) +++++++++

typedef struct {
    char *varname;
} idNodeType;

// typeVal is for constants, typeId is for variables, typeOpr is for operators
typedef enum { 
    typeVal, 
    typeId,
    typeOpr,
    typeVarNameList,
    typeExprList
} nodeEnum; 

typedef struct nodeTypeTag {
    nodeEnum type;              /* type of node */

    union {
        Object val;             /* constants */
        idNodeType id;          /* identifiers */ // TODO can this be pointer to varNameList?
        oprNodeType opr;        /* operators */
        // pointer to varNameList, to create it: 
        VarNameList *varNameList;
        
    };
} nodeType;

// ++++++++ GLOBAL VARIABLES +++++++++
extern int sym[26]; 
extern SymbolTable *symbolTable;
