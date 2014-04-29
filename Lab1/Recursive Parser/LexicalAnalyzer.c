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

		if (feof(filePtr))
		{
			newToken->lexeme[0] = '\0';
			newToken->attribute = 0;
			newToken->type = 0;
			return newToken;
		}
		else if ( (nextCharacter == '\t') || (nextCharacter == ' ') || (nextCharacter == '\n') )
		{
            consumeNextChar();

            /*
			newToken->lexeme[index] = '\0';

			//if we did'nt read a token earlier, consume character, and continue reading 
			if (newToken->type == 0)
			{
				nextCharacter = matchNextChar(filePtr);
			}
			else if (newToken->type == NUM)
			{
				newToken->attribute = atoi(newToken->lexeme);
				nextCharacter = matchNextChar(filePtr);
				return newToken;
			}
			else if (strcmp("return", newToken->lexeme) == 0)
			{
				newToken->type = RETURN;
				nextCharacter = matchNextChar(filePtr);
				return newToken;
			}
			else if (strcmp("if", newToken->lexeme) == 0)
			{
				newToken->type = IF;
				nextCharacter = matchNextChar(filePtr);
				return newToken;
			}
			else if (strcmp("else", newToken->lexeme) == 0)
			{
				newToken->type = ELSE;
				nextCharacter = matchNextChar(filePtr);
				return newToken;
			}
			else if (strcmp("while", newToken->lexeme) == 0)
			{
				newToken->type = WHILE;
				nextCharacter = matchNextChar(filePtr);
				return newToken;
			}
			else if (strcmp("write", newToken->lexeme) == 0)
			{
				newToken->type = WRITE;
				nextCharacter = matchNextChar(filePtr);
				return newToken;
			}
			else if (strcmp("void", newToken->lexeme) == 0)
			{
				newToken->type = VOID;
				nextCharacter = matchNextChar(filePtr);
				return newToken;
			}
			else if (strcmp("int", newToken->lexeme) == 0)
			{
				newToken->type = INT;
				nextCharacter = matchNextChar(filePtr);
				return newToken;
			}
			else 
			{
				nextCharacter = matchNextChar(filePtr);
				return newToken;
			}
            */
		}
		//number
		else if (nextCharacter > 47 && nextCharacter < 58)
		{
            getNumberToken(newToken);
            return newToken;
            /*
			newToken->lexeme[index] = nextCharacter;
			index++;
			newToken->type = NUM;
			nextCharacter = matchNextChar(filePtr);
            */
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