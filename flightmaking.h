#ifndef	_FLIGHTMAKING_H_
#define	_FLIGHTMAKING_H_

#define IO_ERROR 0
#define IO_SUCCESS 1

#define MAX_AIRPORT_NAME_SIZE 69
#define MAX_AIRPORT_LIST_SIZE 1230

int readAirportList(char * fileName);
int makeFlightList(char *fileName);

#endif