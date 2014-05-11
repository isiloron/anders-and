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
            printf("Got token! Lexeme: %s, Type: %d, Attribute: %d\n",nextToken->lexeme,nextToken->type,nextToken->attribute);
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

    if(localVarsCounter > 0)
    {
        fprintf(filePtrDest, "POP %d\n",localVarsCounter);
        localVarsCounter = 0;
    }
    fprintf(filePtrDest,"RTS\n");

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
    char *elseLable, *exitLable, *loopStart, *loopExit;
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
            printf("Parsing error! Expected ';'! Got %s. Line: %d\n", nextToken->lexeme, lineNumber);
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
            printf("Parsing error! Expected '('! Got %s. Line: %d\n", nextToken->lexeme, lineNumber);
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
            fprintf(filePtrDest, "[%s]\n", loopStart);
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
        consumeNextToken();
        if (getNextToken() == EXIT_FAILURE)
        {
            return EXIT_FAILURE;
        }
        if (nextToken->type == ID)
        {
            fprintf(filePtrDest, "LVAL %s\n", nextToken->lexeme);
            fprintf(filePtrDest, "READINT\n");
            fprintf(filePtrDest, "ASSINT\n");
            consumeNextToken();
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
                printf("Parsing error! Expected ';'! Got %s. Line: %d\n", nextToken->lexeme, lineNumber);
                return EXIT_FAILURE;
            }
        }
        else
        {
            printf("Parsing error! Expected ID! Got %s. Line: %d\n", nextToken->lexeme, lineNumber);
            return EXIT_FAILURE;
        }
        break;
    case WRITE:
        consumeNextToken();
        if (getNextToken() == EXIT_FAILURE)
        {
            return EXIT_FAILURE;
        }
        if (nextToken->type == ID)
        {
            fprintf(filePtrDest, "RVALINT %s\n", nextToken->lexeme);
            fprintf(filePtrDest, "WRITEINT\n");
            fprintf(filePtrDest, "POP 1\n");
            consumeNextToken();
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
                printf("Parsing error! Expected ';'! Got %s. Line: %d\n", nextToken->lexeme, lineNumber);
                return EXIT_FAILURE;
            }
        }
        else
        {
            printf("Parsing error! Expected ID! Got %s. Line: %d\n", nextToken->lexeme, lineNumber);
            return EXIT_FAILURE;
        }
        break;
    case INT:
        consumeNextToken();
        if (getNextToken() == EXIT_FAILURE)
        {
            return EXIT_FAILURE;
        }

        if (nextToken->type == ID)
        {
            fprintf(filePtrDest,"DECL %s\n",nextToken->lexeme);
            localVarsCounter++;
            consumeNextToken();
            if (VARDEC() == EXIT_FAILURE)
            {
                return EXIT_FAILURE;
            }
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
                printf("Parsing error! Expected ';'! Got %s. Line: %d\n", nextToken->lexeme, lineNumber);
                return EXIT_FAILURE;
            }
        }
        else
        {
            printf("Parsing error! Expected ID! Got %s. Line: %d\n", nextToken->lexeme, lineNumber);
            return EXIT_FAILURE;
        }
        break;
    case RETURN:
        consumeNextToken();
        fprintf(filePtrDest, "LVAL @\n");
        if (EXPR() == EXIT_FAILURE)
        {
            return EXIT_FAILURE;
        }
        fprintf(filePtrDest, "ASSINT\n");
        if (localVarsCounter > 0)
        {
            fprintf(filePtrDest, "POP %d\n", localVarsCounter);
            localVarsCounter = 0;
        }
        fprintf(filePtrDest, "RTS\n");
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
            printf("Parsing error! Expected ';'! Got %s. Line: %d\n", nextToken->lexeme, lineNumber);
            return EXIT_FAILURE;
        }
        break;
    default:
        printf("Parsing error! Expected id, if, while, read, write, int or return! Got %s. Line: %d\n", nextToken->lexeme, lineNumber);
        return EXIT_FAILURE;
        break;
    }
}

