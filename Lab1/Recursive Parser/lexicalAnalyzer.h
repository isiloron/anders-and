#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include "main.h"


char lookAhead(FILE* filePtr);
char match(FILE* filePtr);
char peekOnNextChar(FILE* filePtr);

#endif


