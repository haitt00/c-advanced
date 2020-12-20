#include <stdio.h>
#include "jrb.h"
#include "dllist.h"
#include <stdlib.h>
#include <string.h>

#define CITY_COUNT_INITIAL 312 
#define CITY_LENGTH 21

typedef struct {
	int cityCount;
	char** cities;
    int** hasflight;
    int** distance;
    int** connections;
} Graph;
// main skeleton: 
Graph createGraph(int cityCount);
void dropGraph(Graph graph);
void addFlight(Graph graph, char * source, char * destination);
void removeFLight(Graph graph, char * source, char * destination);

void printAllFlight(Graph graph);
void printAllFlightFromCity(Graph graph, char * city);

void printQuickestRoute(Graph graph, char* source, char* destination);
//shortest patht algorithm on connections graph
void printShortestRoute(Graph graph, char* source, char* destination);
//quickest route is always by car, no need to run algorithm here

int getCityID(Graph graph, char * city);
char * getCityName(Graph graph, int cityID);

void MENU_USER(){
	printf("\n\tUS & Canada cities flight suggestion system\n");
	printf("---------------------------------\n\n");
	printf("1. PRINT ALL FLIGHTS\n\n");
	printf("2. PRINT ALL FLIGHT FROM A CITY\n\n");
	printf("3. FIND SHORTEST TRAVEL ROUTE BETWEEN TWO CITIES\n\n");
	printf("4. FIND QUICKEST TRAVEL ROUTE BETWEEN TWO CITIES\n\n");
	printf("---------------------------------\n\n");
}
void MENU_ADMINISTRATOR(){
	printf("\n\tUS & Canada cities flight admin system\n");
	printf("---------------------------------\n\n");
	printf("1. PRINT ALL FLIGHTS\n\n");
	printf("2. PRINT ALL FLIGHT FROM A CITY\n\n");
	printf("3. SORT ALL DISTANCES BETWEEN TWO CITIES\n\n");
	printf("4. MODIFY MAP: ADD FLIGHT\n\n");
	printf("---------------------------------\n\n");
}
	
int main(){

}