int IDENT(TOKEN* idToken)
{
    char* lable;
    if (getNextToken() == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    switch (nextToken->type)
    {
    case ASSIGNOP:
        consumeNextToken();
        fprintf(filePtrDest, "LVAL %s\n", idToken->lexeme);
        if (EXPR() == EXIT_FAILURE)
        {
            return EXIT_FAILURE;
        }
        fprintf(filePtrDest, "ASSINT\n");
        return EXIT_SUCCESS;
        break;
    case LPARANTHESIS:
        consumeNextToken();
        fprintf(filePtrDest, "DECL @\n");
        lable = malloc(BUFFERSIZE);
        strcpy(lable, idToken->lexeme);
        if (ARGS(lable) == EXIT_FAILURE)
        {
            return EXIT_FAILURE;
        }
        free(lable);
        if (getNextToken() == EXIT_FAILURE)
        {
            return EXIT_FAILURE;
        }
        if (nextToken->type == RPARANTHESIS)
        {
            consumeNextToken();
            fprintf(filePtrDest, "POP 1\n"); // pop return value
            return EXIT_SUCCESS;
        }
        else
        {
            printf("Parsing error! Expected ')'! Got %s. Line: %d\n", nextToken->lexeme, lineNumber);
            return EXIT_FAILURE;
        }
        break;
    default:
        printf("Parsing error! Expected '=' or '('! Got %s. Line: %d\n", nextToken->lexeme, lineNumber);
        return EXIT_FAILURE;
    }
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
	char *elseLBL, *exitLBL;

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
        else
        {
            printf("Parser error! Missing ')' on line %d", lineNumber);
            return EXIT_FAILURE;
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
            fprintf(filePtrDest, "DECL %s\n", nextToken->lexeme);
            localVarsCounter++;
            consumeNextToken();
            return VARDEC();
        }
        else
        {
            printf("Parser error! Expected ID! Got %s. Line: %d", nextToken->lexeme, lineNumber);
            return EXIT_FAILURE;
        }
    }
    else
    {
        return EXIT_SUCCESS;
    }
}

