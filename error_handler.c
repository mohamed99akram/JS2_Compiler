#ifndef errorhandler_c
#define errorhandler_c

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "parser.h"
#include "symbol_table.h"
#include <string.h>
#include "parser.tab.h"
#include "error_handler.h"


#define MAX_NO_VARS 100 //Number of variables to be tracked if is used or not
extern int currentNonUsedVarLength;
struct Symbol nonUsedVars[MAX_NO_VARS];

currentNonUsedVarLength = 0;

//add an item to nonUsedVars array
void addNonUsedVar(Symbol* s)
{
    // printf("Adding %s\n", s->name);
    // printf("Current length: %d\n", currentNonUsedVarLength);
    if (currentNonUsedVarLength < MAX_NO_VARS)
    {
        Symbol sym;
        sym.name = malloc(strlen(s->name) + 1);
        sym.name = s->name;
        sym.declaration_line = s->declaration_line;
        nonUsedVars[currentNonUsedVarLength] = sym;
        currentNonUsedVarLength++;
    }
}

//remove an item from nonUsedVars array
void removeNonUsedVar(Symbol* s)
{
    // printf("Removing %s\n", s->name);
    // printf("Current length: %d\n", currentNonUsedVarLength);
    int i;
    for (i = 0; i < currentNonUsedVarLength; i++)
    {
        if (strcmp(nonUsedVars[i].name, s->name) == 0)
        {
            int j;
            for (j = i; j < currentNonUsedVarLength - 1; j++)
            {
                nonUsedVars[j] = nonUsedVars[j + 1];
            }
            currentNonUsedVarLength--;
            break;
        }
    }
}

void checkNonUsedVars(){
    int i;
 //   printf("Unused variables: %d\n", currentNonUsedVarLength);
    for (i = 0; i < currentNonUsedVarLength; i++)
    {
        char str1[100] = "Unused variable: ";
        strcat(str1, nonUsedVars[i].name);
        yywarning(str1, nonUsedVars[i].declaration_line);
    }
}



void checkOperandExists(char* varname, int yylineno)
{
    SymbolTable* st = get_symbol_table_instance();
    if (st != NULL)
    {
        Symbol* sym = getSymbol(st, varname);;
        if (sym == NULL)
        {
            char str1[100] = "Undefined variable: ";
            strcat(str1, varname);
            yyerrorextended(str1, yylineno);
            compilationError();
        }
        else {
            if (sym->initialized == 0)
            {
                char str1[100] = "Uninitialized variable: ";
                strcat(str1, varname);
                yyerrorextended(str1, yylineno);
                compilationError();
            }
            removeNonUsedVar(sym);
        }
    }
    else 
    {
        yyerrorextended("Symbol table not initialized", yylineno);
        compilationError();
    }
}

void checkFunctionExists(char* varname, int yylineno)
{
    SymbolTable* st = get_symbol_table_instance();
    if (st != NULL)
    {
        Symbol* sym = getSymbol(st, varname);;
        if (sym == NULL)
        {
            char str1[100] = "Undefined Function: ";
            strcat(str1, varname);
            yyerrorextended(str1, yylineno);
            compilationError();
        }
    }
    else 
    {
        yyerrorextended("Symbol table not initialized", yylineno);
        compilationError();
    }
}

void checkUndefinedVar(int oper, nodeType* operand, int i, int yylineno)
{

    if (oper == '+' || oper == '-' || oper == '*' || oper == '/' || oper == '<' || oper == '>' || oper == GE || oper == LE
     || oper == NE || oper == EQ || oper == AND || oper == OR || oper == XOR || oper == PARAM_LIST || oper == PRINT || oper == NOT
     || (oper == '=' && i==1) || ((oper == WHILE || oper == IF) && i==0) //TODO IF not working
     )
     {
        if (operand != NULL && operand->type == typeId)
        {
            checkOperandExists(operand->id.varname, yylineno);
        }
     }
    if (oper == FUNCTION_CALL && i==0)
    {
        if (operand != NULL && operand->type == typeId)
        {
            checkFunctionExists(operand->id.varname, yylineno);
        }
    }
}

void checkBooleanVariableType(nodeType* operand, int yylineno){
    if (operand != NULL && operand->type == typeVal)
    {
        if (operand->val.type != typeBool)
        {
            char str1[] = "Expected boolean value, wrong operand type";
            yyerrorextended(str1, yylineno);
            compilationError();
        }
    }
    else if (operand != NULL && operand->type == typeId)  // check if it is identifier with type not bool
    {
        SymbolTable* st = get_symbol_table_instance();
        if (st != NULL)
        {
            Symbol* sym = getSymbol(st, operand->id.varname);;
            if (sym != NULL)
            {
                if (sym->data_type != typeBool)
                {
                    char str1[100] = "Expected boolean value, wrong operand type: ";
                    strcat(str1, operand->id.varname);
                    yyerrorextended(str1, yylineno);
                    compilationError();
                }
            }
        }
    }
}

