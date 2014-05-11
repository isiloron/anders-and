#include "parser.h"

int getNextToken()
{
    //nextToken is a global variable
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
            printf("Lexeme: %s, Type: %d, Attribute: %d\n",nextToken->lexeme,nextToken->type,nextToken->attribute);
            return EXIT_SUCCESS;
        }
    }
    else
        return EXIT_SUCCESS;
}

void consumeNextToken()
{
    deleteToken(&nextToken);
}

char* newlabel()
{
    char* label = malloc(LABELSIZE);
    sprintf(label, "Label%d", labelCounter);
    labelCounter++;
    return label;
}

int START_1()
{
    //parse type (void or int)
    if (TYPE() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    //parse identifier
    if (getNextToken() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if (nextToken->type == ID)
        //print function label
        fprintf(filePtrDest, "[%s]\n", nextToken->lexeme);
    else
    {
        printf("Parsing error! Expected identifier! Got '%s'.\n", nextToken->lexeme);
        return EXIT_FAILURE;
    }
    
    consumeNextToken();

    //parse left paranthesis
    if (getNextToken() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    if (nextToken->type == LPARANTHESIS)
        consumeNextToken();
    else
    {
        printf("Parsing error! Expected '('! Got '%s'.\n", nextToken->lexeme);
        return EXIT_FAILURE;
    }

    //parse parameters
    if (PARS_1() == EXIT_FAILURE)
        return EXIT_FAILURE;

    //parse right paranthesis
    if (getNextToken() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    if (nextToken->type == RPARANTHESIS)
        consumeNextToken();
    else
    {
        printf("Parsing error! Expected '('! Got '%s'.\n", nextToken->lexeme);
        return EXIT_FAILURE;
    }

    //parse block
    if (BLOCK_1() == EXIT_FAILURE)
        return EXIT_FAILURE;

    //general function return

    //pop local vaiables generated in block
    if(localVarsCounter > 0)
    {
        fprintf(filePtrDest, "POP %d\n",localVarsCounter);
        localVarsCounter = 0;
    }
    
    fprintf(filePtrDest,"RTS\n");

    return START_2();//parse next function
}

int START_2()
{
    if (getNextToken() == EXIT_FAILURE)
    {
        if (peekOnNextChar() == EOF)
        {
            printf("Finished parsing!\n");
            return EXIT_SUCCESS;
        }
        else
            return EXIT_FAILURE;
    }
    else
        return START_1();
}

int TYPE()
{
    if (getNextToken() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    switch (nextToken->type)
    {
    case INT:
    case VOID:
        consumeNextToken();
        return EXIT_SUCCESS;

    default:
        printf("Parsing error! Expected 'int' or 'void'! Got '%s'. Line: %d\n", nextToken->lexeme, lineNumber);
        return EXIT_FAILURE;
    }
}

int PARS_1()
{
    if (getNextToken() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if (nextToken->type == INT) //parse INT
    {
        consumeNextToken();

        if (getNextToken() == EXIT_FAILURE)
            return EXIT_FAILURE;
        
        if (nextToken->type == ID)//parse ID
        {
            consumeNextToken();
            return PARS_2(); // parse next parameter
        }
        else
        {
            printf("Parsing error! Expected identifier! Got '%s'. Line: %d\n", nextToken->lexeme, lineNumber);
            return EXIT_FAILURE;
        }
    }
    else if (nextToken->type == VOID)//parse VOID
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

int PARS_2()
{
    if (getNextToken() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if (nextToken->type == COMMA)//parse comma
    {
        consumeNextToken();
        
        if (getNextToken() == EXIT_FAILURE)
            return EXIT_FAILURE;
        
        if (nextToken->type == INT)//parse int
        {
            consumeNextToken();
            
            if (getNextToken() == EXIT_FAILURE)
                return EXIT_FAILURE;
            
            if (nextToken->type == ID)//parse ID
            {
                consumeNextToken();
                return PARS_2(); // parse next paramenter
            }
            else
            {
                printf("Parsing error! Expected identifier! Got '%s'. Line: %d\n", nextToken->lexeme, lineNumber);
                return EXIT_FAILURE;
            }
        }
        else
        {
            printf("Parsing error! Expected 'int'! Got '%s'. Line: %d", nextToken->lexeme, lineNumber);
            return EXIT_FAILURE;
        }
    }
    else
        return EXIT_SUCCESS;//no more parameters
}

int BLOCK_1()
{
    if (getNextToken() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if (nextToken->type == LBRACE)//parse left brace (beginning of multi statement block)
    {
        consumeNextToken();
        return BLOCK_2();//parse next statement
    }
    else if (nextToken->type == IF || nextToken->type==WHILE) //parse if or while (illegal one line statement)
    {
        printf("Parsing error! A single line block cannot be an if or a while statement! Line: %d\n",lineNumber);
        return EXIT_FAILURE;
    }
    else//parse legal one line statement
        return STMT();
}

int BLOCK_2()
{
    if (getNextToken() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if (nextToken->type == RBRACE)//parse right brace(end of block)
    {
        consumeNextToken();
        return EXIT_SUCCESS;
    }
    else if (STMT() == EXIT_SUCCESS)//parse statement
        return BLOCK_2();//parse next statment
    else
        return EXIT_FAILURE;
}

int STMT()
{
    char *label; // requeired to temporarily save the id lexeme as a label
    char *elseLabel, *exitLabel, *loopStart, *loopExit;
    
    if (getNextToken() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    switch (nextToken->type)
    {
    case ID: //parse ID
        label = malloc(BUFFERSIZE);
        strcpy(label, nextToken->lexeme); //saving lexeme temporarily
        
        consumeNextToken();
        
        if (IDENT(label) == EXIT_FAILURE)
            return EXIT_FAILURE;
        
        free(label);
        
        if (getNextToken() == EXIT_FAILURE)
            return EXIT_FAILURE;

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

    case IF:
        consumeNextToken();
        
        if (getNextToken() == EXIT_FAILURE)
            return EXIT_FAILURE;

        if (nextToken->type == LPARANTHESIS)
        {
            consumeNextToken();
            
            if (EXPR_1() == EXIT_FAILURE)
                return EXIT_FAILURE;
            
            if (getNextToken() == EXIT_FAILURE)
                return EXIT_FAILURE;
            
            if (nextToken->type == RPARANTHESIS)
            {
                consumeNextToken();
                
                elseLabel = newlabel();
                fprintf(filePtrDest, "BRF %s\n", elseLabel);
                
                if (BLOCK_1() == EXIT_FAILURE)
                    return EXIT_FAILURE;
                
                exitLabel = newlabel();
                fprintf(filePtrDest, "BRA %s\n", exitLabel);
                fprintf(filePtrDest, "[%s]\n", elseLabel);
                
                if (IFSTMT_1() == EXIT_FAILURE)
                    return EXIT_FAILURE;
                
                fprintf(filePtrDest, "[%s]\n", exitLabel);
                
                free(elseLabel);
                free(exitLabel);
                
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

    case WHILE:
        consumeNextToken();
        
        if (getNextToken() == EXIT_FAILURE)
            return EXIT_FAILURE;

        if (nextToken->type == LPARANTHESIS)
        {
            consumeNextToken();
            
            loopStart = newlabel();
            fprintf(filePtrDest, "[%s]\n", loopStart);
            
            if (EXPR_1() == EXIT_FAILURE)
                return EXIT_FAILURE;

            if (getNextToken() == EXIT_FAILURE)
                return EXIT_FAILURE;

            if (nextToken->type == RPARANTHESIS)
            {
                consumeNextToken();
                
                loopExit = newlabel();
                fprintf(filePtrDest, "BRF %s\n", loopExit);
               
                if (BLOCK_1() == EXIT_FAILURE)
                    return EXIT_FAILURE;
                
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

    case READ:
        consumeNextToken();
        
        if (getNextToken() == EXIT_FAILURE)
            return EXIT_FAILURE;

        if (nextToken->type == ID)
        {
            fprintf(filePtrDest, "LVAL %s\n", nextToken->lexeme);
            fprintf(filePtrDest, "READINT\n");
            fprintf(filePtrDest, "ASSINT\n");
            
            consumeNextToken();
            
            if (getNextToken() == EXIT_FAILURE)
                return EXIT_FAILURE;

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

    case WRITE:
        consumeNextToken();
        
        if (EXPR_1() == EXIT_FAILURE)
            return EXIT_FAILURE;

        fprintf(filePtrDest, "WRITEINT\n");
        fprintf(filePtrDest, "POP 1\n");
        
        if (getNextToken() == EXIT_FAILURE)
            return EXIT_FAILURE;

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

    case INT:
        consumeNextToken();
        
        if (getNextToken() == EXIT_FAILURE)
            return EXIT_FAILURE;

        if (nextToken->type == ID)
        {
            fprintf(filePtrDest,"DECL %s\n",nextToken->lexeme);
            localVarsCounter++;
            consumeNextToken();
            
            if (VARDEC() == EXIT_FAILURE)
                return EXIT_FAILURE;

            if (getNextToken() == EXIT_FAILURE)
                return EXIT_FAILURE;

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

    case RETURN:
        consumeNextToken();
        
        fprintf(filePtrDest, "LVAL @\n");
        
        if (EXPR_1() == EXIT_FAILURE)
            return EXIT_FAILURE;
        
        fprintf(filePtrDest, "ASSINT\n");
        
        if (localVarsCounter > 0)
        {
            fprintf(filePtrDest, "POP %d\n", localVarsCounter);
            localVarsCounter = 0;
        }
        
        fprintf(filePtrDest, "RTS\n");
        
        if (getNextToken() == EXIT_FAILURE)
            return EXIT_FAILURE;

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

    default:
        printf("Parsing error! Expected id, if, while, read, write, int or return! Got %s. Line: %d\n", nextToken->lexeme, lineNumber);
        return EXIT_FAILURE;
    }
}

int IDENT(char* label)
{
    if (getNextToken() == EXIT_FAILURE)
        return EXIT_FAILURE;

    switch (nextToken->type)
    {
    case ASSIGNOP:
        consumeNextToken();
        
        fprintf(filePtrDest, "LVAL %s\n", label);
        
        if (EXPR_1() == EXIT_FAILURE)
            return EXIT_FAILURE;

        fprintf(filePtrDest, "ASSINT\n");

        return EXIT_SUCCESS;

    case LPARANTHESIS:
        consumeNextToken();
        fprintf(filePtrDest, "DECL @\n");
        
        if (ARGS_1(label) == EXIT_FAILURE)
            return EXIT_FAILURE;

        if (getNextToken() == EXIT_FAILURE)
            return EXIT_FAILURE;

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

    default:
        printf("Parsing error! Expected '=' or '('! Got %s. Line: %d\n", nextToken->lexeme, lineNumber);
        return EXIT_FAILURE;
    }
}

int IFSTMT_1()
{
	if (getNextToken() == EXIT_FAILURE)
		return EXIT_FAILURE;
	
	if (nextToken->type == ELSE)
	{
		consumeNextToken();

		if (IFSTMT_2() == EXIT_FAILURE)
			return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int IFSTMT_2()
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
			if (EXPR_1() == EXIT_FAILURE)
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
			elseLBL = newlabel();
			fprintf(filePtrDest, "BRF %s \n", elseLBL);
			
			if (BLOCK_1() == EXIT_FAILURE)
				return EXIT_FAILURE;

			exitLBL = newlabel();
			fprintf(filePtrDest, "BRF %s \n", exitLBL);

			fprintf(filePtrDest, "[%s] \n", elseLBL);

			if (IFSTMT_1() == EXIT_FAILURE)
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
		return BLOCK_1();
}

int VARDEC()
{
	if (getNextToken() == EXIT_FAILURE)
		return EXIT_FAILURE;

    if (nextToken->type == COMMA)
    {
        consumeNextToken();
        
        if (getNextToken() == EXIT_FAILURE)
            return EXIT_FAILURE;

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
        return EXIT_SUCCESS;
}

int ARGS_1(char* label)
{
    if (getNextToken() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if (nextToken->type == VOID)
    {
        consumeNextToken();
        
        fprintf(filePtrDest,"BSR %s",label);
        
        return EXIT_SUCCESS;
    }

	if(EXPR_1() == EXIT_FAILURE)
		return EXIT_FAILURE;

	if(ARGS_2(label) == EXIT_FAILURE)
		return EXIT_FAILURE;

    fprintf(filePtrDest,"POP 1\n");

	return EXIT_SUCCESS;
}

int ARGS_2(char* label)
{
	if (getNextToken() == EXIT_FAILURE)
		return EXIT_FAILURE;
	
	if (nextToken->type == COMMA)
	{
		consumeNextToken();
		
        if (EXPR_1() == EXIT_FAILURE)
			return EXIT_FAILURE;

		if (ARGS_2(label) == EXIT_FAILURE)
			return EXIT_FAILURE;
        
        fprintf(filePtrDest, "POP 1\n");
		
        return EXIT_SUCCESS;
	}
    else
    {
        fprintf(filePtrDest, "BSR %s\n", label);
        return EXIT_SUCCESS;
    }
}

int EXPR_1()
{
    if (EXPR_2() == EXIT_FAILURE)
        return EXIT_FAILURE;

    return EXPR_3();
}

int EXPR_2()
{
    if (SUM_1() == EXIT_FAILURE)
        return EXIT_FAILURE;

    return EXPR_4();
}

int EXPR_3()
{
    if (getNextToken() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if (nextToken->type == EQOP)
    {
        consumeNextToken();
        
        if (EXPR_1() == EXIT_FAILURE)
            return EXIT_FAILURE;
        
        fprintf(filePtrDest, "EQINT\n");
        
        return EXIT_SUCCESS;
    }
    else
        return EXIT_SUCCESS;
}



int EXPR_4()
{	
	if (getNextToken() == EXIT_FAILURE)
		return EXIT_FAILURE;

	switch (nextToken->type)
	{
	case LTOP:
		consumeNextToken();
		
        if (EXPR_2() == EXIT_FAILURE)
			return EXIT_FAILURE;
		
        fprintf(filePtrDest, "LTINT\n");
        
        return EXIT_SUCCESS;

	case LOEOP:
		consumeNextToken();

		if (EXPR_2() == EXIT_FAILURE)
			return EXIT_FAILURE;
		
        fprintf(filePtrDest, "LEINT\n");
        
        return EXIT_SUCCESS;

	default:
        return EXIT_SUCCESS;
	}
}

int SUM_1()
{
	if (TERM_1() == EXIT_FAILURE)
		return EXIT_FAILURE;
    
    return SUM_2();
}

int SUM_2()
{
	if (getNextToken() == EXIT_FAILURE)
		return EXIT_FAILURE;

	switch (nextToken->type)
	{
	case PLUSOP:
		consumeNextToken();
		
        if (SUM_1() == EXIT_FAILURE)
			return EXIT_FAILURE;
		
        fprintf(filePtrDest, "ADD\n");
        
        return EXIT_SUCCESS;

	case MINUSOP:
		consumeNextToken();
		
        if (SUM_1() == EXIT_FAILURE)
			return EXIT_FAILURE;
		
        fprintf(filePtrDest, "SUB\n");
        
        return EXIT_SUCCESS;

	default:
		return EXIT_SUCCESS;
	}
}

int TERM_1()
{
    char* label;
	
    if (getNextToken() == EXIT_FAILURE)
		return EXIT_FAILURE;
    
	switch (nextToken->type)
	{
	case NUM:
        fprintf(filePtrDest, "PUSHINT %d\n", nextToken->attribute);
        
        consumeNextToken();
        
        return TERM_2();

	case ID:
		label = malloc(BUFFERSIZE);
        strcpy(label, nextToken->lexeme);
		consumeNextToken();
        
        if (TERM_3(label) == EXIT_SUCCESS)
        {
            free(label);
            return EXIT_SUCCESS;
        }
        else
            return EXIT_FAILURE;

	case LPARANTHESIS:
		consumeNextToken();
		
        if (EXPR_1() == EXIT_FAILURE)
			return EXIT_FAILURE;

		if (getNextToken() == EXIT_FAILURE)
			return EXIT_FAILURE;

		if (nextToken->type == RPARANTHESIS)
		{
            consumeNextToken();
            return EXIT_SUCCESS;
		}
        else
        {
            printf("Parser error! Expected ')'! Got '%s'. Line: %d\n",nextToken->lexeme, lineNumber);
            return EXIT_FAILURE;
        }

	case NOTOP:
		consumeNextToken();
        
        if (NOTOPERAND_1() == EXIT_FAILURE)
            return EXIT_FAILURE;

        fprintf(filePtrDest, "NOT\n");

		if (TERM_2() == EXIT_FAILURE)
			return EXIT_FAILURE;

        return EXIT_SUCCESS;

	default:
        printf("Parser error! Expected number, id, '(' or '!'. Got %s. Line: %d", nextToken->lexeme, lineNumber);
		return EXIT_FAILURE;
	}
}

int TERM_2()
{
	if (getNextToken() == EXIT_FAILURE)
		return EXIT_FAILURE;

	switch (nextToken->type)
	{
	case MULTOP:
		consumeNextToken();
		
        if (TERM_1() == EXIT_FAILURE)
			return EXIT_FAILURE;
		
        fprintf(filePtrDest, "MULT\n");
        
        return EXIT_SUCCESS;

	case DIVOP:
		consumeNextToken();
		
        if (TERM_1() == EXIT_FAILURE)
			return EXIT_FAILURE;
		
        fprintf(filePtrDest, "DIV\n");
        
        return EXIT_SUCCESS;

	default:
		return EXIT_SUCCESS;
	}
}

int TERM_3(char* label)
{
	if (getNextToken() == EXIT_FAILURE)
		return EXIT_FAILURE;
	
	if (nextToken->type == LPARANTHESIS)
	{
		consumeNextToken();
		fprintf(filePtrDest, "DECL @\n");

		if (ARGS_1(label) == EXIT_FAILURE)
			return EXIT_FAILURE;

		if (getNextToken() == EXIT_FAILURE)
			return EXIT_FAILURE;

		if (nextToken->type == RPARANTHESIS)
		{
            consumeNextToken();
            return TERM_2();
		}
		else
		{
            printf("Parser error! Missing right paranthesis on line %d", lineNumber);
            return EXIT_FAILURE;
		}	
	}
	else
	{
        fprintf(filePtrDest, "RVALINT %s\n", label);
        return TERM_2();
	}
}

int NOTOPERAND_1()
{
    char *label;

    if (getNextToken() == EXIT_FAILURE)
        return EXIT_FAILURE;

    switch (nextToken->type)
    {
    case LPARANTHESIS:
        consumeNextToken();
        
        if (EXPR_1() == EXIT_FAILURE)
            return EXIT_FAILURE;

        if (getNextToken() == EXIT_FAILURE)
            return EXIT_FAILURE;

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
        
        if (NOTOPERAND_1() == EXIT_FAILURE)
            return EXIT_FAILURE;

        fprintf(filePtrDest,"NOT\n");

        return EXIT_SUCCESS;

    case NUM:
        fprintf(filePtrDest,"PUSHINT %d\n",nextToken->attribute);
        
        consumeNextToken();
        
        return EXIT_SUCCESS;

    case ID:
        label = malloc(BUFFERSIZE);
        strcpy(label, nextToken->lexeme);
        consumeNextToken();
        
        if (NOTOPERAND_2(label) == EXIT_FAILURE)
            return EXIT_FAILURE;

        free(label);

        return EXIT_SUCCESS;

    default:
        printf("Parser error! Expected '(', '!', number or id! Got '%s'. Line: %d", nextToken->lexeme, lineNumber);
        return EXIT_FAILURE;
    }
}

int NOTOPERAND_2(char* label)
{
    if (getNextToken() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if (nextToken->type == LPARANTHESIS)
    {
        consumeNextToken();

        fprintf(filePtrDest, "DECL @\n");

        if (ARGS_1(label) == EXIT_FAILURE)
            return EXIT_FAILURE;

        if (getNextToken() == EXIT_FAILURE)
            return EXIT_FAILURE;

        if (nextToken->type == RPARANTHESIS)
        {
            consumeNextToken();
            return EXIT_SUCCESS;
        }
        else
        {
            printf("Parser error! Expected ')'! Got '%s'. Line: %d", nextToken->lexeme, lineNumber);
            return EXIT_FAILURE;
        }
    }
    else
    {
        fprintf(filePtrDest, "RVALINT %s\n", label);

        return EXIT_SUCCESS;
    }
}
