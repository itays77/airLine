#ifndef __GENERAL__
#define __GENERAL__

#define MAX_STR_LEN 255


char*	getStrExactName(const char* msg);
char*	myGets(char* buffer, int size);
char*	getDynStr(char* str);
char**	splitCharsToWords(char* str,int* pCount,int* pTotalLength);

int	 writeStringToFile(char* str, FILE* fp,const char* msg);
int	 writeCharsToFile(char* arr,int size, FILE* fp, const char* msg);
int	 writeIntToFile(int val, FILE* fp, const char* msg);

int	  readIntFromFile(int* val, FILE* fp, const char* msg);
char* readStringFromFile(FILE* fp,const char* msg);



#endif

