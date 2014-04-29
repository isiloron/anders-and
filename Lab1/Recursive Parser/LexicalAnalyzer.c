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

TOKEN* getNextToken()
{
	TOKEN* newToken = createEmptyToken();		
	char nextCharacter;
	int index = 0;

	while (1)
	{
		nextCharacter = peekOnNextChar();

		//end of file
		if (feof(filePtr))
		{
			newToken->lexeme[0] = '\0';
			newToken->attribute = 0;
			newToken->type = 0;
			return newToken;
		}
		//ignore
		else if ( (nextCharacter == '\t') || (nextCharacter == ' ') || (nextCharacter == '\n') )
		{
            consumeNextChar();
		}
		//number
		else if (nextCharacter > 47 && nextCharacter < 58)
		{
            getNumberToken(newToken);
            return newToken;
		}
		//character, a capitalized character or underscore
		else if ((nextCharacter > 64 && nextCharacter < 91) || (nextCharacter > 96 && nextCharacter < 123) || (nextCharacter == 95))
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
                free(newToken);
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
            if (peekOnNextChar() == 61) // =
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
            if (peekOnNextChar() == 61) // =
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
		if ((nextChar > 47 && nextChar < 58) || (nextChar > 64 && nextChar < 91) || (nextChar > 96 && nextChar < 123) || (nextChar == 95))
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

int lexemeIsKeyword(TOKEN* newToken)
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