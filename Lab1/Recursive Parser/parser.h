#ifndef PARSER_H
#define PARSER_H

#include "libraries.h"
#include "tokens.h"
#include "lexicalAnalyzer.h"
#include "globalvars.h"

//token handling
int getNextToken();
int consumeNextToken();

//linus
int START();
int START_();
int TYPE();
int PARS();
int PARS_();
int BLOCK();
int BLOCK_();
int STMT();
int IDENT();

//skreven
int IFSTMT();
int IFSTMT_();
int IFSTMT__();
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