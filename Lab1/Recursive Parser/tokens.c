#include "tokens.h"
#include "globalvars.h"

TOKEN* createToken(char* lexeme, int type, int attr)
{
    TOKEN* newToken = malloc(sizeof(TOKEN));

    if (newToken == NULL)
    {
        printf("Could not allocate memory for new token! \n");
        return NULL;
    }
    else
    {
        newToken->attribute = attr;
        newToken->lexeme = lexeme;
        newToken->type = type;
        return newToken;
    }
}

TOKEN* createEmptyToken()
{
    TOKEN* newToken = malloc(sizeof(TOKEN));

    if (newToken == NULL)
    {
        printf("Could not allocate memory for new token! \n");
        return NULL;
    }
    else
    {
        newToken->attribute = 0;
        newToken->lexeme = malloc(BUFFERSIZE);
        newToken->type = 0;
        return newToken;
    }
}

TOKEN* tokenCopy(TOKEN* token)
{
    TOKEN* copy = createEmptyToken();
    copy->attribute = token->attribute;
    strcpy(copy->lexeme, token->lexeme);
    copy->type = token->type;
    return copy;
}

void deleteToken(TOKEN** token)
{
    free((*token)->lexeme);
    free(*token);
    *token = NULL;
    return;
}

void generateTokenList()
{
    tokenList = NULL;
    TOKEN* token;

    //adding keywords to list
    token = createToken("return\0", RETURN, 0);
    appendTokenToList(&tokenList, token);
    
    token = createToken("if\0", IF, 0);
    appendTokenToList(&tokenList, token);

    token = createToken("else\0", ELSE, 0);
    appendTokenToList(&tokenList, token);

    token = createToken("while\0", WHILE, 0);
    appendTokenToList(&tokenList, token);

    token = createToken("write\0", WRITE, 0);
    appendTokenToList(&tokenList, token);

    token = createToken("read\0", READ, 0);
    appendTokenToList(&tokenList, token);

    token = createToken("void\0", VOID, 0);
    appendTokenToList(&tokenList, token);

    token = createToken("int\0", INT, 0);
    appendTokenToList(&tokenList, token);
}

TOKENNODE* createTokenNode()
{
    TOKENNODE* newNode = (TOKENNODE*)malloc(sizeof(TOKENNODE));
    newNode->token = NULL;
    newNode->next = NULL;
    return newNode;
}

void appendTokenToList(TOKENNODE** node, TOKEN* token)
{
    if (*node == NULL)
    {
        *node = createTokenNode();
        (*node)->token = token;
        (*node)->next = NULL;
    }
    else
    {
        appendTokenToList(&(*node)->next, token);
    }
}