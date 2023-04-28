#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "parser.tab.h"

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

Object ex(nodeType *p){
    // TODO make checks for type of v(ex()) return in each call (in while, ...)
    Object o = {typeInt, 0};
    Object tmp = {typeInt, 0};
    if (!p) return o;
    switch(p->type) {
    case typeCon:  return p->con;
    case typeId:
    // traverse the symbol table to find the value
        Object* var = getSymbolValue(p->id.varname);

        printSymbolTable();
        if (var == NULL) {
            printf("Error: variable %s not found\n", p->id.varname);
            exit(1);
        }
        return *var;
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
            if (p->opr.op[0]->con.type == typeStr) {
                printf("%s\n", p->opr.op[0]->con.str);
            } else {
                // printf("%d\n", symbolTable->head->next->value.value);
                printf("%d\n", ex(p->opr.op[0]).value);
            }
            return o;
        
        case ';':       (ex(p->opr.op[0])); return (ex(p->opr.op[1]));
        case ENUM:
        case '=': 
            if (symbolTable == NULL) {
                symbolTable = (SymbolTable*) malloc(sizeof(SymbolTable));
                symbolTable->head = NULL;
            }
            if (symbolTable->head == NULL) {
                symbolTable->head = (Symbol*) malloc(sizeof(Symbol));
                symbolTable->head->name = p->opr.op[0]->id.varname;    
                symbolTable->head->type = typeVar;            
                symbolTable->head->value = ex(p->opr.op[1]);
                symbolTable->head->next = NULL;
            } else {
                Symbol *symbol = symbolTable->head;
                Symbol* prevSymbol = NULL;
                while (symbol != NULL) {
                    // printf("varname: %s\n", p->opr.op[0]->id.varname);
                    if (strcmp(symbol->name, p->opr.op[0]->id.varname) == 0) {
                        // printf("varname: %s should be updated now with value %d\n", p->opr.op[0]->id.varname, ex(p->opr.op[1]).value);
                        symbol->value = ex(p->opr.op[1]);
                        // printf("varname: %s should now be with value %d\n", p->opr.op[0]->id.varname, symbol->value.value);
                        // printf("AAAH:%d\n", symbolTable->head->next->value.value);
                        // print type of symbol
                        // printf("type of symbol: %d\n", symbol->type);
                        printSymbolTable();
                        // print address of symbolTable
                        // printf("address of symbolTable: %p\n", symbolTable);
                        // printf("AAAH:%d\n", symbolTable->head->next->value.value);
                        // exit(1);
                        return o;
                    }
                    prevSymbol = symbol;
                    // printf("current symbol: %s, \n", symbol->name);
                    symbol = symbol->next;
                    // if(symbol == NULL) printf("symbol is null\n");
                    // else printf("symbol=%s\n", symbol->name);
                }
                // printf("varname: %s should be created now\n", p->opr.op[0]->id.varname);    
            
                symbol = (Symbol*) malloc(sizeof(Symbol));
                symbol->name = p->opr.op[0]->id.varname;
                symbol->type = typeVar;
                symbol->value = ex(p->opr.op[1]);
                symbol->next = NULL;
                prevSymbol->next = symbol;
            }
            return o;
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
