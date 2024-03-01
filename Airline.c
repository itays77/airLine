#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Airline.h"
#include "Airport.h"
#include "General.h"
#include "listGen.h"



static const char* sortOptStr[eNofSortOpt] = {
        "None","Airport source code", "Airport destination code" , "Date"};

void	initAirline(Airline* pComp)
{
	//printf("-----------  Init Airline\n");
	pComp->name = getStrExactName("Enter Airline name");
	pComp->flightArr = NULL;
	pComp->flightCount = 0;
	pComp->planeArr = NULL;
	pComp->planeCount = 0;
}

int	initAirlineFromFile(Airline * pComp, AirportManager* pManager, const char* fileName)
{
    L_init(&pComp->flightArr);
    if (loadAirlineFromFile(pComp, pManager, fileName))
    {
        return 1;
    }
    return 0;
}

int	addFlight(Airline* pComp,const AirportManager* pManager)
{
	if (pManager->airportsCount < 2)
	{
		printf("There are not enough airport to set a flight\n");
		return 0;
	}
	if(pComp->planeCount == 0)
	{
		printf("There is no plane in company\n");
		return 0;
	}

	Flight* pFlight = (Flight*)calloc(1, sizeof(Flight));
	if (!pFlight)
		return 0;
	
	Plane* thePlane = FindAPlane(pComp);
	printAirports(pManager);
	initFlight(pFlight, thePlane,pManager);

	pComp->flightArr = (Flight**)realloc(pComp->flightArr, (pComp->flightCount + 1) * sizeof(Flight*));
	if (!pComp->flightArr)
	{
		free(pFlight);
		return 0;
	}
	pComp->flightArr[pComp->flightCount] = pFlight;
	pComp->flightCount++;
	return 1;
}

int		addPlane(Airline* pComp)
{
	pComp->planeArr = (Plane*)realloc(pComp->planeArr, (pComp->planeCount + 1) * sizeof(Plane));
	if (!pComp->planeArr)
		return 0;
	initPlane(&pComp->planeArr[pComp->planeCount], pComp->planeArr, pComp->planeCount);
	pComp->planeCount++;
	return 1;
}

Plane* FindAPlane(Airline* pComp)
{
	printf("Choose a plane from list, type its serial Number\n");
	printPlanesArr(pComp->planeArr,pComp->planeCount);
	int sn;
	Plane* temp = NULL;
	do {
		scanf("%d", &sn);
		temp = findPlaneBySN(pComp->planeArr,pComp->planeCount, sn);
		if (!temp)
			printf("No plane with that serial number! Try again!\n");
	} while (temp == NULL);
	 
	return temp;
}


void printCompany(const Airline* pComp)
{
	printf("Airline %s\n", pComp->name);
	printf("\n -------- Has %d planes\n", pComp->planeCount);
	printPlanesArr(pComp->planeArr, pComp->planeCount);
	printf("\n\n -------- Has %d flights\n", pComp->flightCount);
	printFlightArr(pComp->flightArr, pComp->flightCount);
}

void	printFlightArr(Flight** arr, int size)
{
	for (int i = 0; i < size; i++)
		printFlight(arr[i]);
}

void	printPlanesArr(Plane* arr, int size)
{
	for (int i = 0; i < size; i++)
		printPlane(&arr[i]);
}

void	doPrintFlightsWithPlaneType(const Airline* pComp)
{
	ePlaneType type = getPlaneType();
	int count = 0;
	printf("Flights with plane type %s:\n", GetPlaneTypeStr(type));
	for (int i = 0; i < pComp->flightCount; i++)
	{
		if (isPlaneTypeInFlight(pComp->flightArr[i], type))
		{
			printFlight(pComp->flightArr[i]);
			count++;
		}
	}
	if(count == 0)
		printf("Sorry - could not find a flight with plane type %s:\n", GetPlaneTypeStr(type));
	printf("\n");
}

void	freeFlightArr(Flight** arr, int size)
{
	for (int i = 0; i < size; i++)
		free(arr[i]);
}

void freePlanes(Plane* arr, int size)
{
	for (int i = 0; i < size; i++)
		freePlane(&arr[i]);
}

