#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "parser.tab.h"
#include "log.c"
#include "symbol_table.c"

int intermediate_variable_order = 0;

Object ex(nodeType *p);

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

Object ex(nodeType *p)
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
    {
        Object val = p->val;
        switch (val.type)
        {
        case typeInt:
            fprintf(f, "PUSH %d\n", val.value);
            break;
        case typeFloat:
            fprintf(f, "PUSH %f\n", val.fvalue);
            break;
        case typeStr:
            fprintf(f, "PUSH %s\n", val.str);
            break;
        case typeBool:
            fprintf(f, "PUSH %d\n", val.value);
            break;
        }

        break;
    }
    case typeId:
    {
        Object val = p->val;
        LOG(p->id.varname)
        fprintf(f, "PUSH %s\n", p->id.varname);
        break;
    }
    case typeOpr:
        switch (p->opr.oper)
        {
        case WHILE:
            while (v(ex(p->opr.op[0])))
                (ex(p->opr.op[1]));
            return o;
        case FOR:
            for ((ex(p->opr.op[0])); v(ex(p->opr.op[1])); (ex(p->opr.op[2])))
                (ex(p->opr.op[3]));
            return o;
        case DO:
            do
                (ex(p->opr.op[0]));
            while (v(ex(p->opr.op[1])));
            return o;

        case IF:
            if (v(ex(p->opr.op[0])))
                (ex(p->opr.op[1]));
            else if (p->opr.nops > 2)
                (ex(p->opr.op[2]));
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

        case ',':

        // case FUNCTION:
        //     if (p->opr.nops == 3) // function declaration: 1: name, 2: parameters, 3: statements
        //     {
        //         if (symbolTable == NULL)
        //         {
        //             symbolTable = (SymbolTable *)malloc(sizeof(SymbolTable));
        //             symbolTable->head = NULL;
        //         }
        //         if (symbolTable->head == NULL)
        //         {
        //             symbolTable->head = (Symbol *)malloc(sizeof(Symbol));
        //             symbolTable->head->name = p->opr.op[0]->id.varname;
        //             symbolTable->head->type = typeFunc;
        //             nodeType *p = p->opr.op[1];

        //             // symbolTable->head->parameters = p->opr.op[1];
        //             symbolTable->head->operations = p->opr.op[2];

        //             symbolTable->head->next = NULL;
        //         }
        //         else
        //         {
        //             Symbol *symbol = symbolTable->head;
        //             while (symbol != NULL)
        //             {
        //                 if (strcmp(symbol->name, p->opr.op[0]->id.varname) == 0)
        //                 {
        //                     symbol->type = typeFunc;

        //                     return o;
        //                 }
        //                 symbol = symbol->next;
        //             }
        //             symbol = (Symbol *)malloc(sizeof(Symbol));
        //             symbol->name = p->opr.op[0]->id.varname;
        //             symbol->type = typeFunc;
        //             symbol->next = NULL;
        //         }
        //     }
        case PRINT:;
            {
                fprintf(f, "%s\n", "print");
                Object val = ex(p->opr.op[0]);
                break;
            }

        case ';':;
            (ex(p->opr.op[0]));
            return (ex(p->opr.op[1]));
        case CONST:;
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
            Symbol *new_assign_symbol = createSymbol(p->opr.op[0]->id.varname, typeVar, typeInt, 0, 1, 0);
            insertSymbol(new_assign_symbol, st);
            printSymbolTable(st);
            ex(p->opr.op[1]);
            fprintf(f, "POP %s\n", p->opr.op[0]->id.varname);
            break;
        }

            // return createVar(p->opr.op[0]->id.varname, ex(p->opr.op[1]), typeVar);
        // TODO change for other types
        case UMINUS:;
            tmp = ex(p->opr.op[0]);
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
            tmp = ex(p->opr.op[0]);
            if (tmp.type == typeInt)
            {
                tmp.value = +tmp.value;
            }
            else if (tmp.type == typeFloat)
            {
                tmp.fvalue = +tmp.fvalue;
            }
            return tmp;

        case '+':
        {
            Object left = ex(p->opr.op[0]);
            Object right = ex(p->opr.op[1]);
            Object result;
            int operand_1 = intermediate_variable_order++;
            int operand_2 = intermediate_variable_order++;
            int result_operand = intermediate_variable_order++;

            fprintf(f, "POP t%d\n", operand_1);
            fprintf(f, "POP t%d\n", operand_2);
            fprintf(f, "ADD t%d, t%d, t%d\n", operand_1, operand_2, result_operand);

            break;
        }
        case '-':
        {
            Object left = ex(p->opr.op[0]);
            Object right = ex(p->opr.op[1]);
            Object result;
            int operand_1 = intermediate_variable_order++;
            int operand_2 = intermediate_variable_order++;
            int result_operand = intermediate_variable_order++;

            fprintf(f, "POP t%d\n", operand_1);
            fprintf(f, "POP t%d\n", operand_2);
            fprintf(f, "SUB t%d, t%d, t%d\n", operand_1, operand_2, result_operand);
            break;
        }
        case '*':
        {
            Object left = ex(p->opr.op[0]);
            Object right = ex(p->opr.op[1]);
            Object result;
            int operand_1 = intermediate_variable_order++;
            int operand_2 = intermediate_variable_order++;
            int result_operand = intermediate_variable_order++;

            fprintf(f, "POP t%d\n", operand_1);
            fprintf(f, "POP t%d\n", operand_2);
            fprintf(f, "MUL t%d, t%d, t%d\n", operand_1, operand_2, result_operand);
            break;
        }
        case '/':
        {
            Object left = ex(p->opr.op[0]);
            Object right = ex(p->opr.op[1]);
            Object result;
            int operand_1 = intermediate_variable_order++;
            int operand_2 = intermediate_variable_order++;
            int result_operand = intermediate_variable_order++;

            fprintf(f, "POP t%d\n", operand_1);
            fprintf(f, "POP t%d\n", operand_2);
            fprintf(f, "DIV t%d, t%d, t%d\n", operand_1, operand_2, result_operand);
            break;
        }
        case '<':
        {
            Object left = ex(p->opr.op[0]);
            Object right = ex(p->opr.op[1]);
            int operand_1 = intermediate_variable_order++;
            int operand_2 = intermediate_variable_order++;
            int result_operand = intermediate_variable_order++;

            fprintf(f, "POP t%d\n", operand_1);
            fprintf(f, "POP t%d\n", operand_2);
            fprintf(f, "LT t%d, t%d, t%d\n", operand_2, operand_1, result_operand);
            break;
        }
        case GE:
        {
            Object left = ex(p->opr.op[0]);
            Object right = ex(p->opr.op[1]);
            int operand_1 = intermediate_variable_order++;
            int operand_2 = intermediate_variable_order++;
            int result_operand = intermediate_variable_order++;

            fprintf(f, "POP t%d\n", operand_1);
            fprintf(f, "POP t%d\n", operand_2);
            fprintf(f, "GE t%d, t%d, t%d\n", operand_2, operand_1, result_operand);
            break;
        }
        case LE:
        {
            Object left = ex(p->opr.op[0]);
            Object right = ex(p->opr.op[1]);
            int operand_1 = intermediate_variable_order++;
            int operand_2 = intermediate_variable_order++;
            int result_operand = intermediate_variable_order++;

            fprintf(f, "POP t%d\n", operand_1);
            fprintf(f, "POP t%d\n", operand_2);
            fprintf(f, "LE t%d, t%d, t%d\n", operand_2, operand_1, result_operand);
            break;
        }
        case NE:
        {
            Object left = ex(p->opr.op[0]);
            Object right = ex(p->opr.op[1]);
            int operand_1 = intermediate_variable_order++;
            int operand_2 = intermediate_variable_order++;
            int result_operand = intermediate_variable_order++;

            fprintf(f, "POP t%d\n", operand_1);
            fprintf(f, "POP t%d\n", operand_2);
            fprintf(f, "NE t%d, t%d, t%d\n", operand_1, operand_2, result_operand);
            break;
        }
        case EQ:
        {
            Object left = ex(p->opr.op[0]);
            Object right = ex(p->opr.op[1]);
            int operand_1 = intermediate_variable_order++;
            int operand_2 = intermediate_variable_order++;
            int result_operand = intermediate_variable_order++;

            fprintf(f, "POP t%d\n", operand_1);
            fprintf(f, "POP t%d\n", operand_2);
            fprintf(f, "EQ t%d, t%d, t%d\n", operand_1, operand_2, result_operand);
            break;
        }
        case AND:
        {
            Object left = ex(p->opr.op[0]);
            Object right = ex(p->opr.op[1]);
            int operand_1 = intermediate_variable_order++;
            int operand_2 = intermediate_variable_order++;
            int result_operand = intermediate_variable_order++;

            fprintf(f, "POP t%d\n", operand_1);
            fprintf(f, "POP t%d\n", operand_2);
            fprintf(f, "AND t%d, t%d, t%d\n", operand_1, operand_2, result_operand);
            break;
        }
        case OR:
        {
            Object left = ex(p->opr.op[0]);
            Object right = ex(p->opr.op[1]);
            int operand_1 = intermediate_variable_order++;
            int operand_2 = intermediate_variable_order++;
            int result_operand = intermediate_variable_order++;

            fprintf(f, "POP t%d\n", operand_1);
            fprintf(f, "POP t%d\n", operand_2);
            fprintf(f, "OR t%d, t%d, t%d\n", operand_1, operand_2, result_operand);
            break;
        }

        case NOT:
        {
            Object right = ex(p->opr.op[0]);
            int operand_1 = intermediate_variable_order++;
            int result_operand = intermediate_variable_order++;

            fprintf(f, "POP t%d\n", operand_1);
            fprintf(f, "NOT t%d, t%d\n", operand_1, result_operand);
            break;
        }

        case XOR:
        {
            Object left = ex(p->opr.op[0]);
            Object right = ex(p->opr.op[1]);
            int operand_1 = intermediate_variable_order++;
            int operand_2 = intermediate_variable_order++;
            int result_operand = intermediate_variable_order++;

            fprintf(f, "POP t%d\n", operand_1);
            fprintf(f, "POP t%d\n", operand_2);
            fprintf(f, "XOR t%d, t%d, t%d\n", operand_1, operand_2, result_operand);
            break;
        }
        }
    }
    return o;
}
