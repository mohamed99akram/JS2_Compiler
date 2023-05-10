#ifndef _SYMBOL_TABLE_
#define _SYMBOL_TABLE_

#include "symbol_table.h"
#include "log.c"

SymbolTable *_symbol_table = NULL;

SymbolTable *get_symbol_table_instance()
{
    if (!_symbol_table)
    {
        _symbol_table = (SymbolTable *)malloc(sizeof(SymbolTable));
        _symbol_table->currentTable = NULL;
    }

    return _symbol_table;
}

void free_symbol_table_instance()
{
    free(_symbol_table);
}

void printSymbolTable(SymbolTable *s)
{
    FILE *fp = get_symbol_table_log_instance();

    if (!s)
    {
        printf("Unvalid symbol table, table not exist");
        return;
    }

    _symbolTable *currentTable = s->currentTable;

    while (currentTable)
    {
        fprintf(fp, "Table scope id: %d\n", currentTable->id);
        fprintf(fp, "Name\tKind\tType\tArgs\tInitialized\tline\t\n");
        printSubSymbolTable(currentTable, fp);
        currentTable = currentTable->next;
    }
}

void printSubSymbolTable(_symbolTable *symbol_table, FILE *f)
{
    if (!symbol_table)
        return;

    Symbol *symbol = symbol_table->head;
    while (symbol)
    {
        fprintf(f, "%s\t", symbol->name);
        fprintf(f, "%d\t", symbol->statement_type);
        fprintf(f, "%d\t", symbol->data_type);
        fprintf(f, "%d\t", symbol->initialized);
        fprintf(f, "%d\n", symbol->declaration_line);
        symbol = symbol->next;
    }
}

_symbolTable *createScope(SymbolTable *st)
{
    _symbolTable *new_st = (_symbolTable *)malloc(sizeof(_symbolTable));
    new_st->next = st->currentTable;
    new_st->head = NULL;
    if (st->currentTable)
    {
        _symbolTable *current_table = st->currentTable;
        new_st->id = current_table->id + 1;
    }
    else
        new_st->id = 1;
    st->currentTable = new_st;

    return new_st;
}

void free_scope_space(_symbolTable *st)
{
    // LOG("10")
    if (!st->head)
        return;
    // LOG("11")
    Symbol *current_symbol = st->head;
    // LOG("12")
    while (current_symbol)
    {
        // LOG("14")
        Symbol *next = current_symbol->next;
        // LOG("15")
        free(current_symbol);
        // LOG("16")
        current_symbol = next;
        // LOG("17")
    }
    // LOG("13")
}

void free_symbol_table_space()
{
    // LOG("3")
    if (!_symbol_table)
        return;
    // LOG("4")
    _symbolTable *current_table = _symbol_table->currentTable;
    // LOG("5")
    while (current_table)
    {
        // LOG("6")
        free_scope_space(current_table);
        // LOG("7")
        _symbolTable *next_table = current_table->next;
        // LOG("8")
        free(current_table);
        current_table = next_table;
    }
    // LOG("9")
}

_symbolTable *deleteScope(SymbolTable *st)
{
    if (!st->currentTable)
        return NULL;

    _symbolTable *to_be_deleted = st->currentTable;
    st->currentTable = to_be_deleted->next;
    free_scope_space(to_be_deleted);
    return st->currentTable;
}

Symbol *getSymbol(SymbolTable *s, char *name)
{
    // name: name of the symbol
    // search the symbol table for the symbol and return it
    // TODO if the symbol is not found, error
    if (!s->currentTable)
        return NULL;

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
        if (strcmp(name, s->name) == 0)
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
        // LOG("after creating the scope")
        new_st->head = s;
        // LOG("1")
        return s;
    }
    // LOG("after exiting the checking")
    Symbol *search_symbol = getSymbolFromSymbolTable(st->currentTable, s->name);
    // LOG("3");
    if (search_symbol)
    {
        // LOG("4");
        search_symbol->statement_type = s->statement_type;
        search_symbol->data_type = s->data_type;
        search_symbol->num_args = s->num_args;
        search_symbol->initialized = s->initialized;
        search_symbol->declaration_line = s->declaration_line;
        return search_symbol;
    }
    else
    {
        // LOG("5");
        _symbolTable *current_table = st->currentTable;
        Symbol *insertion_symbol = current_table->head;
        if (!insertion_symbol)
        {
            current_table->head = s;
            return current_table->head;
        }

        while (insertion_symbol->next)
        {
            insertion_symbol = insertion_symbol->next;
        }

        // LOG("6")
        insertion_symbol->next = s;
        return insertion_symbol;
    }
}

Symbol *createSymbol(char *name, symbolType stmt_type, datatypeEnum data_type, int num_args, int initialized, int declare_line)
{
    Symbol *new_symbol = (Symbol *)malloc(sizeof(Symbol));
    new_symbol->name = name;
    new_symbol->statement_type = stmt_type;
    new_symbol->data_type = data_type;
    new_symbol->num_args = num_args;
    new_symbol->initialized = initialized;
    new_symbol->declaration_line = declare_line;
    new_symbol->next = NULL;
    return new_symbol;
}

#endif