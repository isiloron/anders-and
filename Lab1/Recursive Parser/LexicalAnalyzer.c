#include "lexicalAnalyzer.h"

char consumeNextChar()
{
	int nextChar = fgetc(filePtr);
	return (char)nextChar;
}

char peekOnNextChar()
{
	int nextChar = fgetc(filePtr);
	ungetc(nextChar, filePtr);
	return (char)nextChar;
}

TOKEN* createEmptyToken()
{
	TOKEN* emptyToken = malloc(sizeof(TOKEN));

	if (emptyToken == NULL)
	{
		perror("Could not allocate memory for new token! \n");
		exit(EXIT_FAILURE);
	}
	else
	{
		emptyToken->attribute = 0;
		emptyToken->lexeme = malloc(sizeof(BUFFERSIZE));
		emptyToken->type = 0;
		return emptyToken;
	}
}

void deleteToken(TOKEN* token)
{
    free(token->lexeme);
    free(token);
    return;
}

TOKEN* getNextToken(TOKEN* tokenList)
{
	TOKEN* newToken = createEmptyToken();		
	char nextCharacter;
	int index = 0;

	while (1)
	{
		//end of file
		if (feof(filePtr))
		{
			newToken->lexeme[0] = '\0';
			newToken->attribute = 0;
			newToken->type = 0;
			return newToken;
		}
        nextCharacter = peekOnNextChar();
        //whitespace
		if ( (nextCharacter == '\t') || (nextCharacter == ' ') || (nextCharacter == '\n') )
		{
            if (nextCharacter == '\n')
                lineNumber++;

            consumeNextChar();
		}
		//number
		else if (nextCharacter > '0' && nextCharacter < '9')
		{
            getNumberToken(newToken);
            if(newToken != NULL)
                return newToken;
            else
            {
                perror(printf("Expected number on line %d!",lineNumber));
                exit(EXIT_FAILURE);
            }
		}
		//character, a capitalized character or underscore
		else if ((nextCharacter > 'A' && nextCharacter < 'Z') || (nextCharacter > 'a' && nextCharacter < 'z') || (nextCharacter == '_'))
		{
            getLexeme(newToken->lexeme);

            if (lexemeIsKeyword(newToken))
            {
                return newToken;
            }
            else if (lexemeIsID(newToken))
            {
                return newToken;
            }
            else
            {
                //something is very wrong
                deleteToken(newToken);
                return NULL;
            }
		}
		//special character
		else
		{
			specialCharacter(newToken);
			return newToken;
		}
	}
}

void getNumberToken(TOKEN* newToken)
{
    int index = 0;
    char nextChar = peekOnNextChar();

    while (nextChar > '0' && nextChar < '9')
    {
        newToken->lexeme[index] = consumeNextChar();
        nextChar = peekOnNextChar();
        index++;
    }
    
    if ((nextChar > 'A' && nextChar < 'Z') || (nextChar > 'a' && nextChar < 'z') || (nextChar == '_'))
    {
        deleteToken(newToken);
    }
    else
    {
        newToken->lexeme[index] = '\n';
        newToken->type = NUM;
        newToken->attribute = atoi(newToken->lexeme);
    }
    return;
}

void specialCharacter(TOKEN* newToken)
{
    newToken->lexeme[0] = consumeNextChar();
    newToken->lexeme[1] = '\0';

	switch (newToken->lexeme[0])
	{
		case '!':
			newToken->type = NOTOP;
			break;
		case '(':
			newToken->type = LPARANTHESIS;
			break;
		case ')': 
			newToken->type = RPARANTHESIS;
			break;
		case '*':
			newToken->type = MULTOP;
			break;
		case '+':
			newToken->type = PLUSOP;
			break;
		case ',':
			newToken->type = COMMA;
			break;
		case '-':
			newToken->type = MINUSOP;
			break;
		case '/':
			newToken->type = DIVOP;
			break;
		case ';':
			newToken->type = SEMICOLON;
			break;
		case '<':
            if (peekOnNextChar() == '=')
            {
                newToken->lexeme[1] = consumeNextChar();
                newToken->lexeme[2] = '\0';
                newToken->type = LOEOP;
            }
            else
            {
                newToken->type = LTOP;
            }
			break;
		case '=':
            if (peekOnNextChar() == '=')
            {
                newToken->lexeme[1] = consumeNextChar();
                newToken->lexeme[2] = '\0';
                newToken->type = EQOP;
            }
            else
            {
                newToken->type = ASSIGNOP;
            }
			break;
		case '{':
			newToken->type = LBRACE;
			break;
		case '}':
			newToken->type = RBRACE;
			break;
		default:
			perror("Error! %c is not included in the language. \n",peekOnNextChar());
			exit(EXIT_FAILURE);
			break;
	}
	return;
}

void getLexeme(char* string)
{
	int index = 0;
	char nextChar;

	while (1)
	{
		nextChar = peekOnNextChar();

		//if valid character read on
		if ((nextChar >= '0' && nextChar <= '9') || (nextChar >= 'A' && nextChar <= 'Z') || (nextChar >= 'a' && nextChar <= 'z') || (nextChar == '_'))
		{
			string[index] = nextChar;
			index++;
			consumeNextChar();
		}
		//read a special character (end of token)
		else
		{
			string[index] = '\0';
			return;
		}
	}	
}

int lexemeIsKeyword(TOKENLIST* tokenList, TOKEN* newToken)
{
	if (strcmp(newToken->lexeme, "return") == 0)
	{
		newToken->attribute = 0;
		newToken->type = RETURN;
		return 1;
	}
	else if ( strcmp(newToken->lexeme, "if") == 0 )
	{
		newToken->attribute = 0;
		newToken->type = IF;
		return 1;
	}
	else if ( strcmp(newToken->lexeme, "else ") == 0 )
	{
		newToken->attribute = 0;
		newToken->type = ELSE;
		return 1;
	}
	else if ( strcmp(newToken->lexeme, "while") == 0 )
	{
		newToken->attribute = 0;
		newToken->type = WHILE;
		return 1;
	}
	else if ( strcmp(newToken->lexeme, "write")== 0 )
	{
		newToken->attribute = 0;
		newToken->type = WRITE;
		return 1;
	}
	else if ( strcmp(newToken->lexeme, "read") == 0 )
	{
		newToken->attribute = 0;
		newToken->type = READ;
		return 1;
	}
	else if ( strcmp(newToken->lexeme, "void") == 0 )
	{
		newToken->attribute = 0;
		newToken->type = VOID;
		return 1;
	}
	else if (strcmp(newToken->lexeme, "int") == 0)
	{
		newToken->attribute = 0;
		newToken->type = INT;
		return 1;
	}
	else
		return 0;
}

int lexemeIsID(TOKEN* newToken)
{
    return 0;
}

TOKENLIST* createList()
{
	TOKENLIST* ptrToList = (TOKENLIST*)malloc(sizeof(TOKENLIST));
	if (ptrToList == NULL)
	{
		perror("Failed to create a list of tokens");
		exit(EXIT_FAILURE);
	}
	else
	{
		ptrToList->currToken = NULL;
		ptrToList->nextToken = NULL;
		return ptrToList;
	}
}