int ARGS(char* lable)
{
    if (getNextToken() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if (nextToken->type == VOID)
    {
        consumeNextToken();
        fprintf(filePtrDest,"BSR %s",lable);
        return EXIT_SUCCESS;
    }

	if(EXPR() == EXIT_FAILURE)
		return EXIT_FAILURE;

	if(ARGS_(lable) == EXIT_FAILURE)
		return EXIT_FAILURE;
    fprintf(filePtrDest,"POP 1\n");
	return EXIT_SUCCESS;
}

int ARGS_(char* lable)
{
	if (getNextToken() == EXIT_FAILURE)
    {
		return EXIT_FAILURE;
	}
	
	if (nextToken->type == COMMA)
	{
		consumeNextToken();
		if (EXPR() == EXIT_FAILURE)
			return EXIT_FAILURE;

		if (ARGS_(lable) == EXIT_FAILURE)
			return EXIT_FAILURE;
        fprintf(filePtrDest, "POP 1\n");
		return EXIT_SUCCESS;
	}
    else
    {
        fprintf(filePtrDest, "BSR %s\n", lable);
        return EXIT_SUCCESS;
    }
}

int EXPR()
{
	if (SUM() == EXIT_FAILURE)
		return EXIT_FAILURE;
    return EXPR_();
}

int EXPR_()
{	
	if (getNextToken() == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	switch (nextToken->type)
	{
	case EQOP:
		consumeNextToken();
		if (EXPR() == EXIT_FAILURE)
			return EXIT_FAILURE;
		fprintf(filePtrDest, "EQINT\n");
        return EXIT_SUCCESS;
	case LTOP:
		consumeNextToken();
		if (EXPR() == EXIT_FAILURE)
			return EXIT_FAILURE;
		fprintf(filePtrDest, "LTINT\n");
        return EXIT_SUCCESS;
	case LOEOP:
		consumeNextToken();
		if (EXPR() == EXIT_FAILURE)
			return EXIT_FAILURE;
		fprintf(filePtrDest, "LEINT\n");
        return EXIT_SUCCESS;
	default:
        return EXIT_SUCCESS;
	}
}

int SUM()
{
	if (TERM() == EXIT_FAILURE)
		return EXIT_FAILURE;
    return SUM_();
}

int SUM_()
{
	if (getNextToken() == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	switch (nextToken->type)
	{
	case PLUSOP:
		consumeNextToken();
		if (SUM() == EXIT_FAILURE)
			return EXIT_FAILURE;
		fprintf(filePtrDest, "ADD\n");
        return EXIT_SUCCESS;
	case MINUSOP:
		consumeNextToken();
		if (SUM() == EXIT_FAILURE)
			return EXIT_FAILURE;
		fprintf(filePtrDest, "SUB\n");
        return EXIT_SUCCESS;
	default:
		return EXIT_SUCCESS;
	}
}

int TERM()
{
	if (getNextToken() == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}
    char* lable;
	switch (nextToken->type)
	{
	case NUM:
        fprintf(filePtrDest, "PUSHINT %d\n", nextToken->attribute);
        consumeNextToken();
        return TERM_();
	case ID:
		lable = malloc(BUFFERSIZE);
        strcpy(lable, nextToken->lexeme);
		consumeNextToken();
        if (TERM__(lable) == EXIT_SUCCESS)
        {
            free(lable);
            return EXIT_SUCCESS;
        }
        else
        {
            return EXIT_FAILURE;
        }
	case LPARANTHESIS:
		consumeNextToken();
		if (EXPR() == EXIT_FAILURE)
			return EXIT_FAILURE;

		if (getNextToken() == EXIT_FAILURE)
		{
			return EXIT_FAILURE;
		}

		if (nextToken->type == RPARANTHESIS)
		{
            consumeNextToken();
            return EXIT_SUCCESS;
			printf("Parser error! Missing right paranthesis on line %d", lineNumber);
			return EXIT_FAILURE;
		}
        else
        {
            printf("Parser error! Expected ')'! Got '%s'. Line: %d\n",nextToken->lexeme, lineNumber);
            return EXIT_FAILURE;
        }
	case NOTOP:
		consumeNextToken();
        if (NOTOPERAND() == EXIT_FAILURE)
        {
            return EXIT_FAILURE;
        }
        fprintf(filePtrDest, "NOT\n");
		if (TERM_() == EXIT_FAILURE)
			return EXIT_FAILURE;
        return EXIT_SUCCESS;
	default:
        printf("Parser error! Expected number, id, '(' or '!'. Got %s. Line: %d", nextToken->lexeme, lineNumber);
		return EXIT_FAILURE;
	}
}

int TERM_()
{
	if (getNextToken() == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	switch (nextToken->type)
	{
	case MULTOP:
		consumeNextToken();
		if (TERM() == EXIT_FAILURE)
			return EXIT_FAILURE;
		fprintf(filePtrDest, "MULT\n");
        return EXIT_SUCCESS;
	case DIVOP:
		consumeNextToken();
		if (TERM() == EXIT_FAILURE)
			return EXIT_FAILURE;
		fprintf(filePtrDest, "DIV\n");
        return EXIT_SUCCESS;
	default:
		return EXIT_SUCCESS;
	}
}

int TERM__(char* lable)
{
	if (getNextToken() == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}
	
	if (nextToken->type == LPARANTHESIS)
	{
		consumeNextToken();
		fprintf(filePtrDest, "DECL @\n");

		if (ARGS(lable) == EXIT_FAILURE)
			return EXIT_FAILURE;

		if (getNextToken() == EXIT_FAILURE)
		{
			return EXIT_FAILURE;
		}

		if (nextToken->type == RPARANTHESIS)
		{
            consumeNextToken();
            return TERM_();
		}
		else
		{
            printf("Parser error! Missing right paranthesis on line %d", lineNumber);
            return EXIT_FAILURE;
		}	
	}
	else
	{
        return TERM_();
	}
}

int NOTOPERAND()
{
    char *lable;

    if (getNextToken() == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }

    switch (nextToken->type)
    {
    case LPARANTHESIS:
        consumeNextToken();
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
            return EXIT_SUCCESS;
        }
        else
        {
            printf("Parsing error! Expected ')'. Got %s. Line: %d", nextToken->lexeme, lineNumber);
            return EXIT_FAILURE;
        }
    case NOTOP:
        consumeNextToken();
        if (NOTOPERAND() == EXIT_FAILURE)
        {
            return EXIT_FAILURE;
        }
        fprintf(filePtrDest,"NOT\n");
        return EXIT_SUCCESS;
    case NUM:
        fprintf(filePtrDest,"PUSHINT %d",nextToken->attribute);
        consumeNextToken();
        return EXIT_SUCCESS;
    case ID:
        lable = malloc(BUFFERSIZE);
        strcpy(lable, nextToken->lexeme);
        consumeNextToken();
        if (getNextToken() == EXIT_FAILURE)
        {
            return EXIT_FAILURE;
        }
        if (nextToken->type == LPARANTHESIS)
        {
            consumeNextToken();
            if (ARGS(lable) == EXIT_FAILURE)
            {
                return EXIT_FAILURE;
            }
            free(lable);
            if (getNextToken() == EXIT_FAILURE)
            {
                return EXIT_FAILURE;
            }
            if (nextToken->type == RPARANTHESIS)
            {
                consumeNextToken();
                return EXIT_SUCCESS;
            }
            else
            {
                printf("Parser error! Expected ')'! Got '%s'. Line: %d",nextToken->lexeme,lineNumber);
                return EXIT_FAILURE;
            }
        }
        else
        {
            return EXIT_SUCCESS;
        }
    default:
        printf("Parser error! Expected '(', '!', number or id! Got '%s'. Line: %d", nextToken->lexeme, lineNumber);
        return EXIT_FAILURE;
    }
}
