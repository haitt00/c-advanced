#include <stdio.h>
#include "jrb.h"
#include "dllist.h"
#include <stdlib.h>
#include <string.h>
#include "flightmaking.h"
#define INFINITIVE_VALUE 10000000
#define INVALID_NAME "none"
#define INPUT_FILE "flight10.txt"
#define DESTINATION_DISPLAY 5

typedef struct {
    JRB airports;
    JRB flights;
} Graph;

// TODO: change in jval.h so that a jval can hold a string with MAX_AIRPORT_NAME_SIZE;
Graph createGraph();
void dropGraph(Graph graph);
// max DESTINATION_DISPLAY destination for a source 
void printAllFlightAndAirport(Graph graph);
void printAllFlightFromAirport(Graph graph, char * airport);

void addNewAirport(Graph graph, char * airport, int airportID);
int getAirportID(Graph graph, char * airportName);
char * getAiportName(Graph graph, int airportID);

void addFlight(Graph graph, char * source, char * destination, int ticket_price);
int addFlightByFile(Graph graph, char * fileName);

void menu();
void init();

// indegree
int flightToAirport(Graph graph, char * airportName, int* output);
// outdegree
int flightFromAirport(Graph graph, char * airportName, int* output);

int getTicketPrice(Graph graph, char * source, char * destination);
int cheapestFlight(Graph graph, char * source, char * destination, int * path, int * path_length);

// sort purpose
int cheapestFlightFromAirport(Graph graph, char * source, int * output);
int cheapestFlightFromAllAirport(Graph graph, int * output);