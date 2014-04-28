#include "lexicalAnalyzer.h"

char lookAhead(FILE* filePtr)
{
	char nextCharacter;
	nextCharacter = peekOnNextChar(filePtr);
	return nextCharacter;
}

char match(FILE* filePtr)
{
	int nextChar = fgetc(filePtr);
	return (char)nextChar;
}

char peekOnNextChar(FILE* filePtr)
{
	int nextChar = fgetc(filePtr);
		ungetc(nextChar, filePtr);
	
	return (char)nextChar;
}

TOKEN* get_Token(FILE* filePtr)
{
	char nextCharacter;

	while (1)
	{
		nextCharacter = lookAhead(filePtr);

		//This is space
		if (nextCharacter == 32)
		{

		}
		//This is a number
		else if (nextCharacter>47 && nextCharacter<58)
		{

		}
		//This is a character or underscore
		else if ((nextCharacter>64 && nextCharacter<91) || (nextCharacter>96 && nextCharacter<123) || (nextCharacter == 95))
		{

		}
		//This is some special character
		else
		{

			switch (nextCharacter)
			{
			case 33:// ' ! '
				break;

			case 38:// ' & '
				break;

			case 40:// ' ( '
				break;

			case 41:// ' ) '
				break;
			case 42:// ' * '
				break;

			case 43:// ' + '
				break;

			case 45:// ' - '
				break;

			case 47:// ' / '
				break;

			case 59:// ' ; '
				break;

			case 60:// ' < '
				break;
			case 61:// ' = '
				break;

			case 62:// ' > '
				break;

			case 123:// ' { '
				break;

			case 124:// ' | '
				break;

			case 125:// ' } '
				break;

			default:
				perror("Error! %c is not included in the language. \n");
				exit(EXIT_FAILURE);
				break;
			}
		}
}




