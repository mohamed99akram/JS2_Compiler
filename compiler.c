#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "parser.tab.h"
#include "log.c"
#include "symbol_table.c"

Object ex(nodeType *p, int yylineno);

void printObj(Object o, FILE *fp)
{
    if (fp == NULL)
    {
        printf("Could not open file");
        return;
    }
    switch (o.type)
    {
    case typeInt:
        fprintf(fp, "%d\n", o.value);
        break;
    case typeStr:
        fprintf(fp, "%s\n", o.str);
        break;
    case typeFloat:
        fprintf(fp, "%f\n", o.fvalue);
        break;
    case typeBool:
        fprintf(fp, "%s\n", o.value ? "true" : "false");
    default:
        break;
    }
}

/**
 * Function value to return the value of an node object
 * @param ex The node object
 * @return The node value
 */
int v(Object ex)
{
    if (ex.type == typeInt)
    {
        return ex.value;
    }
    else if (ex.type == typeFloat)
    {
        return ex.fvalue;
    }
    else if (ex.type == typeBool)
    {
        return ex.value;
    }
    else
    {
        return 0;
    }
}

void printNode(nodeType *p, int level)
{
    // char *filename = "nodes.txt";
    // FILE *fp = fopen(filename, "a");
    // // fp = stdout;
    // if (fp == NULL)
    // {
    //     printf("Could not open file %s", filename);
    //     return;
    // }
    // if (p == NULL)
    // {
    //     fprintf(fp, "NULL\n");
    //     return;
    // }
    // // nodes.log

    // char *tabs = (char *)malloc(sizeof(char));
    // tabs[0] = '\0';
    // for (int i = 0; i < level; i++)
    // {
    //     tabs = realloc(tabs, strlen(tabs) + strlen("  ") + 1);
    //     if (tabs == NULL)
    //     {
    //         printf("Error allocating memory\n");
    //     }
    //     strcat(tabs, "  ");
    // }
    // if (p->type == typeVal)
    // {
    //     fprintf(fp, "%s", tabs);
    //     printObj(p->val, fp);
    // }
    // else if (p->type == typeId)
    // {
    //     fprintf(fp, "%s", tabs);
    //     fprintf(fp, "typeId: %s\n", p->id.varname);

    //     Symbol *tmp_symbol = getSymbol(p->id.varname);
    //     tabs = realloc(tabs, strlen(tabs) + strlen("  ") + 1);
    //     if (tabs == NULL)
    //     {
    //         printf("Error allocating memory\n");
    //     }
    //     strcat(tabs, "  ");
    //     if (tmp_symbol == NULL)
    //     {
    //         // fprintf(fp, "Error: variable %s not found\n", p->id.varname);
    //         fprintf(fp, "%s", tabs);
    //         fprintf(fp, "Error: variable %s not found\n", p->id.varname);
    //     }
    //     else
    //     {
    //         fprintf(fp, "%s", tabs);
    //         fprintf(fp, "Symbol type: %d\n", tmp_symbol->type);
    //         fprintf(fp, "%s", tabs);
    //         fprintf(fp, "Value type: %d\n", tmp_symbol->value.type);
    //         fprintf(fp, "%s", tabs);
    //         printObj(tmp_symbol->value, fp);
    //     }
    // }
    // else if (p->type == typeOpr)
    // {
    //     fprintf(fp, "%s", tabs);
    //     fprintf(fp, "typeOpr: %d\n", p->opr.oper);
    //     for (int i = 0; i < p->opr.nops; i++)
    //     {
    //         fclose(fp);
    //         printNode(p->opr.op[i], level + 1);
    //         fp = fopen("nodes.log", "a");
    //     }
    // }
    // else if (p->type == typeVarNameList)
    // {
    //     fprintf(fp, "%s", tabs);
    //     fprintf(fp, "typeVarNameList: ");
    //     VarNameList *varNameList = p->varNameList;
    //     VarName *varName = varNameList->head;
    //     while (varName != NULL)
    //     {
    //         fprintf(fp, "%s", tabs);
    //         fprintf(fp, "%s ", varName->name);
    //         varName = varName->next;
    //     }
    //     fprintf(fp, "\n");
    // }
    // else
    // {
    //     fprintf(fp, "%s", tabs);
    //     fprintf(fp, "Error: unknown node type\n");
    // }
    // fclose(fp);
}
VarNameList *getVarNames(nodeType *p)
{
    /// expand VAR_LIST to get linked list of variable names
    VarNameList *namesList = (VarNameList *)malloc(sizeof(VarNameList));
    namesList->head = (VarName *)malloc(sizeof(VarName));

    VarName *varName = namesList->head;
    while (p != NULL)
    {
        if (p->type == typeOpr && p->opr.oper == VAR_LIST)
        {
            varName->name = p->opr.op[0]->id.varname;
            varName->next = (VarName *)malloc(sizeof(VarName));
            varName = varName->next;
            p = p->opr.op[1];
        }
        else if (p->type == typeId)
        {
            varName->name = p->id.varname;
            varName->next = NULL;
            break;
        }
        else
        {
            printf("Error: unknown node type\n");
            exit(1);
        }
    }
    return namesList;
}

