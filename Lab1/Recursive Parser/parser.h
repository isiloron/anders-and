#ifndef PARSER_H
#define PARSER_H

#include "libraries.h"
#include "tokens.h"
#include "lexicalAnalyzer.h"
#include "globalvars.h"

//max label string size
#define LABELSIZE 16

//gets the next token and puts it in the global variable nextToken
int getNextToken();
//frees the global variable nextToken and sets it to NULL
void consumeNextToken();
//generates a label for use in if and while statements
char* newlabel();

//parsing functions, see grammar

int START_1();
int START_2();
int TYPE();
int PARS_1();
int PARS_2();
int BLOCK_1();
int BLOCK_2();
int STMT();
int IDENT(char* label);
int IFSTMT_1();
int IFSTMT_2();
int VARDEC();
int ARGS_1(char* label);
int ARGS_2(char* label);
int EXPR_1();
int EXPR_2();
int EXPR_3();
int EXPR_4();
int SUM_1();
int SUM_2();
int TERM_1();
int TERM_2();
int TERM_3(char* label);
int NOTOPERAND_1();
int NOTOPERAND_2(char* label);


#endif