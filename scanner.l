%option yylineno

%{
#include <stdlib.h>
#include<string.h>
#include<stdio.h>
#include "parser.h"
#include "parser.tab.h"
void yyerror(char *);
// [a-zA-Z_][a-zA-Z0-9_]* { // variable name
                // yylval.sIndex = *yytext - 'a';
                // return VARIABLE;
                // }
%}

%%
"true"          { // create instance of Object
                Object obj;
                obj.type = typeBool;
                obj.value = 1;
                yylval.dValue = obj;
                return TRUE;
                }
"false"         { // create instance of Object
                Object obj;
                obj.type = typeBool;
                obj.value = 0;
                yylval.dValue = obj;
                return FALSE;
                }

"const"        return CONST;
"if"            return IF;
"else"          return ELSE;
"switch"       return SWITCH;
"case"         return CASE;
"default"      return DEFAULT;


"print"         return PRINT;
"function"     return FUNCTION; 
"return"       return RETURN;

"enum"         return ENUM;

"while"         return WHILE;
"continue"     return CONTINUE;
"break"        return BREAK;
"for"          return FOR;
"do"           return DO;


[a-zA-Z_][a-zA-Z0-9_]* { // variable name
                // yylval.sIndex = *yytext - 'a';
                // entry in symbol table
                yylval.sValue = strdup(yytext);
                return VARIABLE;
                }

([0-9]*\.[0-9]+|[0-9]+\.[0-9]*)([eE][-+]?[0-9]+)?  { // removed [-+]? from beginning
                // yylval.fValue = atof(yytext);
                // create instance of Object
                Object obj;
                obj.type = typeFloat;
                obj.fvalue = atof(yytext);
                yylval.dValue = obj;
                // printf("string is %s\n",yytext);
                return FLOAT;
                }

0           {
                // yylval.iValue = atoi(yytext);
                // create instance of Object
                Object obj;
                obj.type = typeInt;
                obj.value = atoi(yytext);
                yylval.dValue = obj;
                return INTEGER;
            }
[1-9][0-9]* {
                // yylval.iValue = atoi(yytext);
                // create instance of Object
                Object obj;
                obj.type = typeInt;
                obj.value = atoi(yytext);
                yylval.dValue = obj;
                return INTEGER;
            }

\"(\\.|[^\\"])*\" {
                // return string without quotes
                char * tmp = strdup(yytext);
                tmp[strlen(yytext)-1] = '\0';
                tmp++;
                Object obj;
                obj.type = typeStr;
                obj.str = tmp;
                yylval.dValue = obj;
                return STRING;
                }
\'(\\.|[^\\'])*\' {// return string without quotes
                char* tmp = strdup(yytext);
                tmp[strlen(yytext)-1] = '\0';
                tmp++;
                Object obj;
                obj.type = typeStr;
                obj.str = tmp;
                yylval.dValue = obj;
                return STRING;
                }

[-()<>=+*/;{}.,:] {
                return *yytext;
             }

">="            return GE;
"<="            return LE;
"=="            return EQ;
"!="            return NE;
"&&"            return AND;
"||"            return OR;
"^"             return XOR;
"!"             return NOT;

"//".*          ;

[ \t\n]+        ;       /* ignore whitespace */

.               yyerror("Unknown character");
%%
int yywrap(void) {
    return 1;
}