Object ex(nodeType *p, int yylineno)
{
    printNode(p, 0);
    // TODO make checks for type of v(ex()) return in each call (in while, ...)
    Object o = {typeInt, 0};
    Object tmp = {typeInt, 0};

    // open log file for translation
    FILE *f = get_log();

    // instantiate the symbol table
    SymbolTable *st = get_symbol_table_instance();

    if (!p)
        return o;
    switch (p->type)
    {
    case typeVal:
        return p->val;

    case typeId:;
        // traverse the symbol table to find the value
        // Object* var = getSymbolValue(p->id.varname);
        // Symbol *tmp_symbol = getSymbol(p->id.varname);
        // if (tmp_symbol == NULL)
        // {
        //     printf("Error: variable %s not found\n", p->id.varname);
        //     exit(1);
        // }
        // Object *var = &(tmp_symbol->value);
        // if (var == NULL)
        // { // TODO seems like this is not needed
        //     printf("Error: variable %s not found\n", p->id.varname);
        //     exit(1);
        // }
        // return *var;

        // return o;
    case typeOpr:
        switch (p->opr.oper)
        {
        case WHILE:
            while (v(ex(p->opr.op[0], yylineno)))
                (ex(p->opr.op[1], yylineno));
            return o;
        case FOR:
            for ((ex(p->opr.op[0], yylineno)); v(ex(p->opr.op[1], yylineno)); (ex(p->opr.op[2], yylineno)))
                (ex(p->opr.op[3], yylineno));
            return o;
        case DO:
            do
                (ex(p->opr.op[0], yylineno));
            while (v(ex(p->opr.op[1], yylineno)));
            return o;

        case IF:
            if (v(ex(p->opr.op[0], yylineno)))
                (ex(p->opr.op[1], yylineno));
            else if (p->opr.nops > 2)
                (ex(p->opr.op[2], yylineno));
            return o;
        case VAR_LIST:; // variable names sepearated by ',': used for enum, function_decl
                        // TODO : this is rubbish, it changes p's type to typeVarNameList !! look how enum is done, maybe repeat for function_decl

            // printf("VAR_LIST\n");
            VarNameList *namesList = getVarNames(p);
            VarName *varName = namesList->head;
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
        {
            fprintf(f, "%s\n", "print");
            Object val = ex(p->opr.op[0], yylineno);
            break;
        }

        case ';':
            (ex(p->opr.op[0], yylineno));
            return (ex(p->opr.op[1], yylineno));
        case CONST:
            // return createVar(p->opr.op[0]->id.varname, ex(p->opr.op[1]), typeConst);

        case ENUM:;
            // printf("enum\n");
            // printf("%d\n", p->opr.op[0]->type);
            // ex(p->opr.op[0]);
            // printNode(p, 0);

            // create variables for each enum value
            VarNameList *namesList2 = getVarNames(p->opr.op[0]);
            VarName *varName2 = namesList2->head;
            // enumerate their value
            int i = 0;
            while (varName2 != NULL)
            {
                Object tmp = {typeInt, i};
                // createVar(varName2->name, tmp, typeEnum);
                varName2 = varName2->next;
                i++;
            }
            return o;

        case '=':
        {
            Symbol *new_assign_symbol = createSymbol(p->opr.op[0]->id.varname, typeVar, typeInt, 0, 1, yylineno);
            insertSymbol(new_assign_symbol, st);
            // LOG("after insertion")
            printSymbolTable(st);

            // calculate the expresion value
            Object val = ex(p->opr.op[1], yylineno);

            // translate into quadrubles
            fprintf(f, "PUSH %d\n", val.value);
            fprintf(f, "POP %s\n", p->opr.op[0]->id.varname);
            break;
        }

            // return createVar(p->opr.op[0]->id.varname, ex(p->opr.op[1]), typeVar);
        // TODO change for other types
        case UMINUS:;
            tmp = ex(p->opr.op[0], yylineno);
            if (tmp.type == typeInt)
            {
                tmp.value = -tmp.value;
            }
            else if (tmp.type == typeFloat)
            {
                tmp.fvalue = -tmp.fvalue;
            }
            return tmp;

        case UPLUS:;
            tmp = ex(p->opr.op[0], yylineno);
            if (tmp.type == typeInt)
            {
                tmp.value = +tmp.value;
            }
            else if (tmp.type == typeFloat)
            {
                tmp.fvalue = +tmp.fvalue;
            }
            return tmp;

        // TODO
        // TODO :::: CAN'T WE MINIMIZE THESE CASES? :::::
        // TODO
        case '+':; // bool addition not supported
            Object left = ex(p->opr.op[0], yylineno);
            Object right = ex(p->opr.op[1], yylineno);
            if (left.type == typeInt && right.type == typeInt)
            {
                tmp.value = left.value + right.value;
                return tmp;
            }
            else if (left.type == typeFloat && right.type == typeFloat)
            {
                tmp.type = typeFloat;
                tmp.fvalue = left.fvalue + right.fvalue;
                return tmp;
            }
            else if (left.type == typeFloat && right.type == typeInt)
            {
                tmp.type = typeFloat;
                tmp.fvalue = left.fvalue + right.value;
                return tmp;
            }
            else if (left.type == typeInt && right.type == typeFloat)
            {
                tmp.type = typeFloat;
                tmp.fvalue = left.value + right.fvalue;
                return tmp;
            }
            else if (left.type == typeStr && right.type == typeStr)
            {
                tmp.type = typeStr;
                tmp.str = (char *)malloc(strlen(left.str) + strlen(right.str) + 1);
                strcpy(tmp.str, left.str);
                strcat(tmp.str, right.str);
                return tmp;
            }
            else
            {
                printf("Error: unsupported types for +\n");
                exit(1);
            }
        case '-':;
            Object left2 = ex(p->opr.op[0], yylineno);
            Object right2 = ex(p->opr.op[1], yylineno);
            if (left2.type == typeInt && right2.type == typeInt)
            {
                tmp.value = left2.value - right2.value;
                return tmp;
            }
            else if (left2.type == typeFloat && right2.type == typeFloat)
            {
                tmp.type = typeFloat;
                tmp.fvalue = left2.fvalue - right2.fvalue;
                return tmp;
            }
            else if (left2.type == typeFloat && right2.type == typeInt)
            {
                tmp.type = typeFloat;
                tmp.fvalue = left2.fvalue - right2.value;
                return tmp;
            }
            else if (left2.type == typeInt && right2.type == typeFloat)
            {
                tmp.type = typeFloat;
                tmp.fvalue = left2.value - right2.fvalue;
                return tmp;
            }
            else
            {
                printf("Error: unsupported types for -\n");
                exit(1);
            }
        case '*':;
            Object left3 = ex(p->opr.op[0], yylineno);
            Object right3 = ex(p->opr.op[1], yylineno);
            if (left3.type == typeInt && right3.type == typeInt)
            {
                tmp.value = left3.value * right3.value;
                return tmp;
            }
            else if (left3.type == typeFloat && right3.type == typeFloat)
            {
                tmp.type = typeFloat;
                tmp.fvalue = left3.fvalue * right3.fvalue;
                return tmp;
            }
            else if (left3.type == typeFloat && right3.type == typeInt)
            {
                tmp.type = typeFloat;
                tmp.fvalue = left3.fvalue * right3.value;
                return tmp;
            }
            else if (left3.type == typeInt && right3.type == typeFloat)
            {
                tmp.type = typeFloat;
                tmp.fvalue = left3.value * right3.fvalue;
                return tmp;
            }
            else
            {
                printf("Error: unsupported types for *\n");
                exit(1);
            }
        case '/':;
            Object left4 = ex(p->opr.op[0], yylineno);
            Object right4 = ex(p->opr.op[1], yylineno);
            if (left4.type == typeInt && right4.type == typeInt)
            {
                tmp.value = left4.value / right4.value;
                return tmp;
            }
            else if (left4.type == typeFloat && right4.type == typeFloat)
            {
                tmp.type = typeFloat;
                tmp.fvalue = left4.fvalue / right4.fvalue;
                return tmp;
            }
            else if (left4.type == typeFloat && right4.type == typeInt)
            {
                tmp.type = typeFloat;
                tmp.fvalue = left4.fvalue / right4.value;
                return tmp;
            }
            else if (left4.type == typeInt && right4.type == typeFloat)
            {
                tmp.type = typeFloat;
                tmp.fvalue = left4.value / right4.fvalue;
                return tmp;
            }
            else
            {
                printf("Error: unsupported types for /\n");
                exit(1);
            }
        case '<':;
            Object left5 = ex(p->opr.op[0], yylineno);
            Object right5 = ex(p->opr.op[1], yylineno);
            if ((left5.type == typeInt || left5.type == typeBool) && (right5.type == typeInt || right5.type == typeBool))
            {
                tmp.type = typeBool;
                tmp.value = left5.value < right5.value;
                return tmp;
            }
            else if (left5.type == typeFloat && right5.type == typeFloat)
            {
                tmp.type = typeBool;
                tmp.value = left5.fvalue < right5.fvalue;
                return tmp;
            }
            else if (left5.type == typeFloat && (right5.type == typeInt || right5.type == typeBool))
            {
                tmp.type = typeBool;
                tmp.value = left5.fvalue < right5.value;
                return tmp;
            }
            else if ((left5.type == typeInt || left5.type == typeBool) && right5.type == typeFloat)
            {
                tmp.type = typeBool;
                tmp.value = left5.value < right5.fvalue;
                return tmp;
            }
            else
            {
                printf("Error: unsupported types for <\n");
                exit(1);
            }
        case '>':;
            Object left6 = ex(p->opr.op[0], yylineno);
            Object right6 = ex(p->opr.op[1], yylineno);
            if ((left6.type == typeInt || left6.type == typeBool) && (right6.type == typeInt || right6.type == typeBool))
            {
                tmp.type = typeBool;
                tmp.value = left6.value > right6.value;
                return tmp;
            }
            else if (left6.type == typeFloat && right6.type == typeFloat)
            {
                tmp.type = typeBool;
                tmp.value = left6.fvalue > right6.fvalue;
                return tmp;
            }
            else if (left6.type == typeFloat && (right6.type == typeInt || right6.type == typeBool))
            {
                tmp.type = typeBool;
                tmp.value = left6.fvalue > right6.value;
                return tmp;
            }
            else if ((left6.type == typeInt || left6.type == typeBool) && right6.type == typeFloat)
            {
                tmp.type = typeBool;
                tmp.value = left6.value > right6.fvalue;
                return tmp;
            }
            else
            {
                printf("Error: unsupported types for >\n");
                exit(1);
            }
        case GE:;
            Object left7 = ex(p->opr.op[0], yylineno);
            Object right7 = ex(p->opr.op[1], yylineno);
            if ((left7.type == typeInt || left7.type == typeBool) && (right7.type == typeInt || right7.type == typeBool))
            {
                tmp.type = typeBool;
                tmp.value = left7.value >= right7.value;
                return tmp;
            }
            else if (left7.type == typeFloat && right7.type == typeFloat)
            {
                tmp.type = typeBool;
                tmp.value = left7.fvalue >= right7.fvalue;
                return tmp;
            }
            else if (left7.type == typeFloat && (right7.type == typeInt || right7.type == typeBool))
            {
                tmp.type = typeBool;
                tmp.value = left7.fvalue >= right7.value;
                return tmp;
            }
            else if ((left7.type == typeInt || left7.type == typeBool) && right7.type == typeFloat)
            {
                tmp.type = typeBool;
                tmp.value = left7.value >= right7.fvalue;
                return tmp;
            }
            else
            {
                printf("Error: unsupported types for >=\n");
                exit(1);
            }
        case LE:;
            Object left8 = ex(p->opr.op[0], yylineno);
            Object right8 = ex(p->opr.op[1], yylineno);
            if ((left8.type == typeInt || left8.type == typeBool) && (right8.type == typeInt || right8.type == typeBool))
            {
                tmp.type = typeBool;
                tmp.value = left8.value <= right8.value;
                return tmp;
            }
            else if (left8.type == typeFloat && right8.type == typeFloat)
            {
                tmp.type = typeBool;
                tmp.value = left8.fvalue <= right8.fvalue;
                return tmp;
            }
            else if (left8.type == typeFloat && (right8.type == typeInt || right8.type == typeBool))
            {
                tmp.type = typeBool;
                tmp.value = left8.fvalue <= right8.value;
                return tmp;
            }
            else if ((left8.type == typeInt || left8.type == typeBool) && right8.type == typeFloat)
            {
                tmp.type = typeBool;
                tmp.value = left8.value <= right8.fvalue;
                return tmp;
            }
            else
            {
                printf("Error: unsupported types for <=\n");
                exit(1);
            }
        case NE:;
            Object left9 = ex(p->opr.op[0], yylineno);
            Object right9 = ex(p->opr.op[1], yylineno);
            if ((left9.type == typeInt || left9.type == typeBool) && (right9.type == typeInt || right9.type == typeBool))
            {
                tmp.type = typeBool;
                tmp.value = left9.value != right9.value;
                return tmp;
            }
            else if (left9.type == typeFloat && right9.type == typeFloat)
            {
                tmp.type = typeBool;
                tmp.value = left9.fvalue != right9.fvalue;
                return tmp;
            }
            else if (left9.type == typeFloat && (right9.type == typeInt || right9.type == typeBool))
            {
                tmp.type = typeBool;
                tmp.value = left9.fvalue != right9.value;
                return tmp;
            }
            else if ((left9.type == typeInt || left9.type == typeBool) && right9.type == typeFloat)
            {
                tmp.type = typeBool;
                tmp.value = left9.value != right9.fvalue;
                return tmp;
            }
            else
            {
                printf("Error: unsupported types for !=\n");
                exit(1);
            }
        case EQ:;
            Object left10 = ex(p->opr.op[0], yylineno);
            Object right10 = ex(p->opr.op[1], yylineno);
            if ((left10.type == typeInt || left10.type == typeBool) && (right10.type == typeInt || right10.type == typeBool))
            {
                tmp.type = typeBool;
                tmp.value = left10.value == right10.value;
                return tmp;
            }
            else if (left10.type == typeFloat && right10.type == typeFloat)
            {
                tmp.type = typeBool;
                tmp.value = left10.fvalue == right10.fvalue;
                return tmp;
            }
            else if (left10.type == typeFloat && (right10.type == typeInt || right10.type == typeBool))
            {
                tmp.type = typeBool;
                tmp.value = left10.fvalue == right10.value;
                return tmp;
            }
            else if ((left10.type == typeInt || left10.type == typeBool) && right10.type == typeFloat)
            {
                tmp.type = typeBool;
                tmp.value = left10.value == right10.fvalue;
                return tmp;
            }
            else
            {
                printf("Error: unsupported types for ==\n");
                exit(1);
            }
        case AND:;
            Object left11 = ex(p->opr.op[0], yylineno);
            Object right11 = ex(p->opr.op[1], yylineno);
            tmp.type = typeBool;
            tmp.value = v(left11) && v(right11);
            return tmp;

        case OR:;
            Object left12 = ex(p->opr.op[0], yylineno);
            Object right12 = ex(p->opr.op[1], yylineno);
            tmp.type = typeBool;
            tmp.value = v(left12) || v(right12);
            return tmp;

        case NOT:;
            Object left13 = ex(p->opr.op[0], yylineno);
            tmp.type = typeBool;
            tmp.value = !v(left13);
            return tmp;

        case XOR:;
            Object left14 = ex(p->opr.op[0], yylineno);
            Object right14 = ex(p->opr.op[1], yylineno);
            tmp.type = typeBool;
            tmp.value = v(left14) ^ v(right14);
            return tmp;
        }
    }
    // fclose(f);
    return o;
}
