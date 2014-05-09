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

void consumeNextToken()
{
    deleteToken(&nextToken);
}

char* newLable()
{
    char* lable = malloc(LABLESIZE);
    sprintf(lable, "Lable%d", lableCounter);
    lableCounter++;
    return lable;
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
    {
        return EXIT_FAILURE;
    }
    if (nextToken->type == ID)
    {
        fprintf(filePtrDest, "[%s]\n", nextToken->lexeme);
    }
    else
    {
        printf("Parsing error! Expected identifier! Got '%s'.\n", nextToken->lexeme);
        return EXIT_FAILURE;
    }
    consumeNextToken();

    //parse left paranthesis
    if (getNextToken() == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    if (nextToken->type == LPARANTHESIS)
    {
        consumeNextToken();
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
    if (getNextToken() == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    if (nextToken->type == RPARANTHESIS)
    {
        consumeNextToken();
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
    if (getNextToken() == EXIT_FAILURE)
    {
        if (peekOnNextChar() == EOF)
        {
            printf("Finished parsing!\n");
            return EXIT_SUCCESS;
        }
        else
        {
            return EXIT_FAILURE;
        }
    }
    else
    {
        return START();
    }
}

int TYPE()
{
    if (getNextToken() == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    
    switch (nextToken->type)
    {
    case INT:
    case VOID:
        consumeNextToken();
        return EXIT_SUCCESS;
        break;
    default:
        printf("Parsing error! Expected 'int' or 'void'! Got '%s'. Line: %d\n", nextToken->lexeme, lineNumber);
        return EXIT_FAILURE;
    }
}

int PARS()
{
    if (getNextToken() == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }

    if (nextToken->type == INT)
    {
        consumeNextToken();

        if (getNextToken() == EXIT_FAILURE)
        {
            return EXIT_FAILURE;
        }
        
        if (nextToken->type == ID)
        {
            consumeNextToken();
            return PARS_();
        }
        else
        {
            printf("Parsing error! Expected identifier! Got '%s'. Line: %d\n", nextToken->lexeme, lineNumber);
            return EXIT_FAILURE;
        }
    }
    else if (nextToken->type == VOID)
    {
        consumeNextToken();
        return EXIT_SUCCESS;
    }
    else
    {
        printf("Parsing error! Expected int or void! Got '%s'. Line: %d\n", nextToken->lexeme, lineNumber);
        return EXIT_FAILURE;
    }
}

int PARS_()
{
    if (getNextToken() == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }

    if (nextToken->type == COMMA)
    {
        consumeNextToken();
        if (getNextToken() == EXIT_FAILURE)
        {
            return EXIT_FAILURE;
        }
        if (nextToken->type == ID)
        {
            consumeNextToken();
            return PARS_();
        }
        else
        {
            printf("Parsing error! Expected identifier! Got '%s'. Line: %d\n", nextToken->lexeme, lineNumber);
            return EXIT_FAILURE;
        }
    }
    else
    {
        return EXIT_SUCCESS;
    }
}

int BLOCK()
{
    if (getNextToken() == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }

    if (nextToken->type == LBRACE)
    {
        consumeNextToken();
        return BLOCK_();
    }
    else
    {
        printf("Parsing error! Expected '{'! Got '%s'. Line: %d\n",nextToken->lexeme,lineNumber);
        return EXIT_FAILURE;
    }
}

int BLOCK_()
{
    if (getNextToken() == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    if (nextToken->type == RBRACE)
    {
        consumeNextToken();
        return EXIT_SUCCESS;
    }
    else if (STMT() == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    else
    {
        return BLOCK_();
    }
}

int STMT()
{
    if (getNextToken() == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    TOKEN* idToken;
    char* elseLable, exitLable, loopStart, loopExit;
    switch (nextToken->type)
    {
    case ID:
        idToken = nextToken; //id required for IDENT function
        nextToken = NULL; //fake consume
        if (IDENT(idToken) == EXIT_FAILURE)
        {
            return EXIT_FAILURE;
        }
        deleteToken(&idToken); //removal of temporary token
        if (getNextToken() == EXIT_FAILURE)
        {
            return EXIT_FAILURE;
        }
        if (nextToken->type == SEMICOLON)
        {
            consumeNextToken();
            return EXIT_SUCCESS;
        }
        else
        {
            printf("Parsing error! Expected ';'! Got %s. Line: %d\n",nextToken->lexeme,lineNumber);
            return EXIT_FAILURE;
        }
        break;
    case IF:
        consumeNextToken();
        if (getNextToken() == EXIT_FAILURE)
        {
            return EXIT_FAILURE;
        }
        if (nextToken->type == LPARANTHESIS)
        {
            consumeNextToken();
            if (getNextToken() == EXIT_FAILURE)
            {
                return EXIT_FAILURE;
            }
            if (nextToken->type == RPARANTHESIS)
            {
                consumeNextToken();
                elseLable = newLable();
                fprintf(filePtrDest, "BRF %s\n", elseLable);
                if (BLOCK() == EXIT_FAILURE)
                {
                    return EXIT_FAILURE;
                }
                exitLable = newLable();
                fprintf(filePtrDest, "BRA %s\n", exitLable);
                fprintf(filePtrDest, "[%s]\n", elseLable);
                if (IFSTMT() == EXIT_FAILURE)
                {
                    return EXIT_FAILURE;
                }
                fprintf(filePtrDest, "[%s]\n", exitLable);
                free(elseLable);
                free(exitLable);
                return EXIT_SUCCESS;
            }
            else
            {
                printf("Parsing error! Expected ')'! Got %s. Line: %d\n", nextToken->lexeme, lineNumber);
                return EXIT_FAILURE;
            }
        }
        else
        {
            printf("Parsing error! Expected '('! Got %s. Line: %d\n",nextToken->lexeme,lineNumber);
            return EXIT_FAILURE;
        }
        break;
    case WHILE:
        consumeNextToken();
        if (getNextToken() == EXIT_FAILURE)
        {
            return EXIT_FAILURE;
        }
        if (nextToken->type == LPARANTHESIS)
        {
            consumeNextToken();
            loopStart = newLable();
            fprintf(filePtrDest,"[%s]\n",loopStart);
            if (EXPR() == EXIT_FAILURE)
            {
                return EXIT_FAILURE;
            }
            if (getNextToken() == EXIT_FAILURE)
            {
                return EXIT_FAILURE;
            }
            if (nextToken->type == RPARANTHESIS)
            {
                consumeNextToken();
                loopExit = newLable();
                fprintf(filePtrDest, "BRF %s\n", loopExit);
                if (BLOCK() == EXIT_FAILURE)
                {
                    return EXIT_FAILURE;
                }
                fprintf(filePtrDest, "BRA %s\n", loopStart);
                fprintf(filePtrDest, "[%s]\n", loopExit);
                free(loopStart);
                free(loopExit);
                return EXIT_SUCCESS;
            }
            else
            {
                printf("Parsing error! Expected ')'! Got %s. Line: %d\n", nextToken->lexeme, lineNumber);
                return EXIT_FAILURE;
            }
        }
        else
        {
            printf("Parsing error! Expected '('! Got %s. Line: %d\n", nextToken->lexeme, lineNumber);
            return EXIT_FAILURE;
        }
        break;
    case READ:

        break;
    case WRITE:

        break;
    case INT:

        break;
    case RETURN:

        break;
    default:
        printf("Parsing error! Expected id, if, while, read, write, int or return! Got %s. Line: %d\n", nextToken->lexeme, lineNumber);
        return EXIT_FAILURE;
        break;
    }
}

int IDENT()
{

}



int IFSTMT()
{
	if (getNextToken() == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}
	
	if (nextToken->type == ELSE)
	{
		consumeNextToken();
		if (IFSTMT_() == EXIT_FAILURE)
			return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int IFSTMT_()
{
	char* elseLBL, exitLBL;

	if (getNextToken() == EXIT_FAILURE)
		return EXIT_FAILURE;

	if (nextToken->type == IF)
	{
		consumeNextToken();
		if (getNextToken() == EXIT_FAILURE)
			return EXIT_FAILURE;

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

		if (getNextToken() == EXIT_FAILURE)
			return EXIT_FAILURE;

		if (nextToken->type == RPARANTHESIS)
		{
			consumeNextToken();
			elseLBL = newLable();
			fprintf(filePtrDest, "BRF %s \n", elseLBL);
			
			if (BLOCK() == EXIT_FAILURE)
				return EXIT_FAILURE;

			exitLBL = newLable();
			fprintf(filePtrDest, "BRF %s \n", exitLBL);

			fprintf(filePtrDest, "[%s] \n", elseLBL);

			if (IFSTMT() == EXIT_FAILURE)
				return EXIT_FAILURE;

			fprintf(filePtrDest, "[%s] \n", exitLBL);
			
			free(elseLBL);
			free(exitLBL);
			return EXIT_SUCCESS;
		}
	}
	else
	{
		return BLOCK();
	}
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
		return EXIT_SUCCESS;
		break;
	case COMMA:
		consumeNextToken();
		if(VARDEC() == EXIT_FAILURE)
			return EXIT_FAILURE;
		break;
	default:
		return EXIT_SUCCESS;
	}
	return EXIT_SUCCESS;
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
	
	if (nextToken->type == COMMA)
	{
		consumeNextToken();
		if (EXPR() == EXIT_FAILURE)
			return EXIT_FAILURE;

		if (ARGS_() == EXIT_FAILURE)
			return EXIT_FAILURE;

		return EXIT_SUCCESS;
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
	TOKEN* id = NULL;
	switch (nextToken->type)
	{
	case NUM:
		fprintf(filePtrDest, "PUSHINT %s \n", nextToken->attribute);
		consumeNextToken();
		if (TERM_() == EXIT_FAILURE)
			return EXIT_FAILURE;

		break;
	case ID:
		id = malloc(sizeof(TOKEN));
		id = nextToken;
		consumeNextToken();
		if (TERM_(id) == EXIT_FAILURE)
			return EXIT_FAILURE;
		deleteToken(&id);
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
