#include "lexicalAnalyzer.h"
#include "main.h"

/*	http://www.cpptutor.com/imgs/ascii_table.gif is used to identify characters from the ASCII table */

int main()
{
	filePtr = fopen("code2BCompiled.txt", "r");

	if (filePtr == NULL)
	{
		perror("Unable to open file: 'code2BCompiled.txt' \n");
		exit(EXIT_FAILURE);
	}

	TOKEN* newToken = malloc(sizeof(TOKEN));

	if (newToken == NULL)
	{
		perror("One simply not allocate memory for a new token! \n");
		exit(EXIT_FAILURE);
	}

	newToken = getToken();
}

