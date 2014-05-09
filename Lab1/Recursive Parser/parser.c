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

int IFSTMT()
{

}

int IFSTMT_()
{

}

int VARDEC()
{
	if (currentToken == NULL)
		currentToken = getNextToken();

	switch (currentToken->type)
	{
	case ASSIGNOP:
		EXPR();
		VARDEC();
		fprintf(filePtrDest, "ASSINT\n");
		break;
	case COMMA:
		currentToken = getNextToken();
		VARDEC();
		break;
	default:
		return EXIT_SUCCESS;

	}
}

int ARGS()
{
	EXPR();
	ARGS_();
	return EXIT_SUCCESS;
}

int ARGS_()
{
	if (currentToken == NULL)
		currentToken = getNextToken();
	
	if (currentToken == COMMA)
	{
		EXPR();
		ARGS_();
	}
	else
		return EXIT_SUCCESS;
}


int EXPR()
{
	SUM();
	EXPR_();
	return EXIT_SUCCESS;
}

int EXPR_()
{	
	if (currentToken == NULL)
		currentToken = getNextToken();

	switch (currentToken->type)
	{
	case ASSIGNOP:
		currentToken = NULL;
		EXPR();
		fprintf(filePtrDest, "ASSINT\n");
		break;
	case LTOP:
		currentToken = NULL;
		EXPR();
		fprintf(filePtrDest, "LTINT\n");
		break;
	case LOEOP:
		currentToken = NULL;
		EXPR();
		fprintf(filePtrDest, "LEINT\n");
		break;
	default:
		printf("Parser error on line %d ! Got '%s', but expected '==', '<' or '<=' operand. ", lineNumber, currentToken->lexeme);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int SUM()
{
	TERM();
	SUM_();
	return EXIT_SUCCESS;
}

int SUM_()
{
	if (currentToken == NULL)
		currentToken = getNext();

	switch (currentToken->type)
	{
	case PLUSOP:
		currentToken = NULL;
		SUM();
		fprintf(filePtrDest, "ADD\n");
		break;
	case MINUSOP:
		currentToken = NULL;
		SUM();
		fprintf(filePtrDest, "SUB\n");
		break;
	default:
		return EXIT_SUCCESS;
	}
	return EXIT_SUCCESS;
}

int TERM()
{
	if (currentToken == NULL)
		currentToken = getNext();

	switch (currentToken->type)
	{
	case NUM:
		fprintf(filePtrDest, "PUSHINT %s \n", currentToken->attribute);
		currentToken = NULL;
		TERM_();
		break;
	case ID:
		TOKEN* id = malloc(sizeof(TOKEN));
		id = currentToken;
		currentToken = NULL;
		TERM__(id);
		break;
	case LPARANTHESIS:
		currentToken = NULL;
		EXPR();

		if (currentToken == NULL)
			currentToken = getNext();

		if (currentToken->type != RPARANTHESIS)
		{
			printf("Parser error! Missing right paranthesis on line %d", lineNumber);
			return EXIT_FAILURE;
		}
		break;
	case NOTOP:
		currentToken = NULL;
		TERM();
		fprintf(filePtrDest, "NOT \n");
		break;
	default:
		printf("Parser error on line %d ! Got '%s', but expected number, id, '(' or '!'.", lineNumber, currentToken->lexeme);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int TERM_()
{
	if (currentToken == NULL)
		currentToken = getNext();

	switch (currentToken->type)
	{
	case MULTOP:
		currentToken = NULL;
		TERM();
		break;
	case DIVOP:
		currentToken = NULL;
		TERM();
		break;
	default:
		return EXIT_SUCCESS;
	}
	return EXIT_SUCCESS;
}

int TERM__(TOKEN* prev)
{
	if (currentToken == NULL)
	{
		currentToken = getNext();
	}
	
	if (currentToken->type == LPARANTHESIS)
	{
		fprintf(filePtrDest, "DECL @\n");
		fprintf(filePtrDest, "PUSHINT ");
		ARGS();

		if (currentToken == NULL)
		{
			currentToken = getNext();
		}
		if (currentToken->type != RPARANTHESIS)
		{
			printf("Parser error! Missing right paranthesis on line %d", lineNumber);
			return EXIT_FAILURE;
		}
		else
		{
			fprintf(filePtrDest, "BSR %s \n", prev->lexeme);
			free(prev);
			return EXIT_SUCCESS;
		}
			
	}
	else
	{
		TERM_();
		return EXIT_SUCCESS;
	}
}
