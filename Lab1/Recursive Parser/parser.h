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
void IFSTMT();
void IFSTMT_();
void IFSTMT__();
void WHILESTMT();
void VARDEC();
void VARDEC_();
void ARGS();
void ARGS_();
void EXPR();
void EXPR_();
void SUM();
void SUM_();
void TERM();
void TERM_();
void TERM__();


#endif