#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "General.h"



char* getStrExactName(const char* msg)
{
	char* str;
	char temp[MAX_STR_LEN];
	printf("%s\t", msg);
	myGets(temp, MAX_STR_LEN);

	str = getDynStr(temp);
	return str;
}

char* getDynStr(char* str)
{
	char* theStr;
	theStr = (char*)malloc((strlen(str) + 1) * sizeof(char));
	if (!theStr)
		return NULL;

	strcpy(theStr, str);
	return theStr;
}

char* myGets(char* buffer, int size)
{
	char* ok;
	if (buffer != NULL && size > 0)
	{
		do { //skip only '\n' strings
			ok = fgets(buffer, size, stdin);
		} while (ok && ((strlen(buffer) <= 1) && (isspace(buffer[0]))));
		if (ok)
		{
            char* back = buffer + strlen(buffer);
            //trim end spaces
			while ((buffer < back) && (isspace(*--back)));
			*(back + 1) = '\0';
			return buffer;
		}
		buffer[0] = '\0';
	}
	return NULL;
}

char** splitCharsToWords(char* str, int* pCount, int* pTotalLength)
{
	char temp[255];
	char* delimiters = " ";
	char* word;
	int count = 0;

	strcpy(temp, str);
	char** wordsArray = NULL;
	*pTotalLength = 0;

	word = strtok(temp, delimiters);
	while (word != NULL)
	{
		wordsArray = (char**)realloc(wordsArray, (count + 1) * sizeof(char*));
		if (!wordsArray)
			return 0;
		wordsArray[count] = getDynStr(word);
		count++;
		*pTotalLength += (int)strlen(word);
		word = strtok(NULL, delimiters);
	}
	*pCount = count;
	return wordsArray;
}

int	 writeStringToFile(char* str, FILE* fp,const char* msg)
{
    int length = strlen(str);
    length++;
    if (!writeIntToFile(length, fp,msg))
        return 0;

    if (!writeCharsToFile(str, length, fp, msg))
        return 0;


    return 1;
}

int	 writeCharsToFile(char* arr, int size, FILE* fp, const char* msg)
{
    if (fwrite(arr, sizeof(char), size, fp) != size)
    {
        puts(msg);
        fclose(fp);
        return 0;
    }
    return 1;

}

int	 writeIntToFile(int val, FILE* fp, const char* msg)
{
    if (fwrite(&val, sizeof(int), 1, fp) != 1)
    {
        puts(msg);
        fclose(fp);
        return 0;
    }
    return 1;
}

char* readStringFromFile(FILE* fp, const char* msg)
{
    char* str;
    int length;
    if (!readIntFromFile(&length, fp,msg))
        return NULL;
    str = (char*)malloc((length) * sizeof(char));
    if (!str)
    {
        puts(msg);
        fclose(fp);
        return NULL;
    }
    if (fread(str, sizeof(char), length, fp) != length)
    {
        puts(msg);
        fclose(fp);
        return NULL;
    }
    return str;

}

int	  readIntFromFile(int* pVal, FILE* fp, const char* msg)
{
    if (fread(pVal, sizeof(int), 1, fp) != 1)
    {
        puts(msg);
        fclose(fp);
        return 0;
    }
    return 1;

}