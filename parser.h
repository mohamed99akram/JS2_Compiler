// WHEN UPDATING THIS FILE, EXECUTE: make clean
typedef enum { typeCon, typeId,typeOpr } nodeEnum; // todo remove typeid2
typedef enum {typeInt, typeStr} conEnum;

// general type
typedef struct {
    conEnum type;
    union {
        int value;              /* value of constant */
        char *str;
    };
} Object;

/* operators */
typedef struct {
    int oper;                   /* operator */
    int nops;                   /* number of operands */
    struct nodeTypeTag *op[1];	/* operands, extended at runtime */
} oprNodeType;
// declare nodeTypeTag here so that it can be used in Symbol
typedef enum { typeVar, typeFunc } symbolType;
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

typedef struct {
    char *varname;
} idNodeType;

typedef struct nodeTypeTag {
    nodeEnum type;              /* type of node */

    union {
        // conNodeType con;        /* constants */
        Object con;
        idNodeType id;          /* identifiers */
        oprNodeType opr;        /* operators */
        
        
    };
} nodeType;


typedef struct SymbolTable {
    struct Symbol *head;
} SymbolTable;

extern int sym[26]; 
extern SymbolTable *symbolTable;