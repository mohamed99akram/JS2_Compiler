%{
#include <stdlib.h>
#include "parser.h"
#include "parser.tab.h"
void yyerror(char *);
// [a-zA-Z_][a-zA-Z0-9_]* { // variable name
                // yylval.sIndex = *yytext - 'a';
                // return VARIABLE;
                // }
%}

%%
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
                yylval.sIndex = *yytext - 'a';
                return VARIABLE;
                }

0           {
                yylval.iValue = atoi(yytext);
                return INTEGER;
            }
\"(\\.|[^\\"])*\" {
                // return string without quotes
                yylval.sValue = strdup(yytext);
                yylval.sValue[strlen(yytext)-1] = '\0';
                yylval.sValue++;
                return STRING;
                }
\'(\\.|[^\\'])*\' {
                yylval.sValue = strdup(yytext);
                yylval.sValue[strlen(yytext)-1] = '\0';
                yylval.sValue++;
                return STRING;
                }
[1-9][0-9]* {
                yylval.iValue = atoi(yytext);
                return INTEGER;
            }

[-()<>=+*/;{}.,:] {
                return *yytext;
             }

">="            return GE;
"<="            return LE;
"=="            return EQ;
"!="            return NE;

"//".*          ;

[ \t\n]+        ;       /* ignore whitespace */

.               yyerror("Unknown character");
%%
int yywrap(void) {
    return 1;
}
