#ifndef TOKENS_H
#define TOKENS_H

#include "libraries.h"

//Token types
#define RETURN 1
#define IF 2
#define ELSE 3
#define WHILE 4
#define WRITE 5
#define READ 6
#define VOID 7
#define INT 8
#define LBRACE 9
#define RBRACE 10
#define LPARANTHESIS 11
#define RPARANTHESIS 12
#define SEMICOLON 13
#define COMMA 14
#define ASSIGNOP 15
#define PLUSOP 16
#define MINUSOP 17
#define MULTOP 18
#define DIVOP 19
#define NOTOP 20
#define EQOP 21
#define LTOP 22
#define LOEOP 23
#define ID 24
#define NUM 25

#define BUFFERSIZE 256

typedef struct token
{
    char *lexeme;
    unsigned __int8 type;
    unsigned __int32 attribute;
}TOKEN;

typedef struct token_node
{
    TOKEN* token;
    struct token_node* next;
}TOKENNODE;

TOKEN* createToken(char* lexeme, int type, int attr);
TOKEN* createEmptyToken();
void deleteToken(TOKEN** token);
TOKEN* tokenCopy(TOKEN* token);
void generateTokenList();
TOKENNODE* createTokenNode();
void appendTokenToList(TOKENNODE** node, TOKEN* token);

#endif