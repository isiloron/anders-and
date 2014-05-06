#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include "libraries.h"
#include "tokens.h"
#include "globalvars.h"


char consumeNextChar();
char peekOnNextChar();
TOKEN* getNextToken();
TOKEN* specialCharacter();
TOKEN* getNumberToken();
char* getLexeme();
TOKEN* checkLexeme(TOKENNODE** node, char* lexeme);



#endif


