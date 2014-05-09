#include "main.h"

void test_lexical_analyzer(TOKEN* newToken)
{
	if (newToken != NULL)
	{
		printf("<");
		//print lexeme of token
		printf(newToken->lexeme);
		printf(",");

		//print type of token
		switch (newToken->type)
		{
		case RETURN:
			printf("RETURN");
			break;
		case IF:
			printf("IF");
			break;
		case ELSE:
			printf("ELSE");
			break;
		case WHILE:
			printf("WHILE");
			break;
		case WRITE:
			printf("WRITE");
			break;
		case READ:
			printf("READ");
			break;
		case VOID:
			printf("VOID");
			break;
		case INT:
			printf("INT");
			break;
		case ID:
			printf("ID");
			break;
		case NUM:
			printf("NUM");
			break;
		case LBRACE:
			printf("LBRACE");
			break;
		case RBRACE:
			printf("RBRACE");
			break;
		case LPARANTHESIS:
			printf("LPARANTHESIS");
			break;
		case RPARANTHESIS:
			printf("RPARANTHESIS");
			break;
		case SEMICOLON:
			printf("SEMICOLON");
			break;
		case COMMA:
			printf("COMMA");
			break;
		case ASSIGNOP:
			printf("ASSIGNOP");
			break;
		case PLUSOP:
			printf("PLUSOP");
			break;
		case MINUSOP:
			printf("MINUSOP");
			break;
		case MULTOP:
			printf("MULTOP");
			break;
		case DIVOP:
			printf("DIVOP");
			break;
		case NOTOP:
			printf("NOTOP");
			break;
		case EQOP:
			printf("EQOP");
			break;
		case LTOP:
			printf("LTOP");
			break;
		case LOEOP:
			printf("LOEOP");
			break;
		default:
			break;
		}

		printf(",");
		//print attribute of token
		printf("%d>\n", newToken->attribute);
		free(newToken);
	}
}