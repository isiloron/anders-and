#include "lexicalAnalyzer.h"
#include "main.h"

/*
	http://www.cpptutor.com/imgs/ascii_table.gif
*/

int main()
{
	FILE* filePtr = fopen("code2BCompiled.txt", "r");

	if (filePtr == NULL)
	{
		printf("Unable to open file: 'code2BCompiled.txt' \n");
		return NULL;
	}

}