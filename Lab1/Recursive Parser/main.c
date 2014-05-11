#include "main.h"

/*	http://www.cpptutor.com/imgs/ascii_table.gif is used to identify characters from the ASCII table */

int main(int argc, char* argv[])
{
	if (prepareGlobalVars(argv[1]) == EXIT_FAILURE)
	{
		printf("Failed to prepare global varaibles!\n");
        getchar();
		return EXIT_FAILURE;
	}
    /*
    TOKEN* token;
    while (1)
    {
        token = readNextToken();
        if (token == NULL)
        {
            break;
        }
        test_lexical_analyzer(token);
    }
    */

    if (START_1() == EXIT_FAILURE)
    {
        if (peekOnNextChar() == EOF)
            printf("Parsing complete!\n");
        else
            printf("Parsing incomplete!\n");
        getchar();
    }

    fclose(filePtrDest);
    fclose(filePtrSource);

	getchar();
    return EXIT_SUCCESS;
}

