%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "parser.h"

/* prototypes */
nodeType *opr(int oper, int nops, ...);
nodeType *id(char *i);
nodeType *con(Object value);
void freeNode(nodeType *p);
Object ex(nodeType *p);
int yylex(void);
void printSymbolTable();
void yyerror(char *s);
int sym[26];                    /* symbol table */
SymbolTable *symbolTable;
void pr(char *s){
    FILE *fp = fopen("calls.log", "a");
    if (fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    fprintf(fp, "%s\n", s);
    fclose(fp);

}

%}

%union { 
    int iValue;                 /* integer value */
    char sIndex;                /* symbol table index */
    char *sValue;               /* string value */
    nodeType *nPtr;             /* node pointer */
    Object dValue;            /* data value */
};

// %token <iValue> INTEGER
// %token <sValue> STRING
%token <dValue> INTEGER STRING FLOAT TRUE FALSE
// %token <sIndex> VARIABLE
%token <sValue> VARIABLE 

%token WHILE IF PRINT DO
%token SWITCH CASE DEFAULT

// - dummy tokens
%token FUNCTION_CALL FUNCTION_DECL VAR_LIST PARAM_LIST

%token FUNCTION RETURN
%token ENUM
%token CONTINUE BREAK FOR REPEAT UNTIL
%token CONST
%nonassoc IFX 
%nonassoc ELSE
%left AND OR XOR
%left GE LE EQ NE '>' '<' 
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS
%nonassoc UPLUS
%nonassoc NOT

%type <nPtr> stmt expr stmt_list enum_stmt var_list
%type <nPtr> param_list function_decl function_call
%type <nPtr> while_stmt for_stmt switch_stmt case_list case_stmt assignment_stmt do_while

%%

program:
        function                { pr("### program ###"); printSymbolTable(); exit(0); }
        ;

function:
          function stmt         { ex($2); freeNode($2); }
        | /* NULL */
        ;

function_decl: 
          FUNCTION VARIABLE '(' var_list ')' '{' stmt_list '}' { $$ = opr(FUNCTION_DECL, 3, id($2), $4, $7);  pr("function_decl"); } // opr should store function in symbol table
        | FUNCTION VARIABLE '(' var_list ')' ';' { $$ = opr(FUNCTION_DECL, 3, id($2), $4, NULL); pr("function_decl");} // opr should store function in symbol table
        | FUNCTION VARIABLE '(' var_list ')' '{' stmt_list RETURN expr ';' '}' { $$ = opr(FUNCTION_DECL, 4, id($2), $4, $7, $9); pr("function_decl"); } // TODO multiple return values
        //   FUNCTION VARIABLE '(' param_list ')' '{' stmt_list '}' { $$ = opr(FUNCTION, 3, id($2), $4, $7);  pr("function_decl"); } // opr should store function in symbol table
        // | FUNCTION VARIABLE '(' param_list ')' ';' { $$ = opr(FUNCTION, 3, id($2), $4, NULL); pr("function_decl");} // opr should store function in symbol table
        // | FUNCTION VARIABLE '(' param_list ')' '{' stmt_list RETURN expr ';' '}' { $$ = opr(FUNCTION, 4, id($2), $4, $7, $9); pr("function_decl"); } // TODO multiple return values
        ;

// list of expressions separated by ','
param_list:
        expr                    { $$ = $1; pr("param_list");}
        // | param_list ',' expr   { $$ = opr(',', 2, $1, $3); }
        | param_list ',' expr   { $$ = opr(PARAM_LIST, 2, $1, $3); pr("param_list");}
        | /* NULL */            { $$ = NULL; }
        ;
      
// list of variable names separated by ','
var_list:
          VARIABLE { $$ = id($1); }
        // | var_list ',' VARIABLE { $$ = opr(',', 2, $1, id($3)); }
        // | var_list ',' VARIABLE { $$ = opr(VAR_LIST, 2, $1, id($3)); pr("var_list"); }
        | VARIABLE ',' var_list { $$ = opr(VAR_LIST, 2, id($1), $3); pr("var_list"); } 
        | /* NULL */            { $$ = NULL; }
        ;
        
function_call:
        // TODO should be FUNCTION_CALL instead of FUNCTION
          VARIABLE '(' param_list ')' { $$ = opr(FUNCTION_CALL, 2, id($1), $3); pr("function_call"); } // opr should call function
        ;
while_stmt:
          WHILE '(' expr ')' stmt { $$ = opr(WHILE, 2, $3, $5); pr("while_stmt\n");}
          // TODO continue, break
        ;

for_stmt:
          FOR '(' assignment_stmt ';' expr ';' assignment_stmt ')' stmt { $$ = opr(FOR, 4, $3, $5, $7, $9); pr("for_stmt");}
        | FOR '(' assignment_stmt ';' expr ';' expr ')' stmt { $$ = opr(FOR, 4, $3, $5, $7, $9); pr("for_stmt");}
          // TODO continue, break
        ;

// repeat_stmt:
//           REPEAT stmt UNTIL '(' expr ')' ';' { $$ = opr(REPEAT, 2, $2, $5); pr("repeat_stmt");}
//           // TODO continue, break
//         ;
do_while:
            DO stmt WHILE '(' expr ')' ';' { $$ = opr(DO, 2, $2, $5); pr("do_while");}
            // TODO continue, break
            ;

switch_stmt:
          SWITCH '(' expr ')' '{' case_list '}' { $$ = opr(SWITCH, 2, $3, $6); pr("switch_stmt");}
          
        ;

case_list:
          case_stmt { $$ = $1; }
        | case_list case_stmt { $$ = opr(';', 2, $1, $2); }
        ;

