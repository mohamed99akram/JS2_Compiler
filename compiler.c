#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "parser.h"
#include "parser.tab.h"
#include "log.c"
#include "symbol_table.c"

int intermediate_variable_order = 0;
int jump_label_order = 0;

Object ex(nodeType *p, int yylineno, ...);

datatypeEnum getNumericalType(Object left, Object right)
{
    if (left.type == typeFloat || right.type == typeFloat)
    {
        return typeFloat;
    }
    else
    {
        return typeInt;
    }
}

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
    if (!p)
        return NULL;
    /// expand VAR_LIST to get linked list of variable names
    VarNameList *namesList = (VarNameList *)malloc(sizeof(VarNameList));
    namesList->head = (VarName *)malloc(sizeof(VarName));

    VarName *varName = namesList->head;
    while (p != NULL)
    {
        //printf("whileee\n");
        if (p->type == typeOpr && p->opr.oper == VAR_LIST)
        {
            printf("first if\n");
            varName->name = p->opr.op[0]->id.varname;
           printf("varName->name: %s\n", varName->name);
            varName->next = (VarName *)malloc(sizeof(VarName));
            varName = varName->next;
            p = p->opr.op[1];
        }
        else if (p->type == typeId)
        {
           // printf("second if\n");
            varName->name = p->id.varname;
          //  printf("varName->name: %s\n", varName->name);
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

/**
 * Get the intermediate variables resulted from
 * the expressions evaluation within the function
 * call body
 * @param p Root of the function call parsing tree
 * @return List of the intermediate variables
 */
VarNameList *getParamsNames(nodeType *p, int yylineno)
{
    if (!p)
        return NULL;

    VarNameList *namesList = (VarNameList *)malloc(sizeof(VarNameList));
    namesList->head = (VarName *)malloc(sizeof(VarName));
    VarName *varName = namesList->head;
    while (p)
    {
        if (p->type == typeOpr && (p->opr.oper == PARAM_LIST || p->opr.oper == EXPR_LIST))
        {
            varName->name = ex(p->opr.op[0], yylineno).str;
            varName->next = (VarName *)malloc(sizeof(VarName));
            varName = varName->next;
            p = p->opr.op[1];
        }
        else
        {
            varName->name = ex(p, yylineno).str;
            varName->next = NULL;
            break;
        }
    }


    //  switch (val.type)
    //     {
    //     case typeInt:
    //         fprintf(f, "PUSH %d\n", val.value);
    //         sprintf(val.str, "%d", val.value);
    //         break;
    //     case typeFloat:
    //         fprintf(f, "PUSH %f\n", val.fvalue);
    //         sprintf(val.str, "%d", val.fvalue);
    //         break;
    //     case typeStr:
    //         fprintf(f, "PUSH %s\n", val.str);
    //         sprintf(val.str, "%s", val.str);
    //         break;
    //     case typeBool:
    //         fprintf(f, "PUSH %d\n", val.value);
    //         sprintf(val.str, "%d", val.value);
    //         break;
    //     }

    return namesList;
}

Object ex(nodeType *p, int yylineno, ...)
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

        // return the result object
        return val;

        break;
    }
    case typeId:
    {
        Object result;
        //Get type of variable of name p->id.varname from symbol table
        Symbol *s = getSymbol(st, p->id.varname);
        if (s != NULL)
        {
            result.type = s->data_type;
        }

        result.str = strdup("1");
        fprintf(f, "PUSH %s\n", p->id.varname);
        sprintf(result.str, "%s", p->id.varname);
        return result;
        break;
    }
    case typeOpr:

        for (int i = 0; i < p->opr.nops; i++){

        //check if variable is defined
        checkUndefinedVar(p->opr.oper, p->opr.op[i], i, p->opr.op[i]->lineNo);

        //check if operand types are correct
        checkWrongOperandTypes(p->opr.oper, p->opr.op[i], i, p->opr.op[i]->lineNo);

        //check assign of const
        checkAssigmentnOfConst(p->opr.oper, p->opr.op[i], i, p->opr.op[i]->lineNo);

        //check condition warning
        checkConditionWarnings(p->opr.oper, p->opr.op[i], i, p->opr.op[i]->lineNo);

        }
        
        switch (p->opr.oper)
        {
        case VARIABLE_DECL:
        {
            Symbol *new_assign_symbol = createSymbol(p->opr.op[0]->id.varname, typeVar, typeInt, 0, 0, p->lineNo);
            insertSymbol(new_assign_symbol, st);
            printSymbolTable(st);
            fprintf(f, "PUSH 0\n"); //default value as garbage
            fprintf(f, "POP %s\n", p->opr.op[0]->id.varname);
            break;
        }
        case WHILE:
        {
            createScope(st);
            int label_1 = jump_label_order++;
            int label_2 = jump_label_order++;
            fprintf(f, "Label_%d:\n", label_1);
            ex(p->opr.op[0], yylineno);
            fprintf(f, "JF Label_%d\n", label_2);
            ex(p->opr.op[1], yylineno);
            fprintf(f, "JMP Label_%d\n", label_1);
            fprintf(f, "Label_%d:\n", label_2);
            deleteScope(st);
            break;
        }
        case FOR:
        {
            createScope(st);
            int label_1 = jump_label_order++;
            int label_2 = jump_label_order++;
            ex(p->opr.op[0], yylineno);
            fprintf(f, "Label_%d:\n", label_1);
            ex(p->opr.op[1], yylineno);
            fprintf(f, "JF Label_%d\n", label_2);
            ex(p->opr.op[3], yylineno);
            ex(p->opr.op[2], yylineno);
            fprintf(f, "JMP Label_%d\n", label_1);
            fprintf(f, "Label_%d:\n", label_2);
            deleteScope(st);
            break;
        }
        case DO:
        {
            createScope(st);
            int label = jump_label_order++;
            fprintf(f, "Label_%d\n", label);
            ex(p->opr.op[0], yylineno);
            ex(p->opr.op[1], yylineno);
            fprintf(f, "JT Label_%d\n", label);
            deleteScope(st);
            break;
        }
        case REPEAT:
        {
            createScope(st);
            int label = jump_label_order++;
            fprintf(f, "Label_%d\n", label);
            ex(p->opr.op[0], yylineno);
            ex(p->opr.op[1], yylineno);
            fprintf(f, "JF Label_%d\n", label);
            deleteScope(st);
            break;
        }

        case IF:
        {
            int label_order_1 = jump_label_order++;
            if (p->opr.nops == 2) // no else statement
            {
                createScope(st);
                ex(p->opr.op[0], yylineno);
                fprintf(f, "JF Label_%d\n", label_order_1);                
                ex(p->opr.op[1], yylineno);
                fprintf(f, "Label_%d:\n", label_order_1);
                deleteScope(st);
            }
            else
            {
                createScope(st);
                int label_order_2 = jump_label_order++;
                ex(p->opr.op[0], yylineno);
                fprintf(f, "JT Label_%d\n", label_order_1);
                ex(p->opr.op[2], yylineno);
                fprintf(f, "JMP Label_%d\n", label_order_2);
                fprintf(f, "Label_%d:\n", label_order_1);
                deleteScope(st);
                createScope(st);
                ex(p->opr.op[1], yylineno);
                fprintf(f, "Label_%d:\n", label_order_2);
                deleteScope(st);
            }
            break;
        }
        case SWITCH:
        {
            int int_var = intermediate_variable_order++;
            fprintf(f, "SWITCH_STATEMENT:\n");

            /*expression of switch*/
            ex(p->opr.op[0], yylineno);
            fprintf(f, "POP t%d\n", int_var);

            // instantiate an object to send it
            // to its case list hoping that
            // cases know the intermediate variable
            // to compare with
            char *switch_var;
            sprintf(switch_var, "t%d", int_var);

            // implement case statements
            ex(p->opr.op[1], yylineno, switch_var);
            // default block code
            ex(p->opr.op[2], yylineno);
            break;
        }
        case CASE:
        {
            createScope(st);
            va_list ap;
            va_start(ap, 1);
            char *switch_var = va_arg(ap, char *);
            int int_var_1 = intermediate_variable_order++;
            int int_var_2 = intermediate_variable_order++;
            int label = jump_label_order++;

            fprintf(f, "CASE:\n");
            // execute the case expresion
            ex(p->opr.op[0], yylineno);

            fprintf(f, "POP t%d\n", int_var_1);
            fprintf(f, "EQ t%d, %s, t%d\n", int_var_1, switch_var, int_var_2);
            fprintf(f, "JF Label_%d\n", label);

            // execute the inner statement
            ex(p->opr.op[1], yylineno);
            fprintf(f, "Label_%d\n", label);

            // if there is any another statements
            // send them the same intermediate variable
            ex(p->opr.op[2], switch_var);

            va_end(ap);
            deleteScope(st);
            break;
        }
        case DEFAULT:
        {
            createScope(st);
            ex(p->opr.op[0], yylineno);
            deleteScope(st);
            break;
        }
        case FUNCTION_DECL:
        {
            printf("FUNCTION_DECL\n");
            char *function_name = p->opr.op[0]->id.varname;
            int num_args = p->opr.nops;
            fprintf(f, "%s PROC ", function_name);
            ex(p->opr.op[1], p->opr.op[1]->lineNo);

            fprintf(f, "\n");
            //if (p->opr.op[2]!=NULL)
            ex(p->opr.op[2],yylineno);

            // check if there's any return value
            if (num_args == 4)
                ex(p->opr.op[3], yylineno);
            fprintf(f, "RET\n");

            deleteScope(st);

            break;
        }

        case FUNCTION_CALL:
        {
            idNodeType function_name_variable = p->opr.op[0]->id;
            char *function_name = function_name_variable.varname;
            VarNameList *arguements = getParamsNames(p->opr.op[1], yylineno);
            fprintf(f, "CALL %s", function_name);

            if (arguements)
            {
                // print space from the function name
                fprintf(f, " ");

                VarName *current = arguements->head;
                while (current)
                {
                    fprintf(f, "%s ", current->name);
                    current = current->next;
                }
            }
            fprintf(f, ",\n");

            break;
        }

        case PARAM_LIST:
        {
            LOG("hello from")
            break;
        }

        case VAR_LIST:
        {
            printf("VAR_LIST\n");
            VarNameList *args = getVarNames(p);
            VarName *arg = args ? args->head : NULL;
            createScope(st);

            while (arg)
            {
                fprintf(f, "%s, ", arg->name);

                Symbol *new_assign_symbol = createSymbol(arg->name, typeVar, typeInt, 0, 1, p->opr.op[0]->lineNo);
                insertSymbol(new_assign_symbol, st);
                printSymbolTable(st);

                arg = arg->next;
            }
        }
        // variable names sepearated by ',': used for enum, function_decl
                        // TODO : this is rubbish, it changes p's type to typeVarNameList !! look how enum is done, maybe repeat for function_decl

            // printf("VAR_LIST\n");
            // VarNameList *namesList = getVarNames(p);
            // VarName *varName = namesList->head;
            // p->type = typeVarNameList;
            // p->varNameList = namesList;
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
                Object val = ex(p->opr.op[0], yylineno);
                break;
            }

        case ';':;
            (ex(p->opr.op[0], yylineno));
            return (ex(p->opr.op[1], yylineno));
        case CONST:
        {
            Object returnedObject = ex(p->opr.op[1], yylineno);
            Symbol *new_assign_symbol = createSymbol(p->opr.op[0]->id.varname, typeConst, returnedObject.type, 0, 1, p->lineNo);
            insertSymbol(new_assign_symbol, st);
            printSymbolTable(st);
            ex(p->opr.op[1], yylineno);
            fprintf(f, "POP %s\n", p->opr.op[0]->id.varname);
            break;
        }
            // return createVar(p->opr.op[0]->id.varname, ex(p->opr.op[1], yylineno), typeConst);

        case ENUM:
        {
            // create variables for each enum value
            VarNameList *names_list = getVarNames(p->opr.op[0]);
            VarName *var_name = names_list->head;
            // enumerate their value
            int i = 0;
            while (var_name)
            {
                fprintf(f, "PUSH %d\n", i);
                fprintf(f, "POP %s\n", var_name->name);
                var_name = var_name->next;
                i++;
            }
            break;
        }
        case '=':
        {
    
            Object returnedObject = ex(p->opr.op[1], yylineno);

            Symbol *new_assign_symbol = createSymbol(p->opr.op[0]->id.varname, typeVar, returnedObject.type, 0, 1, p->lineNo);
            insertSymbol(new_assign_symbol, st);
            printSymbolTable(st);

            fprintf(f, "POP %s\n", p->opr.op[0]->id.varname);
            break;
        }

            // return createVar(p->opr.op[0]->id.varname, ex(p->opr.op[1], yylineno), typeVar);
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

        case '+':
        {
            Object left = ex(p->opr.op[0], yylineno);
            Object right = ex(p->opr.op[1], yylineno);
            Object result;
            int operand_1 = intermediate_variable_order++;
            int operand_2 = intermediate_variable_order++;
            int result_operand = intermediate_variable_order++;

            fprintf(f, "POP t%d\n", operand_1);
            fprintf(f, "POP t%d\n", operand_2);
            fprintf(f, "ADD t%d, t%d, t%d\n", operand_1, operand_2, result_operand);

            // return the intermediate result variable
            // in order to be used from if needed in above calls
            result.str = strdup("1");
            sprintf(result.str, "t%d", result_operand);
            result.type=getNumericalType(left, right);
            return result;

            break;
        }
        case '-':
        {
            Object left = ex(p->opr.op[0], yylineno);
            Object right = ex(p->opr.op[1], yylineno);
            Object result;
            int operand_1 = intermediate_variable_order++;
            int operand_2 = intermediate_variable_order++;
            int result_operand = intermediate_variable_order++;

            fprintf(f, "POP t%d\n", operand_1);
            fprintf(f, "POP t%d\n", operand_2);
            fprintf(f, "SUB t%d, t%d, t%d\n", operand_1, operand_2, result_operand);

            // return the intermediate result variable
            // in order to be used from if needed in above calls
            result.str = strdup("1");
            sprintf(result.str, "t%d", result_operand);
            result.type=getNumericalType(left, right);
            return result;

            break;
        }
        case '*':
        {
            Object left = ex(p->opr.op[0], yylineno);
            Object right = ex(p->opr.op[1], yylineno);
            Object result;
            int operand_1 = intermediate_variable_order++;
            int operand_2 = intermediate_variable_order++;
            int result_operand = intermediate_variable_order++;

            fprintf(f, "POP t%d\n", operand_1);
            fprintf(f, "POP t%d\n", operand_2);
            fprintf(f, "MUL t%d, t%d, t%d\n", operand_1, operand_2, result_operand);

            // return the intermediate result variable
            // in order to be used from if needed in above calls
            result.str = strdup("1");
            sprintf(result.str, "t%d", result_operand);
            result.type=getNumericalType(left, right);
            return result;

            break;
        }
        case '/':
        {
            Object left = ex(p->opr.op[0], yylineno);
            Object right = ex(p->opr.op[1], yylineno);
            Object result;
            int operand_1 = intermediate_variable_order++;
            int operand_2 = intermediate_variable_order++;
            int result_operand = intermediate_variable_order++;

            fprintf(f, "POP t%d\n", operand_1);
            fprintf(f, "POP t%d\n", operand_2);
            fprintf(f, "DIV t%d, t%d, t%d\n", operand_1, operand_2, result_operand);

            // return the intermediate result variable
            // in order to be used from if needed in above calls
            result.str = strdup("1");
            sprintf(result.str, "t%d", result_operand);
            result.type=getNumericalType(left, right);
            return result;

            break;
        }
        case '<':
        {
            Object left = ex(p->opr.op[0], yylineno);
            Object right = ex(p->opr.op[1], yylineno);
            Object result;
            int operand_1 = intermediate_variable_order++;
            int operand_2 = intermediate_variable_order++;
            int result_operand = intermediate_variable_order++;

            fprintf(f, "POP t%d\n", operand_1);
            fprintf(f, "POP t%d\n", operand_2);
            fprintf(f, "LT t%d, t%d, t%d\n", operand_2, operand_1, result_operand);

            // return the intermediate result variable
            // in order to be used from if needed in above calls
            result.str = strdup("1");
            sprintf(result.str, "t%d", result_operand);
            result.type = typeBool;
            return result;

            break;
        }
        case '>':
        {
            Object left = ex(p->opr.op[0], yylineno);
            Object right = ex(p->opr.op[1], yylineno);
            Object result;
            int operand_1 = intermediate_variable_order++;
            int operand_2 = intermediate_variable_order++;
            int result_operand = intermediate_variable_order++;

            fprintf(f, "POP t%d\n", operand_1);
            fprintf(f, "POP t%d\n", operand_2);
            fprintf(f, "GT t%d, t%d, t%d\n", operand_2, operand_1, result_operand);

            // return the intermediate result variable
            // in order to be used from if needed in above calls
            result.str = strdup("1");
            sprintf(result.str, "t%d", result_operand);
            result.type = typeBool;
            return result;

            break;
        }
        case GE:
        {
            Object left = ex(p->opr.op[0], yylineno);
            Object right = ex(p->opr.op[1], yylineno);
            Object result;
            int operand_1 = intermediate_variable_order++;
            int operand_2 = intermediate_variable_order++;
            int result_operand = intermediate_variable_order++;

            fprintf(f, "POP t%d\n", operand_1);
            fprintf(f, "POP t%d\n", operand_2);
            fprintf(f, "GE t%d, t%d, t%d\n", operand_2, operand_1, result_operand);

            // return the intermediate result variable
            // in order to be used from if needed in above calls
            result.str = strdup("1");
            sprintf(result.str, "t%d", result_operand);
            result.type = typeBool;
            return result;

            break;
        }
        case LE:
        {
            Object left = ex(p->opr.op[0], yylineno);
            Object right = ex(p->opr.op[1], yylineno);
            Object result;
            int operand_1 = intermediate_variable_order++;
            int operand_2 = intermediate_variable_order++;
            int result_operand = intermediate_variable_order++;

            fprintf(f, "POP t%d\n", operand_1);
            fprintf(f, "POP t%d\n", operand_2);
            fprintf(f, "LE t%d, t%d, t%d\n", operand_2, operand_1, result_operand);

            // return the intermediate result variable
            // in order to be used from if needed in above calls
            result.str = strdup("1");
            sprintf(result.str, "t%d", result_operand);
            result.type = typeBool;
            return result;

            break;
        }
        case NE:
        {
            Object left = ex(p->opr.op[0], yylineno);
            Object right = ex(p->opr.op[1], yylineno);
            Object result;
            int operand_1 = intermediate_variable_order++;
            int operand_2 = intermediate_variable_order++;
            int result_operand = intermediate_variable_order++;

            fprintf(f, "POP t%d\n", operand_1);
            fprintf(f, "POP t%d\n", operand_2);
            fprintf(f, "NE t%d, t%d, t%d\n", operand_1, operand_2, result_operand);

            // return the intermediate result variable
            // in order to be used from if needed in above calls
            result.str = strdup("1");
            sprintf(result.str, "t%d", result_operand);
            result.type = typeBool;

            return result;

            break;
        }
        case EQ:
        {
            Object left = ex(p->opr.op[0], yylineno);
            Object right = ex(p->opr.op[1], yylineno);
            Object result;
            int operand_1 = intermediate_variable_order++;
            int operand_2 = intermediate_variable_order++;
            int result_operand = intermediate_variable_order++;

            fprintf(f, "POP t%d\n", operand_1);
            fprintf(f, "POP t%d\n", operand_2);
            fprintf(f, "EQ t%d, t%d, t%d\n", operand_1, operand_2, result_operand);

            // return the intermediate result variable
            // in order to be used from if needed in above calls
            result.str = strdup("1");
            sprintf(result.str, "t%d", result_operand);
            result.type = typeBool;
            return result;

            break;
        }
        case AND:
        {
            Object left = ex(p->opr.op[0], yylineno);
            Object right = ex(p->opr.op[1], yylineno);
            Object result;
            int operand_1 = intermediate_variable_order++;
            int operand_2 = intermediate_variable_order++;
            int result_operand = intermediate_variable_order++;

            fprintf(f, "POP t%d\n", operand_1);
            fprintf(f, "POP t%d\n", operand_2);
            fprintf(f, "AND t%d, t%d, t%d\n", operand_1, operand_2, result_operand);

            // return the intermediate result variable
            // in order to be used from if needed in above calls
            result.str = strdup("1");
            sprintf(result.str, "t%d", result_operand);
            result.type = typeBool;
            return result;

            break;
        }
        case OR:
        {
            Object left = ex(p->opr.op[0], yylineno);
            Object right = ex(p->opr.op[1], yylineno);
            Object result;
            int operand_1 = intermediate_variable_order++;
            int operand_2 = intermediate_variable_order++;
            int result_operand = intermediate_variable_order++;

            fprintf(f, "POP t%d\n", operand_1);
            fprintf(f, "POP t%d\n", operand_2);
            fprintf(f, "OR t%d, t%d, t%d\n", operand_1, operand_2, result_operand);

            // return the intermediate result variable
            // in order to be used from if needed in above calls
            result.str = strdup("1");
            sprintf(result.str, "t%d", result_operand);
            result.type = typeBool;
            return result;

            break;
        }

        case NOT:
        {
            Object right = ex(p->opr.op[0], yylineno);
            Object result;
            int operand_1 = intermediate_variable_order++;
            int result_operand = intermediate_variable_order++;

            fprintf(f, "POP t%d\n", operand_1);
            fprintf(f, "NOT t%d, t%d\n", operand_1, result_operand);

            // return the intermediate result variable
            // in order to be used from if needed in above calls
            result.str = strdup("1");
            sprintf(result.str, "t%d", result_operand);
            result.type = typeBool;
            return result;

            break;
        }

        case XOR:
        {
            Object left = ex(p->opr.op[0], yylineno);
            Object right = ex(p->opr.op[1], yylineno);
            Object result;
            int operand_1 = intermediate_variable_order++;
            int operand_2 = intermediate_variable_order++;
            int result_operand = intermediate_variable_order++;

            fprintf(f, "POP t%d\n", operand_1);
            fprintf(f, "POP t%d\n", operand_2);
            fprintf(f, "XOR t%d, t%d, t%d\n", operand_1, operand_2, result_operand);

            // return the intermediate result variable
            // in order to be used from if needed in above calls
            result.str = strdup("1");
            sprintf(result.str, "t%d", result_operand);
            result.type = typeBool;
            return result;

            break;
        }
        }
    }
    return o;
}
