#include "parser.h"

int peekOnNextToken()
{
    if (currentToken == NULL)
    {
        currentToken = getNextToken();
        if (currentToken == NULL)
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

int consumeToken()
{
    deleteToken(&currentToken);
    if (currentToken == NULL)
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

	switch (currentToken->type)
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
	peekOnNextToken();

	switch (currentToken->type)
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
	peekOnNextToken();

	switch (currentToken->type)
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

	switch (currentToken->type)
	{
	case NUM:
		fprintf(filePtrDest, "PUSHINT %s \n", currentToken->attribute);
		consumeToken();
		TERM_();
		break;
	case ID:
		TOKEN* id = malloc(sizeof(TOKEN));
		id = currentToken;
		consumeToken();
		TERM__(id);
		break;
	case LPARANTHESIS:
		consumeToken();
		EXPR();
		peekOnNextToken();

		if (currentToken->type != RPARANTHESIS)
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
		printf("Parser error on line %d ! Got '%s', but expected number, id, '(' or '!'.", lineNumber, currentToken->lexeme);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int TERM_()
{
	peekOnNextToken();

	switch (currentToken->type)
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
	
	if (currentToken->type == LPARANTHESIS)
	{
		fprintf(filePtrDest, "DECL @\n");
		fprintf(filePtrDest, "PUSHINT ");
		ARGS();
		peekOnNextToken();

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
