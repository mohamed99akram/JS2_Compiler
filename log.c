#ifndef _LOG_
#define _LOG_
#include <stdio.h>

#define LOG(message) printf("%s\n", message);

const char *LOG_FILE_NAME = "quadrubles.log";
FILE *f = NULL;

FILE *get_log()
{
    if (!f)
        f = fopen(LOG_FILE_NAME, "w");
    return f;
}

void close_log()
{
    fclose(f);
}

#endif