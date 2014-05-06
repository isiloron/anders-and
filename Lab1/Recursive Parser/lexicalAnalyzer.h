#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include "main.h"

typedef struct token_list
{
	TOKEN* currToken;
	TOKEN* nextToken;
}TOKENLIST;

char consumeNextChar();
char peekOnNextChar();
TOKEN* createEmptyToken();
void deleteToken(TOKEN* token);
TOKEN* getNextToken(TOKENLIST* tokenList);
void specialCharacter(TOKEN* newToken);
void getNumberToken(TOKEN* newToken);
void getLexeme(char* string);
int lexemeIsKeyword(TOKEN* newToken);
int lexemeIsID(TOKEN* newToken);



#endif


