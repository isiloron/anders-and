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
	peekOnNextToken();

	switch (currentToken->type)
	{
	case ELSE:
		consumeToken();
		IFSTMT_();
	default:
		return EXIT_SUCCESS;
	}
}

int IFSTMT_()
{
	peekOnNextToken();

	switch (currentToken->type)
	{
	case IF:
		consumeToken();
		peekOnNextToken();
		IFSTMT_();
	default:
		BLOCK();
	}
}

int VARDEC()
{
	peekOnNextToken();

	switch (nextToken->type)
	{
	case ASSIGNOP:
		consumeToken();
		EXPR();
		VARDEC();
		fprintf(filePtrDest, "ASSINT\n");
		break;
	case COMMA:
		consumeToken();
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
	peekOnNextToken();
	
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
	peekOnNextToken();

	switch (nextToken->type)
	{
	case ASSIGNOP:
		consumeToken();
		EXPR();
		fprintf(filePtrDest, "ASSINT\n");
		break;
	case LTOP:
		consumeToken();
		EXPR();
		fprintf(filePtrDest, "LTINT\n");
		break;
	case LOEOP:
		consumeToken();
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
	peekOnNextToken();

	switch (nextToken->type)
	{
	case PLUSOP:
		consumeToken();
		SUM();
		fprintf(filePtrDest, "ADD\n");
		break;
	case MINUSOP:
		consumeToken();
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
	peekOnNextToken();

	switch (nextToken->type)
	{
	case NUM:
		fprintf(filePtrDest, "PUSHINT %s \n", nextToken->attribute);
		consumeToken();
		TERM_();
		break;
	case ID:
		TOKEN* id = malloc(sizeof(TOKEN));
		id = nextToken;
		consumeToken();
		TERM__(id);
		break;
	case LPARANTHESIS:
		consumeToken();
		EXPR();
		peekOnNextToken();

		if (nextToken->type != RPARANTHESIS)
		{
			printf("Parser error! Missing right paranthesis on line %d", lineNumber);
			return EXIT_FAILURE;
		}
		break;
	case NOTOP:
		consumeToken();
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
	peekOnNextToken();

	switch (nextToken->type)
	{
	case MULTOP:
		consumeToken();
		TERM();
		break;
	case DIVOP:
		consumeToken();
		TERM();
		break;
	default:
		return EXIT_SUCCESS;
	}
	return EXIT_SUCCESS;
}

int TERM__(TOKEN* prev)
{
	peekOnNextToken();
	
	if (nextToken->type == LPARANTHESIS)
	{
		fprintf(filePtrDest, "DECL @\n");
		fprintf(filePtrDest, "PUSHINT ");
		ARGS();
		peekOnNextToken();

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