case_stmt:
          CASE expr ':' stmt { $$ = opr(CASE, 2, $2, $4); }
        | DEFAULT ':' stmt { $$ = opr(DEFAULT, 1, $3); }
        ;

assignment_stmt:
          VARIABLE '=' expr { $$ = opr('=', 2, id($1), $3); pr("assignment_stmt"); }
        | CONST VARIABLE '=' expr    { $$ = opr(CONST, 2, id($2), $4); pr("const variable assignment");}
        ;

enum_stmt:
            // '{' enum_list '}' { $$ = $2; }
            '{' var_list '}' { $$ = opr(ENUM, 1, $2); pr("enum_stmt");}
            ;

// enum_list:
//             VARIABLE { $$ = opr(ENUM, 1, id($1)); }
//             | enum_list ',' VARIABLE { $$ = opr(ENUM, 2, $1, id($3)); }
//             ;

stmt:
          ';'                            { $$ = opr(';', 2, NULL, NULL); }
        | expr ';'                       { $$ = $1; }
        
        | PRINT '(' expr ')' ';'         { $$ = opr(PRINT, 1, $3); pr("print");} // todo make in function calls? 
        
        | assignment_stmt ';'           { $$ = $1; }
        // | ENUM VARIABLE '=' expr ';'     { $$ = opr(ENUM, 2, id($2), $4); pr("enum variable assignment");}
        | enum_stmt ';'                  { $$ = $1; }
        | IF '(' expr ')' stmt %prec IFX { $$ = opr(IF, 2, $3, $5); }
        | IF '(' expr ')' stmt ELSE stmt { $$ = opr(IF, 3, $3, $5, $7); }
        | switch_stmt                    { $$ = $1; }
        | '{' stmt_list '}'              { $$ = $2; }
        
        | function_decl                  { $$ = $1; }
        // | RETURN expr ';'                { $$ = opr(RETURN, 1, $2); } // TODO iS tHiS aLlOwEd?
        
        | while_stmt                     { $$ = $1; }
        | do_while                       { $$ = $1; }
        | for_stmt                       { $$ = $1; }
        // | CONTINUE ';'                   { $$ = opr(CONTINUE, 1, NULL); } // TODO iS tHiS aLlOwEd?
        // | BREAK ';'                      { $$ = opr(BREAK, 1, NULL); } // TODO iS tHiS aLlOwEd?
        ;


stmt_list:
          stmt                  { $$ = $1; }
        | stmt_list stmt        { $$ = opr(';', 2, $1, $2); }
        ;



expr:
          INTEGER               { $$ = con($1); }
        // | STRING                { $$ = con_str($1); }
        | STRING                { $$ = con($1); }
        | FLOAT                 { $$ = con($1); }
        | TRUE                  { $$ = con($1); }
        | FALSE                 { $$ = con($1); }

        | VARIABLE              { $$ = id($1); }
        | function_call         { $$ = $1; }
        | '-' expr %prec UMINUS { $$ = opr(UMINUS, 1, $2); }
        | '+' expr %prec UPLUS  { $$ = opr(UPLUS, 1, $2); }
        | NOT expr              { $$ = opr(NOT, 1, $2); }
        | expr '+' expr         { $$ = opr('+', 2, $1, $3); }
        | expr '-' expr         { $$ = opr('-', 2, $1, $3); }
        | expr '*' expr         { $$ = opr('*', 2, $1, $3); }
        | expr '/' expr         { $$ = opr('/', 2, $1, $3); }
        | expr '<' expr         { $$ = opr('<', 2, $1, $3); }
        | expr '>' expr         { $$ = opr('>', 2, $1, $3); }
        | expr GE expr          { $$ = opr(GE, 2, $1, $3); }
        | expr LE expr          { $$ = opr(LE, 2, $1, $3); }
        | expr NE expr          { $$ = opr(NE, 2, $1, $3); }
        | expr EQ expr          { $$ = opr(EQ, 2, $1, $3); }
        | expr AND expr         { $$ = opr(AND, 2, $1, $3); }
        | expr OR expr          { $$ = opr(OR, 2, $1, $3); }
        | expr XOR expr         { $$ = opr(XOR, 2, $1, $3); }

        
        | '(' expr ')'          { $$ = $2; }
        ;

%%


nodeType *con(Object value) {
    nodeType *p;

    /* allocate node */
    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeVal; // node type
    p->val.type = value.type; // data type
    if(value.type == typeStr){
        p->val.str = value.str; // TODO should we copy the string?
    } else if(value.type == typeInt){
        p->val.value = value.value;
    } else if(value.type == typeFloat){
        p->val.fvalue = value.fvalue;
    } else if(value.type == typeBool){
        p->val.value = value.value;
    }

    return p;
}
nodeType *id(char* varname) {
    nodeType *p;

    /* allocate node */
    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeId;
    p->id.varname = varname;

    return p;
}

nodeType *opr(int oper, int nops, ...) {
    va_list ap;
    nodeType *p;
    int i;

    /* allocate node, extending op array */
    if ((p = malloc(sizeof(nodeType) + (nops-1) * sizeof(nodeType *))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeOpr;
    p->opr.oper = oper;
    p->opr.nops = nops;
    va_start(ap, nops);
    for (i = 0; i < nops; i++)
        p->opr.op[i] = va_arg(ap, nodeType*);
    va_end(ap);
    return p;
}

void freeNode(nodeType *p) {
    int i;

    if (!p) return;
    if (p->type == typeOpr) {
        for (i = 0; i < p->opr.nops; i++)
            freeNode(p->opr.op[i]);
    }
    free (p);
}

void yyerror(char *s) {
    fprintf(stdout, "%s\n", s);
}

int main(void) {
    yyparse();
    return 0;
}
