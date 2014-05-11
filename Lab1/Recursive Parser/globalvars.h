#ifndef GLOBALVARS_H
#define GLOBALVARS_H

#include "libraries.h"
#include "tokens.h"

//file pointer for reading characters
FILE* filePtrSource;

// file pointer for stack machine code
FILE* filePtrDest;

//Current line
int lineNumber;

//lable counter
int lableCounter;

//local vars counter

int localVarsCounter;

//token list
TOKENNODE* tokenList;

//next token to be parsed
TOKEN* nextToken;

int prepareGlobalVars(char* fileName);

#endif

