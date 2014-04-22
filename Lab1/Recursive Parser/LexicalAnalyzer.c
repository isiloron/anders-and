#include "lexicalAnalyzer.h"

char* readToken()
{
	FILE* filePtr;
	filePtr = fopen("/Resource Files/code2BCompiled.txt", "r");

	if (filePtr == NULL) 
	{
		printf("Unable to open file: 'code2BCompiled.txt' ");
	}

	// EOF is not considered a char, but an int, and since all
	// char values can be stored in int, we define nextChar to be an int

	char token[20];		//for reading token (max 20 letters long)
	int nextChar;  

	int numOfCharsRead = 0;

	while(1) 
	{
		nextChar = getc(filePtr);
		numOfCharsRead++;

		switch(nextChar)
		{
			case ';':
				printf("Chars read: %i", numOfCharsRead);
				printf("Slut på statement!");
				
				return token;
			break;
		}
	}          
}

