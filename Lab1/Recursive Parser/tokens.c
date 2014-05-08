#include "tokens.h"

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
        printf("Could not allocate memory for new token!\n");
        return NULL;
    }
    else
    {
        newToken->lexeme = malloc(BUFFERSIZE);
        if (newToken->lexeme == NULL)
        {
            printf("Could not allocate memory for lexeme in token!\n");
            free(newToken);
            return NULL;
        }
        newToken->attribute = 0;
        newToken->type = 0;
        return newToken;
    }
}

TOKEN* tokenCopy(TOKEN* token)
{
    TOKEN* copy = createEmptyToken();
    if (copy == NULL)
    {
        printf("Failed to create empty token!\n");
        return NULL;
    }
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

int generateTokenList()
{
    tokenList = NULL;
    TOKEN* token;

    //adding keywords to list
    token = createToken("return\0", RETURN, 0);
    if (token == NULL)
    {
        printf("Failed to create token!\n");
        return EXIT_FAILURE;
    }
    if (appendTokenToList(&tokenList, token))
    {
        printf("Failed to append token to list!\n");
        return EXIT_FAILURE;
    }
    
    token = createToken("if\0", IF, 0);
    if (token == NULL)
    {
        printf("Failed to create token!\n");
        return EXIT_FAILURE;
    }
    if (appendTokenToList(&tokenList, token))
    {
        printf("Failed to append token to list!\n");
        return EXIT_FAILURE;
    }

    token = createToken("else\0", ELSE, 0);
    if (token == NULL)
    {
        printf("Failed to create token!\n");
        return EXIT_FAILURE;
    }
    if (appendTokenToList(&tokenList, token))
    {
        printf("Failed to append token to list!\n");
        return EXIT_FAILURE;
    }

    token = createToken("while\0", WHILE, 0);
    if (token == NULL)
    {
        printf("Failed to create token!\n");
        return EXIT_FAILURE;
    }
    if (appendTokenToList(&tokenList, token))
    {
        printf("Failed to append token to list!\n");
        return EXIT_FAILURE;
    }

    token = createToken("write\0", WRITE, 0);
    if (token == NULL)
    {
        printf("Failed to create token!\n");
        return EXIT_FAILURE;
    }
    if (appendTokenToList(&tokenList, token))
    {
        printf("Failed to append token to list!\n");
        return EXIT_FAILURE;
    }

    token = createToken("read\0", READ, 0);
    if (token == NULL)
    {
        printf("Failed to create token!\n");
        return EXIT_FAILURE;
    }
    if (appendTokenToList(&tokenList, token))
    {
        printf("Failed to append token to list!\n");
        return EXIT_FAILURE;
    }

    token = createToken("void\0", VOID, 0);
    if (token == NULL)
    {
        printf("Failed to create token!\n");
        return EXIT_FAILURE;
    }
    if (appendTokenToList(&tokenList, token))
    {
        printf("Failed to append token to list!\n");
        return EXIT_FAILURE;
    }

    token = createToken("int\0", INT, 0);
    if (token == NULL)
    {
        printf("Failed to create token!\n");
        return EXIT_FAILURE;
    }
    if (appendTokenToList(&tokenList, token))
    {
        printf("Failed to append token to list!\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

TOKENNODE* createTokenNode(TOKEN* token, TOKENNODE* node)
{
    TOKENNODE* newNode = (TOKENNODE*)malloc(sizeof(TOKENNODE));
    if (newNode == NULL)
    {
        printf("Could not allocate memory for token node!\n");
        return NULL;
    }
    newNode->token = token;
    newNode->next = node;
    return newNode;
}

int appendTokenToList(TOKENNODE** node, TOKEN* token)
{
    if (*node == NULL)
    {
        *node = createTokenNode(token,NULL);
        if (*node == NULL)
        {
            printf("Failed to create token node!\n");
            return EXIT_FAILURE;
        }
        else
            return EXIT_SUCCESS;
    }
    else
    {
        return appendTokenToList(&(*node)->next, token);
    }
}