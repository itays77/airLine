#ifndef __COMP__
#define __COMP__

#include "Flight.h"
#include "AirportManager.h"

typedef enum { eNone,  eSrcCode, eDstCode, eDate, eNofSortOpt } eSortOptions;
static const char* sortOptStr[eNofSortOpt];

typedef struct
{
	char*		name;
	int			flightCount;
	Flight**	flightArr;
	int			planeCount;
	Plane*		planeArr;
    eSortOptions	sortOpt;
}Airline;

void	initAirline(Airline* pComp);
int	initAirlineFromFile(Airline * pComp, AirportManager* pManager, const char* fileName);
int		addFlight(Airline* pComp,const AirportManager* pManager);
int		addPlane(Airline* pComp);
Plane*	FindAPlane(Airline* pComp);
void	printCompany(const Airline* pComp);
void	printFlightArr(Flight** arr, int size);
void	printPlanesArr(Plane* arr,int size);
void	doPrintFlightsWithPlaneType(const Airline* pComp);
void	freeFlightArr(Flight** arr, int size);
void	freePlanes(Plane* arr, int size);
void	freeCompany(Airline* pComp);
void sortFlight(Airline* pComp);
void findFlight(const Airline * pComp);
int saveAirlineToFile(const Airline* pComp, const char* fileName);
int loadAirlineFromFile(Airline* pComp, const AirportManager* pManager, const char* fileName);
eSortOptions showSortMenu();

#endif

