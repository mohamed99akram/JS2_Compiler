// WHEN UPDATING THIS FILE, EXECUTE: make clean
// typedef enum { typeCon, typeId, typeOpr, typeStr } nodeEnum;
typedef enum { typeCon, typeId, typeId2, typeOpr } nodeEnum; // todo remove typeid2
typedef enum {typeInt, typeStr} conEnum;
// general type
typedef struct {
    conEnum type;
    union {
        int value;              /* value of constant */
        char *str;
    };
} Object;
/* constants */
// typedef struct {
//     // int value;                  /* value of constant */
//     // can be int or string
//     // TODO change it back to int
//     union {
//         int value;
//         char *str;
//     };

// } conNodeType;
// typedef struct {
//     char *str;
// } strNodeType;

/* identifiers */
typedef struct {
    int i;                      /* subscript to sym array */
} idNodeType2;

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
        idNodeType2 id2;          /* identifiers */
        oprNodeType opr;        /* operators */
        
        
    };
} nodeType;


typedef struct SymbolTable {
    struct Symbol *head;
} SymbolTable;

extern int sym[26]; 
extern SymbolTable *symbolTable;