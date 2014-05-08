#include "globalvars.h"

int prepareGlobalVars(char* fileName)
{
    if (generateTokenList())
    {
        printf("Failed to generate list of keywords!\n");
        return EXIT_FAILURE;
    }
    filePtrSource = fopen(fileName, "r");
    if (filePtrSource == NULL)
    {
        printf("Unable to open file: %s \n",fileName);
        return EXIT_FAILURE;
    }
    lineNumber = 1;
    currentToken = NULL;
    return EXIT_SUCCESS;
}