#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "parser.tab.h"
Object ex(nodeType *p);
void printSymbolTable() {
    Symbol *symbol = symbolTable->head;
    char* filename = "symbol_table.log";
    FILE *fp = fopen(filename, "w");
    while (symbol != NULL) {
        fprintf(fp, "%s: ", symbol->name);
        // printf( "FFF %s: ", symbol->name);
        switch (symbol->value.type) {
            case typeInt:
                fprintf(fp, "%d\n", symbol->value.value);
                break;
            case typeStr:
                fprintf(fp, "%s\n", symbol->value.str);
                break;
            default:
                break;
        }
        symbol = symbol->next;
    }
     // close the file
        fclose(fp);
}

// Object* getSymbolValue(SymbolTable *symbolTable, char* name){
Object* getSymbolValue(char* name){
    // TODO if the symbol is not found, error
    Symbol *symbol = symbolTable->head;
    while (symbol != NULL) {
        if (strcmp(symbol->name, name) == 0) {
            return &(symbol->value);
        }
        symbol = symbol->next;
    }
    return NULL;
}

int v(Object ex){
    if (ex.type == typeInt) {
        return ex.value;
    } else {
        return 0;
    }
}

Object createVar(char* varname, Object val, int type){
    Object o = {typeInt, 0};
    if (symbolTable == NULL) {
                symbolTable = (SymbolTable*) malloc(sizeof(SymbolTable));
                symbolTable->head = NULL;
            }
            if (symbolTable->head == NULL) {
                symbolTable->head = (Symbol*) malloc(sizeof(Symbol));
                symbolTable->head->name = varname;    
                symbolTable->head->type = type;            
                symbolTable->head->value = val;
                symbolTable->head->next = NULL;
            } else {
                Symbol *symbol = symbolTable->head;
                Symbol* prevSymbol = NULL;
                while (symbol != NULL) {
                    if (strcmp(symbol->name, varname) == 0) {
                        symbol->value = val;
                        printSymbolTable();
                        return o;
                    }
                    prevSymbol = symbol;
                    symbol = symbol->next;
                }  
            
                symbol = (Symbol*) malloc(sizeof(Symbol));
                symbol->name = varname;
                symbol->type = type;
                symbol->value = val;
                symbol->next = NULL;
                prevSymbol->next = symbol;
            }
            return o;
}

Object ex(nodeType *p){
    // TODO make checks for type of v(ex()) return in each call (in while, ...)
    Object o = {typeInt, 0};
    Object tmp = {typeInt, 0};
    if (!p) return o;
    switch(p->type) {
    case typeVal:  return p->val;
    case typeId:
    // traverse the symbol table to find the value
        Object* var = getSymbolValue(p->id.varname);

        printSymbolTable();
        if (var == NULL) {
            printf("Error: variable %s not found\n", p->id.varname);
            exit(1);
        }
        return *var;
        // return o;
    case typeOpr:
        switch(p->opr.oper) {
        case WHILE: while(v(ex(p->opr.op[0]))) (ex(p->opr.op[1]));return o;
        case FOR: for((ex(p->opr.op[0])); v(ex(p->opr.op[1])); (ex(p->opr.op[2]))) (ex(p->opr.op[3])); return o;
        case DO:    do (ex(p->opr.op[0])); while(v(ex(p->opr.op[1]))); return o;
        
        case IF:        if (v(ex(p->opr.op[0])))
                            (ex(p->opr.op[1]));
                        else if (p->opr.nops > 2)
                            (ex(p->opr.op[2]));
                        return o;
        
        // case ',':

        // case FUNCTION:
        //     if(p->opr.nops == 3) // function declaration: 1: name, 2: parameters, 3: statements
        //     {
        //         if (symbolTable == NULL) {
        //             symbolTable = (SymbolTable*) malloc(sizeof(SymbolTable));
        //             symbolTable->head = NULL;
        //         }
        //         if (symbolTable->head == NULL) {
        //             symbolTable->head = (Symbol*) malloc(sizeof(Symbol));
        //             symbolTable->head->name = p->opr.op[0]->id.varname;
        //             symbolTable->head->type = typeFunc;
        //             nodeType* p = p->opr.op[1];

        //             // symbolTable->head->parameters = p->opr.op[1];
        //             symbolTable->head->operations = p->opr.op[2];
                    
        //             symbolTable->head->next = NULL;
        //         } else {
        //             Symbol *symbol = symbolTable->head;
        //             while (symbol != NULL) {
        //                 if (strcmp(symbol->name, p->opr.op[0]->id.varname) == 0) {
        //                     symbol->type = typeFunc;
                            
        //                     return o;
        //                 }
        //                 symbol = symbol->next;
        //             }
        //             symbol = (Symbol*) malloc(sizeof(Symbol));
        //             symbol->name = p->opr.op[0]->id.varname;
        //             symbol->type = typeFunc;
        //             symbol->next = NULL;
        //         }
        //     }
        case PRINT:
            if (p->opr.op[0]->val.type == typeStr) {
                printf("%s\n", p->opr.op[0]->val.str);
            } else {
                // printf("%d\n", symbolTable->head->next->value.value);
                printf("%d\n", ex(p->opr.op[0]).value);
            }
            return o;
        
        case ';':       (ex(p->opr.op[0])); return (ex(p->opr.op[1]));
        case CONST:
            return createVar(p->opr.op[0]->id.varname, ex(p->opr.op[1]), typeConst);
            
        case ENUM:
            printf("enum\n");
            printf("%d\n", p->opr.nops);

            // for(int i = 0; i < p->opr.nops; i++) {
            //     // createVar(p->opr.op[i]->id.varname, (Object) {typeInt, i}, typeEnum);
            //     printf("%s = %d\n", p->opr.op[i]->id.varname, i);
            // }
            // printSymbolTable();
            return o;
        case '=': 
            return createVar(p->opr.op[0]->id.varname, ex(p->opr.op[1]), typeVar);
        // TODO change for other types
        case UMINUS:tmp.value = -v(ex(p->opr.op[0])); return tmp;
        case '+':   tmp.value = v(ex(p->opr.op[0])) + v(ex(p->opr.op[1])); return tmp;
        case '-':   tmp.value = v(ex(p->opr.op[0])) - v(ex(p->opr.op[1])); return tmp;
        case '*':   tmp.value = v(ex(p->opr.op[0])) * v(ex(p->opr.op[1])); return tmp;
        case '/':   tmp.value = v(ex(p->opr.op[0])) / v(ex(p->opr.op[1])); return tmp;
        case '<':   tmp.value = v(ex(p->opr.op[0])) < v(ex(p->opr.op[1])); return tmp;
        case '>':   tmp.value = v(ex(p->opr.op[0])) > v(ex(p->opr.op[1])); return tmp;
        case GE:    tmp.value = v(ex(p->opr.op[0])) >= v(ex(p->opr.op[1])); return tmp;
        case LE:    tmp.value = v(ex(p->opr.op[0])) <= v(ex(p->opr.op[1])); return tmp;
        case NE:    tmp.value = v(ex(p->opr.op[0])) != v(ex(p->opr.op[1])); return tmp;
        case EQ:    tmp.value = v(ex(p->opr.op[0])) == v(ex(p->opr.op[1])); return tmp;
        }
    }
    return o;
}
