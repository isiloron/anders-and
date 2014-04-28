#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include "main.h"


char lookAhead();
TOKEN* createToken();
FILE* openFile();
char peekOnNextChar(FILE* filePtr);

#endif


