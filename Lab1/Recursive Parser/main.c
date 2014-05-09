#include "main.h"

/*	http://www.cpptutor.com/imgs/ascii_table.gif is used to identify characters from the ASCII table */

int main(int argc, char* argv[])
{
	if (prepareGlobalVars(argv[0]))
	{
		printf("Failed to prepare global varaibles!\n");
		return EXIT_FAILURE;
	}

	while (!feof(filePtrSource))
	{
		TOKEN* newToken;
		newToken = readNextToken(tokenList);
		test_lexical_analizer(newToken);
	}
	getchar();
}

