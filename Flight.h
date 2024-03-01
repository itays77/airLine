#ifndef __FLIGHT__
#define __FLIGHT__

#include "AirportManager.h"
#include "Date.h"
#include "Plane.h"


typedef struct
{
	char	sourceCode[IATA_LENGTH + 1];
	char	destCode[IATA_LENGTH + 1];
	Plane	flightPlane;
	Date	date;
}Flight;

void	initFlight(Flight* pFlight, Plane* thePlane,const AirportManager* pManager);
int		isFlightFromSourceAirport(const Flight* pFlight, const char* code);
int		isFlightToDestAirport(const Flight* pFlight, const char* code);
void	printFlight(const Flight* pFlight);
Airport* setAiportToFlight(const AirportManager* pManager, const char* msg);
int		isPlaneTypeInFlight(const Flight* pFlight, ePlaneType type);
int	compareByDate(const void* flight1, const void* flight2);
int	compareBySrcCode(const void* flight1, const void* flight2);
int	compareByDstCode(const void* flight1, const void* flight2);
int	saveFlightToFile(const Flight* pF, FILE* fp);
int loadFlightFromFile(Flight* pF, const AirportManager* pManager, FILE* fp);



#endif
