#ifndef _LOG_
#define _LOG_
#include <stdio.h>

#define LOG(message) printf("%s\n", message);

const char *LOG_FILE_NAME = "quadrubles.log";
const char *SYMBOL_TABLE_FILE_NAME = "symbol_table.log";

FILE *f = NULL;
FILE *symbol_table_log_file = NULL;

FILE *get_log()
{
    if (!f)
        f = fopen(LOG_FILE_NAME, "w");
    return f;
}

FILE *get_symbol_table_log_instance()
{
    if (!symbol_table_log_file)
        symbol_table_log_file = fopen(SYMBOL_TABLE_FILE_NAME, "w");
    return symbol_table_log_file;
}

void close_log()
{
    fclose(f);
    fclose(symbol_table_log_file);
}

#endif