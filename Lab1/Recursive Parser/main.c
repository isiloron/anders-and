#include "lexicalAnalyzer.h"
#include "main.h"


int main()
{
	FILE* filePtr = fopen("code2BCompiled.txt", "r");

	if (filePtr == NULL)
	{
		printf("Unable to open file: 'code2BCompiled.txt' \n");
		return NULL;
	}

	char nextCharacter;

	while (1)
	{
		nextCharacter = match(filePtr);
		switch (nextCharacter)
		{

			default:
				printf("%c", nextCharacter);
				break;
		}
	}
}