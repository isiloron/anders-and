#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include "libraries.h"
#include "tokens.h"
#include "globalvars.h"

TOKEN* readNextToken();
char consumeNextChar();
char peekOnNextChar();
char peekOnNextNextChar();
TOKEN* getNumberToken();
TOKEN* specialCharacter();
char* getLexeme();
TOKEN* checkLexeme(TOKENNODE** node, char* lexeme);



#endif


