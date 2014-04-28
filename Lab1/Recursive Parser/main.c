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
	int i = 30;
	while (i!=0)
	{

		TOKEN* newToken = malloc(sizeof(TOKEN));

		if (newToken == NULL)
		{
			perror("One simply not allocate memory for a new token! \n");
			exit(EXIT_FAILURE);
		}

		newToken = getToken();

		switch (newToken->type)
		{
		case RETURN:
			printf("<RETURN>\n");
			break;
		case IF:
			printf("<IF>\n");
			break;
		case ELSE:
			printf("<ELSE>\n");
			break;
		case WHILE:
			printf("<WHILE>\n");
			break;
		case WRITE:
			printf("<WRITE>\n");
			break;
		case READ:
			printf("<READ>\n");
			break;
		case VOID:
			printf("<VOID>\n");
			break;
		case INT:
			printf("<INT>\n");
			break;
		case ID:
			printf("<ID>\n");
			break;
		case NUM:
			printf("<NUM>\n");
			break;
		default:
			break;
		}

		free(newToken);
		i--;
	}

	fflush(stdin);
	getchar();

}

