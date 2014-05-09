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
int START();
int START_();
int TYPE();
int PARS();
int PARS_();
int BLOCK();
int BLOCK_();
int STMT();
int IDENT(TOKEN* idToken);

//skreven
int IFSTMT();
int IFSTMT_();
int VARDEC();
int ARGS();
int ARGS_();
int EXPR();
int EXPR_();
int SUM();
int SUM_();
int TERM();
int TERM_();
int TERM__(TOKEN* prev);


#endif