#include "lexicalAnalyzer.h"

char consumeNextChar()
{
	int nextChar = fgetc(filePtr);
	return (char)nextChar;
}

char peekOnNextChar()
{
	int nextChar = fgetc(filePtr);
    if (nextChar == EOF)
        return EOF;
	ungetc(nextChar, filePtr);
	return nextChar;
}

char peekOnNextNextChar()
{
	int chars[2];

	chars[0] = fgetc(filePtr);
	if (chars[0] == EOF)
		return EOF;
	chars[1] = fgetc(filePtr);
	if (chars[1] == EOF)
		return EOF;
	ungetc(chars[1], filePtr);
	ungetc(chars[0], filePtr);

	return chars[1];
}

TOKEN* getNextToken()
{
	TOKEN* newToken;		
	char nextCharacter, next_next_character;
	int index = 0;
    while (1)
    {
        nextCharacter = peekOnNextChar();
		next_next_character = peekOnNextNextChar();

        //end of file
        if (nextCharacter == EOF)
        {
            return NULL;
        }
        
        //whitespace
        else if ((nextCharacter == '\t') || (nextCharacter == ' ') || (nextCharacter == '\n'))
        {
            if (nextCharacter == '\n')
                lineNumber++;

            consumeNextChar();
        }
        //number
        else if (nextCharacter > '0' && nextCharacter < '9')
        {
            newToken = getNumberToken();
            if (newToken != NULL)
                return newToken;
            else
            {
                printf("Expected number on line %d!\n", lineNumber);
                return NULL;
            }
        }
        //character, a capitalized character or underscore
        else if ((nextCharacter > 'A' && nextCharacter < 'Z') || (nextCharacter > 'a' && nextCharacter < 'z') || (nextCharacter == '_'))
        {
            char* lexeme = getLexeme();

            return checkLexeme(&tokenList, lexeme);

        }
		//comment
		else if (nextCharacter == '/' && peekOnNextNextChar() == '*')
		{
			consumeNextChar();
			consumeNextChar();

			while (peekOnNextChar != '*' && peekOnNextNextChar() != '/')
			{
				consumeNextChar();
			}

			consumeNextChar();
			consumeNextChar();

		}
		//linecomment
		else if (nextCharacter == '/' && peekOnNextChar() == '/')
		{
			while (peekOnNextChar != '\n')
			{
				consumeNextChar();
			}
			consumeNextChar();
		}
        //special character
        else
        {	
				newToken = specialCharacter();
				return newToken;
			
        }
    }
}

TOKEN* getNumberToken()
{
    TOKEN* newToken = createEmptyToken();
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
        deleteToken(&newToken);
        return NULL;
    }
    else
    {
        newToken->lexeme[index] = '\0';
        newToken->type = NUM;
        newToken->attribute = atoi(newToken->lexeme);
        return newToken;
    }
}

TOKEN* specialCharacter()
{
    TOKEN* newToken = createEmptyToken();
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
            printf("Error! '%c'(%d) is not included in the language. Line: %d\n", newToken->lexeme[0], newToken->lexeme[0], lineNumber);
            deleteToken(&newToken);
			break;
	}
	return newToken;
}

char* getLexeme()
{
	int index = 0;
	char nextChar;
    char* string = malloc(BUFFERSIZE);

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
			return string;
		}
	}	
}

TOKEN* checkLexeme(TOKENNODE** node, char* lexeme)
{
    if (*node == NULL)
    {
        printf("Error! Something is wrong with the token list!");
        return NULL;
    }
    else if (strcmp((*node)->token->lexeme, lexeme) == 0)
    {
        return tokenCopy((*node)->token);
    }
    else if ((*node)->next == NULL)
    {
        if ((*node)->token->type == ID)
        {
            TOKEN* newToken = createToken(lexeme, ID, (*node)->token->attribute + 1);
            appendTokenToList(&tokenList, newToken);
            return tokenCopy(newToken);
        }
        else
        {
            TOKEN* newToken = createToken(lexeme, ID, 1);
            appendTokenToList(&tokenList, newToken);
            return tokenCopy(newToken);
        }
    }
    else
    {
        return checkLexeme(&((*node)->next), lexeme);
    }
}