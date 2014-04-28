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




