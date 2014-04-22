#include "lexicalAnalyzer.h"

char* readToken()
{
	FILE* filePtr;
	filePtr = fopen("code2BCompiled.txt", "r");

	if (filePtr == NULL) 
	{
		printf("Unable to open file: 'code2BCompiled.txt' ");
	}

	// EOF is not considered a char, but an int, and since all
	// char values can be stored in int, we define nextChar to be an int

	char token[30];		//for reading token (max 20 letters long)
	int nextChar;  

	int numOfCharsRead = 0;

	while (numOfCharsRead<30)
	{
		nextChar = getc(filePtr);
		numOfCharsRead++;

		switch(nextChar)
		{
			case ';':
				printf("Chars read: %i \n", numOfCharsRead);
				printf("End of statement! \n");
				fclose(filePtr);
				getchar();
				return token;
			break;
		}
	}
	printf("To many characters have been read! \n");
	getchar();
	return NULL;
}

