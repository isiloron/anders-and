#include "parser.h"

int getNextToken()
{
    if (nextToken == NULL)
    {
        nextToken = readNextToken();
        if (nextToken == NULL)
        {
            printf("No more tokens! Line: %d\n", lineNumber);
            return EXIT_FAILURE;
        }
        else
        {
            return EXIT_SUCCESS;
        }
    }
    else
    {
        return EXIT_SUCCESS;
    }
}

int consumeNextToken()
{
    deleteToken(&nextToken);
    if (nextToken == NULL)
    {
        return EXIT_SUCCESS;
    }
    else
    {
        printf("Could not consume token!\n");
        return EXIT_FAILURE;
    }
}

int START()
{
    //parse type (void or int)
    if (TYPE() == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }

    //parse identifier
    if (getNextToken() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if (nextToken->type == ID)
    {
        fprintf(filePtrDest, "[%s]\n", nextToken->lexeme);
    }
    else
    {
        printf("Parsing error! Expected identifier! Got '%s'.\n", nextToken->lexeme);
        return EXIT_FAILURE;
    }
    if (consumeNextToken() == EXIT_FAILURE)
        return EXIT_FAILURE;

    //parse left paranthesis
    if (getNextToken() == EXIT_FAILURE)
        return EXIT_FAILURE;
    if (nextToken->type == LPARANTHESIS)
    {
        deleteToken(&nextToken);
    }
    else
    {
        printf("Parsing error! Expected '('! Got '%s'.\n", nextToken->lexeme);
        return EXIT_FAILURE;
    }

    //parse parameters
    if (PARS() == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }

    //parse left paranthesis
    if (nextToken == NULL)
    {
        nextToken = readNextToken();
        if (nextToken == NULL)
        {
            printf("Could not get next token! Line: %d\n", lineNumber);
            return EXIT_FAILURE;
        }
    }
    if (nextToken->type == RPARANTHESIS)
    {
        deleteToken(&nextToken);
    }
    else
    {
        printf("Parsing error! Expected '('! Got '%s'.\n", nextToken->lexeme);
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
    if (nextToken == NULL)
    {
        nextToken = readNextToken();
        if (nextToken == NULL)
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
    if (nextToken == NULL)
    {
        nextToken = readNextToken();
        if (nextToken == NULL)
        {
            printf("Could not get next token! Line: %d\n", lineNumber);
            return EXIT_FAILURE;
        }
    }
    
    switch (nextToken->type)
    {
    case INT:
        deleteToken(&nextToken);
        return EXIT_SUCCESS;
        break;
    case VOID:
        deleteToken(&nextToken);
        return EXIT_SUCCESS;
        break;
    default:
        printf("Parsing error! Expected 'int' or 'void'! Got '%s'. Line: %d\n", nextToken->lexeme, lineNumber);
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
	if (nextToken == NULL)
		nextToken = readNextToken();

	switch (nextToken->type)
	{
	case ASSIGNOP:
		EXPR();
		VARDEC();
		fprintf(filePtrDest, "ASSINT\n");
		break;
	case COMMA:
		nextToken = readNextToken();
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
	if (nextToken == NULL)
		nextToken = readNextToken();
	
	if (nextToken == COMMA)
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
	if (nextToken == NULL)
		nextToken = readNextToken();

	switch (nextToken->type)
	{
	case ASSIGNOP:
		nextToken = NULL;
		EXPR();
		fprintf(filePtrDest, "ASSINT\n");
		break;
	case LTOP:
		nextToken = NULL;
		EXPR();
		fprintf(filePtrDest, "LTINT\n");
		break;
	case LOEOP:
		nextToken = NULL;
		EXPR();
		fprintf(filePtrDest, "LEINT\n");
		break;
	default:
		printf("Parser error on line %d ! Got '%s', but expected '==', '<' or '<=' operand. ", lineNumber, nextToken->lexeme);
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
	if (nextToken == NULL)
		nextToken = getNext();

	switch (nextToken->type)
	{
	case PLUSOP:
		nextToken = NULL;
		SUM();
		fprintf(filePtrDest, "ADD\n");
		break;
	case MINUSOP:
		nextToken = NULL;
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
	if (nextToken == NULL)
		nextToken = getNext();

	switch (nextToken->type)
	{
	case NUM:
		fprintf(filePtrDest, "PUSHINT %s \n", nextToken->attribute);
		nextToken = NULL;
		TERM_();
		break;
	case ID:
		TOKEN* id = malloc(sizeof(TOKEN));
		id = nextToken;
		nextToken = NULL;
		TERM__(id);
		break;
	case LPARANTHESIS:
		nextToken = NULL;
		EXPR();

		if (nextToken == NULL)
			nextToken = getNext();

		if (nextToken->type != RPARANTHESIS)
		{
			printf("Parser error! Missing right paranthesis on line %d", lineNumber);
			return EXIT_FAILURE;
		}
		break;
	case NOTOP:
		nextToken = NULL;
		TERM();
		fprintf(filePtrDest, "NOT \n");
		break;
	default:
		printf("Parser error on line %d ! Got '%s', but expected number, id, '(' or '!'.", lineNumber, nextToken->lexeme);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int TERM_()
{
	if (nextToken == NULL)
		nextToken = getNext();

	switch (nextToken->type)
	{
	case MULTOP:
		nextToken = NULL;
		TERM();
		break;
	case DIVOP:
		nextToken = NULL;
		TERM();
		break;
	default:
		return EXIT_SUCCESS;
	}
	return EXIT_SUCCESS;
}

int TERM__(TOKEN* prev)
{
	if (nextToken == NULL)
	{
		nextToken = getNext();
	}
	
	if (nextToken->type == LPARANTHESIS)
	{
		fprintf(filePtrDest, "DECL @\n");
		fprintf(filePtrDest, "PUSHINT ");
		ARGS();

		if (nextToken == NULL)
		{
			nextToken = getNext();
		}
		if (nextToken->type != RPARANTHESIS)
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
