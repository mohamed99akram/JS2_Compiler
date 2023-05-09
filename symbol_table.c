#include "symbol_table.h"

void printSymbolTable(SymbolTable *s)
{
    char *filename = "symbol_table.log";
    FILE *fp = fopen(filename, "w");

    if (fp == NULL)
    {
        printf("Could not open file %s", filename);
        return;
    }

    if (!s)
    {
        printf("Unvalid symbol table, table not exist");
        return;
    }

    _symbolTable *currentTable = s->currentTable;

    while (currentTable)
    {
        fprintf(fp, "Table scope id: %d\n", currentTable->id);
        fprintf(fp, "name\ttype\tdeclaration line\t\n");
        printSubSymbolTable(currentTable, fp);
    }
    fclose(fp);
}

void printSubSymbolTable(_symbolTable *symbol_table, FILE *f)
{
    if (!symbol_table)
        return;

    Symbol *symbol = symbol_table->head;
    while (symbol)
    {
        fprintf(f, "%s\t%d", symbol->name, symbol->declaration_line);
        symbol = symbol->next;
    }
}

_symbolTable *createScope(SymbolTable *st)
{
    _symbolTable *new_st = (_symbolTable *)malloc(sizeof(_symbolTable));
    new_st->next = st->currentTable;
    st->currentTable = new_st;
    return new_st;
}

_symbolTable *deleteScope(SymbolTable *st)
{
    // if(!st->currentTable)
    // return NULL;

    // _symbolTable* to_be_deleted = st->currentTable;
    // st->currentTable = st->currentTable->next;
    // free(to_be_deleted);
    // return st->currentTable;
}

Symbol *getSymbol(SymbolTable *s, char *name)
{
    // name: name of the symbol
    // search the symbol table for the symbol and return it
    // TODO if the symbol is not found, error
    if (symbolTable == NULL)
    {
        return NULL;
    }

    _symbolTable *currentTable = s->currentTable;

    while (currentTable)
    {
        Symbol *searchSymbol = getSymbolFromSymbolTable(currentTable, name);
        if (searchSymbol)
            return searchSymbol;
        currentTable = currentTable->next;
    }
    return NULL;
}

Symbol *getSymbolFromSymbolTable(_symbolTable *st, char *name)
{
    Symbol *s = st->head;

    while (s)
    {
        if (strcmp(name, s->name))
            return s;
        s = s->next;
    }
    return NULL;
}

Symbol *insertSymbol(Symbol *s, SymbolTable *st)
{
    // varname: name of the variable
    // val: value of the variable
    // type: type of the variable (variable, const, enum)
    if (!st->currentTable)
    {
        _symbolTable *new_st = createScope(st);
        new_st->next = NULL;
        new_st->head = s;
        new_st->id = 1;
        return new_st;
    }

    Symbol *search_symbol = getSymbolFromSymbolTable(st->currentTable, s->name);
    if (search_symbol)
    {
        search_symbol->value = s->value;
        search_symbol->type = s->type;
        search_symbol->params = s->params;
        search_symbol->paramNames = s->paramNames;
        search_symbol->operations = s->operations;
        return search_symbol;
    }
    else
    {
        _symbolTable *current_table = st->currentTable;
        Symbol *insertion_symbol = current_table->head;
        if (!insertion_symbol)
        {
            insertion_symbol = s;
            insertion_symbol->next = NULL;
            return insertion_symbol;
        }

        while (insertion_symbol->next)
            ;
        insertion_symbol->next = s;
        return insertion_symbol;
    }
}