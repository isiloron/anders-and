#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include "main.h"

char matchNextChar();
char peekOnNextChar();
TOKEN* createEmptyToken();
TOKEN* getToken();
TOKEN* specialCharacter(char nextCharacter, TOKEN* newToken);

#endif


