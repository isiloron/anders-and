#ifndef PARSER_H
#define PARSER_H

#include "libraries.h"
#include "tokens.h"
#include "lexicalAnalyzer.h"
#include "globalvars.h"

//max lable string size
#define LABLESIZE 16

//token handling
int getNextToken();
void consumeNextToken();
char* newLable();

//linus
int START_1();
int START_2();
int TYPE();
int PARS_1();
int PARS_2();
int BLOCK_1();
int BLOCK_2();
int STMT();
int IDENT(TOKEN* idToken);

//skreven
int IFSTMT_1();
int IFSTMT_2();
int VARDEC();
int ARGS_1(char* lable);
int ARGS_2(char* lable);
int EXPR_1();
int EXPR_2();
int EXPR_3();
int EXPR_4();
int SUM_1();
int SUM_2();
int TERM_1();
int TERM_2();
int TERM_3(char* lable);
int NOTOPERAND();


#endif