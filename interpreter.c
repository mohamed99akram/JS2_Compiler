#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "parser.tab.h"
Object ex(nodeType *p);
void printSymbolTable() {
    char* filename = "symbol_table.log";
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Could not open file %s", filename);
        return;
    }
    if (symbolTable == NULL) {
        fprintf(fp, "Symbol table is empty\n");
        fclose(fp);
        return;
    }
    Symbol *symbol = symbolTable->head;

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

Symbol* getSymbol(char* name){
    // name: name of the symbol
    // search the symbol table for the symbol and return it
    // TODO if the symbol is not found, error
    if (symbolTable == NULL) {
        return NULL;
    }
    Symbol *symbol = symbolTable->head;
    while (symbol != NULL) {
        if (strcmp(symbol->name, name) == 0) {
            return symbol;
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
    // varname: name of the variable
    // val: value of the variable
    // type: type of the variable (variable, const, enum)
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
                        symbol->type = type;
                        // printSymbolTable();
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

void printNode(nodeType*p, int level){
    char* filename = "nodes.log";
    FILE *fp = fopen(filename, "a");
    // fp = stdout;
    if (fp == NULL) {
        printf("Could not open file %s", filename);
        return;
    }
    if(p == NULL){
        fprintf(fp, "NULL\n");
        return;
    }
    // nodes.log
    
    for(int i = 0; i < level; i++)
    {
        fprintf(fp, "  ");
    }
    if (p->type == typeVal)
    {
        if(p->val.type == typeStr){
            fprintf(fp, "typeStr: %s\n", p->val.str);
        }
        else if(p->val.type == typeInt){
            fprintf(fp, "typeInt: %d\n", p->val.value);
        }
    }
    else if (p->type == typeId)
    {
        fprintf(fp, "typeId: %s\n", p->id.varname);
    }
    else if (p->type == typeOpr)
    {
        fprintf(fp, "typeOpr: %d\n", p->opr.oper);
        for(int i = 0; i < p->opr.nops; i++)
        {
            fclose(fp);
            printNode(p->opr.op[i], level + 1);
            fp = fopen("nodes.log", "a");
        }
    }
    else if (p->type == typeVarNameList){
        fprintf(fp, "typeVarNameList: ");
        VarNameList* varNameList = p->varNameList;
        VarName* varName = varNameList->head;
        while (varName != NULL) {
            fprintf(fp, "%s ", varName->name);
            varName = varName->next;
        }
        fprintf(fp, "\n");
    }
    else {
        fprintf(fp, "Error: unknown node type\n");
    }
    fclose(fp);
}
VarNameList* getVarNames(nodeType* p) {
    /// expand VAR_LIST to get linked list of variable names
    VarNameList* namesList = (VarNameList*) malloc(sizeof(VarNameList));
    namesList->head = (VarName*) malloc(sizeof(VarName));

    VarName* varName = namesList->head;
    while (p != NULL) { 
        if (p->type == typeOpr && p->opr.oper == VAR_LIST) {
            varName->name = p->opr.op[0]->id.varname;
            varName->next = (VarName*) malloc(sizeof(VarName));
            varName = varName->next;
            p = p->opr.op[1];
        } else if (p->type == typeId) {
            varName->name = p->id.varname;
            varName->next = NULL;
            break;
        } else {
            printf("Error: unknown node type\n");
            exit(1);
        }
    }
    return namesList;
}

Object ex(nodeType *p){
    printNode(p, 0);
    // TODO make checks for type of v(ex()) return in each call (in while, ...)
    Object o = {typeInt, 0};
    Object tmp = {typeInt, 0};
    if (!p) return o;
    switch(p->type) {
    case typeVal:  return p->val;
    case typeId:
    // traverse the symbol table to find the value
        // Object* var = getSymbolValue(p->id.varname);
        Symbol* tmp_symbol = getSymbol(p->id.varname);
        if (tmp_symbol == NULL) {
            printf("Error: variable %s not found\n", p->id.varname);
            exit(1);
        }
        Object* var = &(tmp_symbol->value);
        if (var == NULL) { // TODO seems like this is not needed
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
        case VAR_LIST: // variable names sepearated by ',': used for enum, function_decl
        // TODO : this is rubbish, it changes p's type to typeVarNameList !!
            // printf("VAR_LIST\n");
            VarNameList* namesList =  getVarNames(p);
            VarName* varName = namesList->head;
            p->type = typeVarNameList;
            p->varNameList = namesList;
            // printNode(p, 0);
            // while (varName != NULL) {
            //     createVar(varName->name, o, typeVar); // TODO typeVar or typeEnum? should they be separate rules? or set it when finding enum?
            //     varName = varName->next;
            // }
            
            // printSymbolTable();
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
            // printf("enum\n");
            // printf("%d\n", p->opr.op[0]->type);
            // ex(p->opr.op[0]); 
            // printNode(p, 0);

            // create variables for each enum value
            VarNameList* namesList2 =  getVarNames(p->opr.op[0]);
            VarName* varName2 = namesList2->head;
            // enumerate their value
            int i = 0;
            while (varName2 != NULL) {
                Object tmp = {typeInt, i};
                createVar(varName2->name, tmp, typeEnum);
                varName2 = varName2->next;
                i++;
            }
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