void checkNumericalOperandTypes(nodeType* operand, int yylineno){
    if (operand != NULL && operand->type == typeVal)
    {
        if (operand->val.type != typeInt && operand->val.type != typeFloat)
        {
            char str1[] = "Expected numerial value, wrong operand type";
            yyerrorextended(str1, yylineno);
            compilationError();
        }
    }
    else if (operand != NULL && operand->type == typeId)  // check if it is identifier with type not int or float
    {
        SymbolTable* st = get_symbol_table_instance();
        if (st != NULL)
        {
            Symbol* sym = getSymbol(st, operand->id.varname);;
            if (sym != NULL)
            {
                if (sym->data_type != typeInt && sym->data_type != typeFloat)
                {
                    char str1[100] = "Expected numerial value, wrong operand type: ";
                    strcat(str1, operand->id.varname);
                    yyerrorextended(str1, yylineno);
                    compilationError();
                }
            }
        }
    }
}

void checkWrongOperandTypes(int oper, nodeType* operand, int i, int yylineno)
{
    if (oper == '+' || oper == '-' || oper == '*' || oper == '/' || oper == '<' || oper == '>' || oper == GE || oper == LE
     || oper == NE)
     {
        checkNumericalOperandTypes(operand, yylineno);
     }
     else if (oper == AND || oper == OR || oper == XOR || oper == NOT) // check type boolean
     {
        checkBooleanVariableType(operand, yylineno);
     }
}


void red() {
  printf("\033[1;31m");
}

void yellow() {
  printf("\033[1;33m");
}

void cyan() {
    printf("\033[0;36m");
}

void green() {
    printf("\033[0;32m");
}

void purple() {
    printf("\033[0;35m");
}

void reset() {
  printf("\033[0m");
}


void yyerrorextended(char *s, int yylineno) {
    red();
    printf("Error: ");
    reset();
    printf("line ");
    cyan();
    printf("%d ", yylineno);
    reset();
    printf(": %s \n", s);
    reset();
    char str1[100] = "Error: ";
    strcat(str1, s);
    errorlog(s, yylineno);
}

void compilationError(){
    red();
    printf("Compilation Error :( \n");
    reset();
    exit(1);
}

void yywarning(char *s, int yylineno) {
    purple();
    printf("Warning: ");
    reset();
    printf("line ");
    cyan();
    printf("%d ", yylineno);
    reset();
    printf(": %s \n", s);
    reset();
    char str1[100] = "Warning: ";
    strcat(str1, s);
    errorlog(str1, yylineno);
}

void errorlog(char *s, int yylineno){
    FILE *fp = fopen("errors.log", "a");
    if (fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    fprintf(fp, "line %d : %s \n", yylineno, s);
    fclose(fp);
}

void checkAssigmentnOfConst(int oper, nodeType* operand, int i, int yylineno){
    if(oper == '=' && i==0){
        SymbolTable* st = get_symbol_table_instance();
        if (st != NULL)
        {
            Symbol* sym = getSymbol(st, operand->id.varname);;
            if (sym != NULL)
            {
                if (sym->statement_type == typeConst)
                {
                    char str1[100] = "Cannot assign value to const variable: ";
                    strcat(str1, operand->id.varname);
                    yyerrorextended(str1, yylineno);
                    compilationError();
                }
            }
        }
        else 
        {
            yyerrorextended("Symbol table not initialized", yylineno);
            compilationError();
        }
    }
    
}

void checkUsedFunctionName(int oper, nodeType* operand, int i, int yylineno) {
    if (oper == FUNCTION_DECL && i==0)
    {
        SymbolTable* st = get_symbol_table_instance();
        if (st != NULL)
        {
            Symbol* sym = getSymbol(st, operand->id.varname);;
            if (sym != NULL)
            {
                char str1[100] = "Function identifier already used: ";
                strcat(str1, operand->id.varname);
                yyerrorextended(str1, yylineno);
                compilationError();
            }
        }
    }
    else 
    {
        yyerrorextended("Symbol table not initialized", yylineno);
        compilationError();
    }
}

void checkConditionWarnings(int oper, nodeType* operand, int i, int yylineno)
{

    if ((oper == IF || oper == WHILE) && i==0 && operand->type == typeVal)
    {
        if (operand->val.value == 1) // if (true)
        {
            if(oper==IF)
                yywarning("Condition is always true", yylineno);
            else if (oper==WHILE)
                yywarning("Infinite loop", yylineno);
        }
        else if (operand->val.value == 0) // if (false)
        {
            yywarning("Unreachable code", yylineno);
        }
    }
}

#endif 
