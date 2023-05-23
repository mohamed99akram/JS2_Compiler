#ifndef errorhandler_h
#define errorhandler_h

#include "parser.h"

void checkOperandExists(char* varname, int yylineno);

void checkFunctionExists(char* varname, int yylineno);

void checkUndefinedVar(int oper, nodeType* operand, int i, int yylineno);

void checkBooleanVariableType(nodeType* operand, int yylineno);

void checkNumericalOperandTypes(nodeType* operand, int yylineno);

void checkWrongOperandTypes(int oper, nodeType* operand, int i, int yylineno);

void checkAssigmentnOfConst(int oper, nodeType* operand, int i, int yylineno);

void checkUsedFunctionName(int oper, nodeType* operand, int i, int yylineno);

void checkConditionWarnings(int oper, nodeType* operand, int i, int yylineno);

void addNonUsedVar(Symbol* s);

void removeNonUsedVar(Symbol* s);

void checkNonUsedVars();

void red();

void yellow();

void cyan();

void green();

void purple();

void reset(); //reset print color

void yyerrorextended(char *s, int yylineno);

void compilationError();

void yywarning(char *s, int yylineno);

void errorlog(char *s, int yylineno);

#endif 
