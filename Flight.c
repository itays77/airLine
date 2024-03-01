#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Flight.h"
#include "General.h"


void	initFlight(Flight* pFlight,Plane* thePlane, const AirportManager* pManager)
{
	Airport* pPortOr = setAiportToFlight(pManager, "Enter code of origin airport:");
	strcpy(pFlight->sourceCode, pPortOr->code);
	int same;
	Airport* pPortDes;
	do {
		pPortDes = setAiportToFlight(pManager, "Enter code of destination airport:");
		same = isSameAirport(pPortOr, pPortDes);
		if (same)
			printf("Same origin and destination airport\n");
	} while (same);
	strcpy(pFlight->destCode, pPortDes->code);
	pFlight->flightPlane = *thePlane;
	getCorrectDate(&pFlight->date);
}

int		isFlightFromSourceAirport(const Flight* pFlight, const char* code)
{
	if (strcmp(pFlight->sourceCode, code) == 0)
		return 1;
		
	return 0;
}


int		isFlightToDestAirport(const Flight* pFlight, const char* code)
{
	if (strcmp(pFlight->destCode, code) == 0)
		return 1;

	return 0;


}

int		isPlaneTypeInFlight(const Flight* pFlight, ePlaneType type)
{
	return (pFlight->flightPlane.type == type);
}


void	printFlight(const Flight* pFlight)
{
	printf("Flight From %s To %s\t",pFlight->sourceCode, pFlight->destCode);
	printDate(&pFlight->date);
	printf("\t");
	printPlane(&pFlight->flightPlane);
}

Airport* setAiportToFlight(const AirportManager* pManager, const char* msg)
{
	char code[MAX_STR_LEN];
	Airport* port;
	do
	{
		printf("%s\t", msg);
		getAirportCode(code);
		port = findAirportByCode(pManager, code);
		if (port == NULL)
			printf("No airport with this code - try again\n");
	} while (port == NULL);

	return port;
}

int saveFlightToFile(const Flight* pF, FILE* fp)
{

    if (fwrite(pF, sizeof(Flight), 1, fp) != 1)
    {
        printf("Error write flight\n");
        return 0;
    }

    return 1;
}


int loadFlightFromFile(Flight* pF, const AirportManager* pManager, FILE* fp)
{

    if (fread(pF, sizeof(Flight), 1, fp) != 1)
    {
        printf("Error read flight\n");
        return 0;
    }

    return 1;
}

int	compareBySrcCode(const void* flight1, const void* flight2)
{
    const Flight* pFlight1 = *(const Flight**)flight1;
    const Flight* pFlight2 = *(const Flight**)flight2;
    return strcmp(pFlight1->sourceCode, pFlight2->sourceCode);
}

int	compareByDstCode(const void* flight1, const void* flight2)
{
    const Flight* pFlight1 = *(const Flight**)flight1;
    const Flight* pFlight2 = *(const Flight**)flight2;
    return strcmp(pFlight1->destCode, pFlight2->destCode);
}

int	compareByDate(const void* flight1, const void* flight2)
{
    const Flight* pFlight1 = *(const Flight**)flight1;
    const Flight* pFlight2 = *(const Flight**)flight2;

    Date d1 = pFlight1->date;
    Date d2 = pFlight2->date;

    if (d1.year > d2.year)
        return 1;
    if (d1.year < d2.year)
        return -1;

    if (d1.month > d2.month)
        return 1;
    if (d1.month < d2.month)
        return -1;

    if (d1.day > d2.day)
        return 1;
    if (d1.day < d2.day)
        return -1;
    return 0;
}
