#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include "libraries.h"
#include "tokens.h"
#include "globalvars.h"

/*Reads characters from sorce-file, and assembles them into tokens. 
Returning ptr to next token */
TOKEN* readNextToken();

/*Move the file-ptr to the next char in the file*/
char consumeNextChar();

/*Look on the next char in file. 
File-ptr remains the same. */
char peekOnNextChar();

/*Look on the 2nd next char in file, from were the current file-ptr is pointing.
File-ptr remains the same. */
char peekOnNextNextChar();

/*Reading characters from sorce-file into <NUM> token*/
TOKEN* getNumberToken();

/*Reading characters from sorce-file that is not <ID> or <NUM> token*/
TOKEN* specialCharacter();

/*Reading characters from sorce-file into <ID> token*/
char* getLexeme();

/*Checking if <ID> token has been read allrdy*/
TOKEN* checkLexeme(TOKENNODE** node, char* lexeme);

#endif


