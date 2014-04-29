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

TOKEN* getNextToken()
{
	TOKEN* newToken = createEmptyToken();		
	char nextCharacter;
	int index = 0;

	while (1)
	{
		if (feof(filePtr))
		{
			newToken->lexeme[0] = '\0';
			newToken->attribute = 0;
			newToken->type = 0;
			return newToken;
		}

        nextCharacter = peekOnNextChar();
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
                perror("Expected number on line %d!",lineNumber);
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

            /*
			newToken->lexeme[index] = nextCharacter;
			index++;
			newToken->type = ID;
			nextCharacter = matchNextChar(filePtr);
            */
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

TOKEN* specialCharacter(TOKEN* newToken)
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