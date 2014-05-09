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
	if (getNextToken() == EXIT_FAILURE)
	{
		printf("Parser error! Could not get next token. ");
		return EXIT_FAILURE;
	}
	
	if (nextToken->type == ELSE)
	{
		consumeNextToken();
		if (IFSTMT_() == EXIT_FAILURE)
			return EXIT_FAILURE;
	}
	else
	{
		return EXIT_SUCCESS;
	}
	return EXIT_SUCCESS;
}

int IFSTMT_()
{
	if (getNextToken() == EXIT_FAILURE)
	{
		printf("Parser error! Could not get next token. ");
		return EXIT_FAILURE;
	}

	if (nextToken->type == IF)
	{
		consumeNextToken();
		if (getNextToken() == EXIT_FAILURE)
		{
			printf("Parser error! Could not get next token. ");
			return EXIT_FAILURE;
		}

		if (nextToken->type == LPARANTHESIS)
		{
			consumeNextToken();
			if (EXPR() == EXIT_FAILURE)
				return EXIT_FAILURE;
		}
		else
		{
			printf("Parser error! Missing left paranthesis on line %d", lineNumber);
			return EXIT_FAILURE;
		}
		
		if (nextToken->type == RPARANTHESIS)
		{
			consumeNextToken();
			if (BLOCK() == EXIT_FAILURE)
				return EXIT_FAILURE;

			if (IFSTMT() == EXIT_FAILURE)
				return EXIT_FAILURE;
		}
	}
	else
	{
		if (BLOCK() == EXIT_FAILURE)
			return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int VARDEC()
{
	if (getNextToken() == EXIT_FAILURE)
	{
		printf("Parser error! Could not get next token. ");
		return EXIT_FAILURE;
	}

	switch (nextToken->type)
	{
	case ASSIGNOP:
		consumeNextToken();
		if (EXPR() == EXIT_FAILURE)
			return EXIT_FAILURE;

		if(VARDEC() == EXIT_FAILURE)
			return EXIT_FAILURE;

		fprintf(filePtrDest, "ASSINT\n");
		break;
	case COMMA:
		consumeNextToken();
		if(VARDEC() == EXIT_FAILURE)
			return EXIT_FAILURE;
		break;
	default:
		return EXIT_SUCCESS;
	}
}

int ARGS()
{
	if(EXPR() == EXIT_FAILURE)
		return EXIT_FAILURE;

	if(ARGS_() == EXIT_FAILURE)
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

int ARGS_()
{
	if (getNextToken() == EXIT_FAILURE)
	{
		printf("Parser error! Could not get next token. ");
		return EXIT_FAILURE;
	}
	
	if (nextToken == COMMA)
	{
		consumeNextToken();
		if (EXPR() == EXIT_FAILURE)
			return EXIT_FAILURE;

		if (ARGS_() == EXIT_FAILURE)
			return EXIT_FAILURE;
	}
	else
		return EXIT_SUCCESS;
}


int EXPR()
{
	if (SUM() == EXIT_FAILURE)
		return EXIT_FAILURE;

	if (EXPR_() == EXIT_FAILURE)
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

int EXPR_()
{	
	if (getNextToken() == EXIT_FAILURE)
	{
		printf("Parser error! Could not get next token. ");
		return EXIT_FAILURE;
	}

	switch (nextToken->type)
	{
	case ASSIGNOP:
		consumeNextToken();
		if (EXPR() == EXIT_FAILURE)
			return EXIT_FAILURE;

		fprintf(filePtrDest, "ASSINT\n");
		break;
	case LTOP:
		consumeNextToken();
		if (EXPR() == EXIT_FAILURE)
			return EXIT_FAILURE;

		fprintf(filePtrDest, "LTINT\n");
		break;
	case LOEOP:
		consumeNextToken();
		if (EXPR() == EXIT_FAILURE)
			return EXIT_FAILURE;

		fprintf(filePtrDest, "LEINT\n");
		break;
	default:
		return EXIT_SUCCESS;
	}

	return EXIT_SUCCESS;
}

int SUM()
{
	if (TERM() == EXIT_FAILURE)
		return EXIT_FAILURE;

	if (SUM_() == EXIT_FAILURE)
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

int SUM_()
{
	if (getNextToken() == EXIT_FAILURE)
	{
		printf("Parser error! Could not get next token. ");
		return EXIT_FAILURE;
	}

	switch (nextToken->type)
	{
	case PLUSOP:
		consumeNextToken();
		if (SUM_() == EXIT_FAILURE)
			return EXIT_FAILURE;

		fprintf(filePtrDest, "ADD\n");
		break;
	case MINUSOP:
		consumeNextToken();
		if (SUM_() == EXIT_FAILURE)
			return EXIT_FAILURE;

		fprintf(filePtrDest, "SUB\n");
		break;
	default:
		return EXIT_SUCCESS;
	}
	return EXIT_SUCCESS;
}

int TERM()
{
	if (getNextToken() == EXIT_FAILURE)
	{
		printf("Parser error! Could not get next token. ");
		return EXIT_FAILURE;
	}

	switch (nextToken->type)
	{
	case NUM:
		fprintf(filePtrDest, "PUSHINT %s \n", nextToken->attribute);
		consumeNextToken();
		if (TERM_() == EXIT_FAILURE)
			return EXIT_FAILURE;

		break;
	case ID:
		TOKEN* id = malloc(sizeof(TOKEN));
		id = nextToken;
		consumeNextToken();
		if (TERM_(id) == EXIT_FAILURE)
			return EXIT_FAILURE;

		break;
	case LPARANTHESIS:
		consumeNextToken();
		if (EXPR() == EXIT_FAILURE)
			return EXIT_FAILURE;

		if (getNextToken() == EXIT_FAILURE)
		{
			printf("Parser error! Could not get next token. ");
			return EXIT_FAILURE;
		}

		if (nextToken->type != RPARANTHESIS)
		{
			printf("Parser error! Missing right paranthesis on line %d", lineNumber);
			return EXIT_FAILURE;
		}
		break;
	case NOTOP:
		consumeNextToken();
		if (TERM() == EXIT_FAILURE)
			return EXIT_FAILURE;
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
	if (getNextToken() == EXIT_FAILURE)
	{
		printf("Parser error! Could not get next token. ");
		return EXIT_FAILURE;
	}

	switch (nextToken->type)
	{
	case MULTOP:
		consumeNextToken();
		if (TERM() == EXIT_FAILURE)
			return EXIT_FAILURE;

		fprintf(filePtrDest, "MULT\n");
		break;
	case DIVOP:
		consumeNextToken();
		if (TERM() == EXIT_FAILURE)
			return EXIT_FAILURE;

		fprintf(filePtrDest, "DIV\n");
		break;
	default:
		return EXIT_SUCCESS;
	}
	return EXIT_SUCCESS;
}

int TERM__(TOKEN* prev)
{
	if (getNextToken() == EXIT_FAILURE)
	{
		printf("Parser error! Could not get next token. ");
		return EXIT_FAILURE;
	}
	
	if (nextToken->type == LPARANTHESIS)
	{
		consumeNextToken();
		fprintf(filePtrDest, "DECL @\n");
		fprintf(filePtrDest, "PUSHINT ");

		if (ARGS() == EXIT_FAILURE)
			return EXIT_FAILURE;

		if (getNextToken() == EXIT_FAILURE)
		{
			printf("Parser error! Could not get next token. ");
			return EXIT_FAILURE;
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
		if (TERM_() == EXIT_FAILURE)
			return EXIT_FAILURE;

		return EXIT_SUCCESS;
	}
}
