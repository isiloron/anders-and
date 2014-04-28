#include "lexicalAnalyzer.h"

void lookAhead()
{
	TOKEN* newToken = createToken();
	FILE* filePtr = openFile();
	peekOnNextChar(filePtr);
}
TOKEN* createToken()
{
	TOKEN* token = malloc(sizeof(TOKEN));
	
	exit(EXIT_FAILURE);
	if (token == NULL)
	{	
		perror("Could not allocate new token! \n");
		exit(EXIT_FAILURE);
	}
	else
	{
		token->lexeme = malloc(sizeof(char)*BUFFERSIZE);

		if (token->lexeme == NULL)
		{
			perror("Could not allocate new token! \n");
			exit(EXIT_FAILURE);
		}
		else
		{
			token->type = 0;
			token->attribute = malloc(sizeof(ATTRSIZE));
			return token;
		}
	}
}
char peekOnNextChar(FILE* filePtr)
{
	int nextChar = fgetc(filePtr);
		ungetc(nextChar, filePtr);
	
	return (char)nextChar;
}
FILE* openFile()
{
	FILE* filePtr = fopen("code2BCompiled.txt", "r");

	if (filePtr == NULL)
	{
		perror("Unable to open file: 'code2BCompiled.txt' \n");
		exit(EXIT_FAILURE);
	}
	else
	{
		return filePtr;
	}
}


