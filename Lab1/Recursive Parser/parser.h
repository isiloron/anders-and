#ifndef PARSER_H
#define PARSER_H

#include "libraries.h"
#include "tokens.h"
#include "lexicalAnalyzer.h"
#include "globalvars.h"

//token handling
int peekOnNextToken();
int consumeToken();

//linus
int START();
int START_();
int TYPE();
void PARS();
void PARS_();
void BLOCK();
void BLOCK_();
void STMT();
void IDENT();
void ASSIGN();
void FUNCCALL();

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