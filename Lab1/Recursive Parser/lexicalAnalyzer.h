#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include "main.h"

char consumeNextChar();
char peekOnNextChar();
TOKEN* createEmptyToken();
TOKEN* getNextToken();
TOKEN* specialCharacter(TOKEN* newToken);
void getNumberToken(TOKEN* newToken);
void getLexeme(char* string);
int lexemeIsKeyword(TOKEN* newToken);
int lexemeIsID(TOKEN* newToken);



#endif


