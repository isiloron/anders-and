#include "lexicalAnalyzer.h"

TOKEN* getNextToken()
{	
    TOKEN* newToken;
	char nextCharacter, next_next_character;
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
        else if (nextCharacter >= '0' && nextCharacter <= '9')
        {
            newToken = getNumberToken();
            if (newToken == NULL)
            {
                printf("Expected number on line %d!\n", lineNumber);
                return NULL;
            }
            else
            {
                return newToken;
            }
        }
        //character, a capitalized character or underscore
        else if ((nextCharacter >= 'A' && nextCharacter <= 'Z') || (nextCharacter >= 'a' && nextCharacter <= 'z') || (nextCharacter == '_'))
        {
            char* lexeme = getLexeme();
            if (lexeme == NULL)
            {
                printf("Failed to get lexeme for keword or ID! Line: %d\n",lineNumber);
                return NULL;
            }
            newToken = checkLexeme(&tokenList, lexeme);
            if (newToken == NULL)
            {
                printf("Failed to match lexeme with kewords or ID. Line: %d\n",lineNumber);
                return NULL;
            }
            return newToken;

        }
		//comment
		else if (nextCharacter == '/' && next_next_character == '*')
		{
			consumeNextChar();
			consumeNextChar();

			while (peekOnNextChar() != '*' && peekOnNextNextChar() != '/')
			{
                if (peekOnNextChar() == EOF)
                {
                    printf("Comment end marker missing!\n",lineNumber);
                    return NULL;
                }
                else if (peekOnNextChar() == '\n')
                {
                    lineNumber++;
                }
				consumeNextChar();
			}

			consumeNextChar();
			consumeNextChar();

		}
		//linecomment
		else if (nextCharacter == '/' && next_next_character == '/')
		{
			while (peekOnNextChar() != '\n')
			{
				consumeNextChar();
			}
			consumeNextChar();
            lineNumber++;
		}
        //special character
        else
        {
            return specialCharacter();
        }
    }
}

char consumeNextChar()
{
    int nextChar = fgetc(filePtrSource);
    return (char)nextChar;
}

char peekOnNextChar()
{
    int nextChar = fgetc(filePtrSource);
    if (nextChar == EOF)
        return EOF;
    else
    {
        ungetc(nextChar, filePtrSource);
        return nextChar;
    }
}

char peekOnNextNextChar()
{
    char chars[2];

    chars[0] = fgetc(filePtrSource);
    
    if (chars[0] == EOF)
        return EOF;
    else
        chars[1] = fgetc(filePtrSource);
    
    if (chars[1] == EOF)
    {
        ungetc(chars[0], filePtrSource);
        return EOF;
    }
    else
    {
        ungetc(chars[1], filePtrSource);
        ungetc(chars[0], filePtrSource);
        return chars[1];
    }
}

TOKEN* getNumberToken()
{
    TOKEN* newToken = createEmptyToken();
    if (newToken == NULL)
    {
        printf("Failed to create empty token!\n");
        return NULL;
    }

    int index = 0;
    char nextChar = peekOnNextChar();

    while (nextChar >= '0' && nextChar <= '9')
    {
        newToken->lexeme[index] = consumeNextChar();
        nextChar = peekOnNextChar();
        index++;
    }

    newToken->lexeme[index] = '\0';
    newToken->type = NUM;
    newToken->attribute = atoi(newToken->lexeme);
    return newToken;

}

TOKEN* specialCharacter()
{
    TOKEN* newToken = createEmptyToken();
    if (newToken == NULL)
    {
        printf("Failed to create empty token!\n");
        return NULL;
    }

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
            printf("Error! The character '%c'(%d) is not included in the language. Line: %d\n", newToken->lexeme[0], newToken->lexeme[0], lineNumber);
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

    if (string == NULL)
    {
        printf("Failed to allocate memory for string!\n");
        return NULL;
    }

    while (index < BUFFERSIZE)
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
    printf("Lexeme too long! Line: %d\n",lineNumber);
    return NULL;
}

TOKEN* checkLexeme(TOKENNODE** node, char* lexeme)
{
    if (*node == NULL)
    {
        printf("Error! Something is wrong with the token list!\n");
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
            TOKENNODE* newNode = createTokenNode(newToken, NULL);
            (*node)->next = newNode;
            return tokenCopy(newToken);
        }
        else
        {
            TOKEN* newToken = createToken(lexeme, ID, 1);
            TOKENNODE* newNode = createTokenNode(newToken, NULL);
            (*node)->next = newNode;
            return tokenCopy(newToken);
        }
    }
    else
    {
        return checkLexeme(&((*node)->next), lexeme);
    }
}