void sortFlight(Airline* pComp)
{
    pComp->sortOpt = showSortMenu();
    int(*compare)(const void* air1, const void* air2) = NULL;

    switch (pComp->sortOpt)
    {
        case eSrcCode:
            compare = compareBySrcCode;
            break;
        case eDstCode:
            compare = compareByDstCode;
            break;
        case eDate:
            compare = compareByDate;
            break;

    }

    if (compare != NULL)
        qsort(pComp->flightArr, pComp->flightCount, sizeof(Flight*), compare);

}

eSortOptions showSortMenu()
{
    int opt;
    printf("Base on what field do you want to sort?\n");
    do {
        for (int i = 1; i < eNofSortOpt; i++)
            printf("Enter %d for %s\n", i, sortOptStr[i]);
        scanf("%d", &opt);
    } while (opt < 0 || opt >eNofSortOpt);

    return (eSortOptions)opt;
}

void findFlight(const Airline * pComp)
{
    int(*compare)(const void* air1, const void* air2) = NULL;
    Flight f = { 0 };
    Flight* pFlight = &f;


    switch (pComp->sortOpt)
    {

        case eSrcCode:
            getchar();
            getAirportCode(f.sourceCode);
            compare = compareByDstCode;
            break;
        case eDstCode:
            getchar();
            getAirportCode(f.destCode);
            compare = compareByDstCode;
            break;

        case eDate:
            getchar();
            getCorrectDate(&f.date);
            compare = compareByDate;
            break;
    }

    if (compare != NULL)
    {
        Flight** pF = bsearch(&pFlight, pComp->flightArr, pComp->flightCount, sizeof(Flight*), compare);
        if (pF == NULL)
            printf("Flight was not found\n");
        else {
            printf("Flight found, ");
            printFlight(*pF);
        }
    }
    else {
        printf("The search cannot be performed, array not sorted\n");
    }

}

int saveAirlineToFile(const Airline* pComp, const char* fileName)
{
    FILE* fp;
    fp = fopen(fileName, "wb");
    if (!fp) {
        printf("Error open airline file to write\n");
        return 0;
    }

    if (!writeStringToFile(pComp->name, fp, "Error write airline name\n"))
        return 0;

    if (!writeIntToFile(pComp->flightCount, fp, "Error write flight count\n"))
        return 0;

    if (!writeIntToFile((int)pComp->sortOpt, fp, "Error write sort option\n"))
        return 0;

    for (int i = 0; i < pComp->flightCount; i++)
    {
        if (!saveFlightToFile(pComp->flightArr[i], fp))
            return 0;
    }

    fclose(fp);
    return 1;
}

int loadAirlineFromFile(Airline* pComp, const AirportManager* pManager, const char* fileName)
{
    FILE* fp;
    fp = fopen(fileName, "rb");
    if (!fp)
    {
        printf("Error open company file\n");
        return 0;
    }

    pComp->flightArr = NULL;


    pComp->name = readStringFromFile(fp, "Error reading company name\n");
    if (!pComp->name)
        return 0;

    if (!readIntFromFile(&pComp->flightCount, fp, "Error reading flight count name\n"))
        return 0;

    int opt;
    if (!readIntFromFile(&opt, fp,"Error reading sort option\n"))
        return 0;

    pComp->sortOpt = (eSortOptions)opt;

    if (pComp->flightCount > 0)
    {
        pComp->flightArr = (Flight**)malloc(pComp->flightCount * sizeof(Flight*));
        if (!pComp->flightArr)
        {
            printf("Alocation error\n");
            fclose(fp);
            return 0;
        }
    }
    else
        pComp->flightArr = NULL;

    for (int i = 0; i < pComp->flightCount; i++)
    {
        pComp->flightArr[i] = (Flight*)calloc(1, sizeof(Flight));
        if (!pComp->flightArr[i])
        {
            printf("Alocation error\n");
            fclose(fp);
            return 0;
        }
        if (!loadFlightFromFile(pComp->flightArr[i], pManager, fp))
            return 0;
    }

    fclose(fp);
    return 1;
}



void	freeCompany(Airline* pComp)
{
	freeFlightArr(pComp->flightArr, pComp->flightCount);
	free(pComp->flightArr);
	free(pComp->planeArr);
	free(pComp->name);
}
