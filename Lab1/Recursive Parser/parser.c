#include "parser.h"



int START()
{
    //parse type (void or int)
    if (TYPE() == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }

    //parse identifier
    if (currentToken == NULL)
    {
        currentToken = getNextToken();
        if (currentToken == NULL)
        {
            printf("Could not get next token! Line: %d\n", lineNumber);
            return EXIT_FAILURE;
        }
    }
    if (currentToken->type == ID)
    {
        fprintf(filePtrDest, "[%s]\n", currentToken->lexeme);
    }
    else
    {
        printf("Parsing error! Expected identifier! Got '%s'.\n", currentToken->lexeme);
        return EXIT_FAILURE;
    }
    deleteToken(&currentToken);

    //parse left paranthesis
    currentToken = getNextToken();
    if (currentToken == NULL)
    {
        printf("Could not get next token! Line: %d\n", lineNumber);
        return EXIT_FAILURE;
    }
    if (currentToken->type == LPARANTHESIS)
    {
        deleteToken(&currentToken);
    }
    else
    {
        printf("Parsing error! Expected '('! Got '%s'.\n", currentToken->lexeme);
        return EXIT_FAILURE;
    }

    //parse parameters
    if (PARS() == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }

    //parse left paranthesis
    if (currentToken == NULL)
    {
        currentToken = getNextToken();
        if (currentToken == NULL)
        {
            printf("Could not get next token! Line: %d\n", lineNumber);
            return EXIT_FAILURE;
        }
    }
    if (currentToken->type == RPARANTHESIS)
    {
        deleteToken(&currentToken);
    }
    else
    {
        printf("Parsing error! Expected '('! Got '%s'.\n", currentToken->lexeme);
        return EXIT_FAILURE;
    }

    //parse block
    if (BLOCK() == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }

    //parse next function
    if (START_() == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int START_()
{
    if (currentToken == NULL)
    {
        currentToken = getNextToken();
        if (currentToken == NULL)
        {
            printf("Could not get next token! Line: %d\n", lineNumber);
            return EXIT_SUCCESS;
        }
        else
        {
            return START();
        }
    }
}

int TYPE()
{
    if (currentToken == NULL)
    {
        currentToken = getNextToken();
        if (currentToken == NULL)
        {
            printf("Could not get next token! Line: %d\n", lineNumber);
            return EXIT_FAILURE;
        }
    }
    
    switch (currentToken->type)
    {
    case INT:
        deleteToken(&currentToken);
        return EXIT_SUCCESS;
        break;
    case VOID:
        deleteToken(&currentToken);
        return EXIT_SUCCESS;
        break;
    default:
        printf("Parsing error! Expected 'int' or 'void'! Got '%s'. Line: %d\n", currentToken->lexeme, lineNumber);
        return EXIT_FAILURE;
    }
}


