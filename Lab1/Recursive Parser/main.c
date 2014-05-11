/*
Compiler theory - spring 2014
Lab 1: 
Lexical analysis and recursive parser

The program reads characters with a lexical analyzer from the sorce-file, 
assembles them into tokens. The parser then checks the syntax and translates 
the tokens into track42 machine code in the destination-file. 

Students:	Linus Järvelä		(lja08001)
			Stefan Danielsson	(sdn08003)
*/

#include "main.h"

/*The in-argument is the name of the sorce-file (c-code), that we want to pars (track42)*/
int main(int argc, char* argv[])
{
	if (prepareGlobalVars(argv[1]) == EXIT_FAILURE)
	{
		printf("Failed to prepare global varaibles!\n");
        getchar();
		return EXIT_FAILURE;
	}

	//Uncomment the code below, and comment out the rest, to test the lexical analyzer alone. 

    /* 
    TOKEN* token;
    while (1)
    {
        token = readNextToken();
        if (token == NULL)
            break;
        test_lexical_analyzer(token);
    }

	getchar();

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