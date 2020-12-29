#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CITY_COUNT_INITIAL 312 
#define CITY_NAME_LENGTH 25
#define CITY_LIST_FILE
#define FLIGHT_LIST_FILE
#define DISTANCE_LIST_FILE
#define CAR_SPEED 1
#define PLANE_SPEED 100
#define INVALID_CITY_ID -1
#define INVALID_CITY_NAME "INVALID"
#define INVALID_DISTANCE

void swap(int* a, int i, int j);

int cmpfunc(const void* a, const void* b) 
{
	return (*(int*)a - *(int*)b);
}

typedef struct {
	int cityCount;
	char** cities;
    int** hasflight;
    int** distance;
    int** connections;
} Graph;
// linh:
Graph createGraph(int cityCount);
void readGraphFromFile(Graph graph);
void dropGraph(Graph graph);
// make change to text file also
void addFlight(Graph graph, char * source, char * destination);
void removeFLight(Graph graph, char * source, char * destination);

// hai:
// generate flight file
void printShortestRoute(Graph graph, char* source, char* destination);
void printAllFlight(Graph graph);
void printAllFlightFromCity(Graph graph, char * city);

// khoi:
int getCityID(Graph graph, char* city)
{
	for (int i = 0; i < graph.cityCount; i++)
		if (strcmp(graph.cities[i], city) == 0)
			return i;
	return INVALID_CITY_ID;
}

char * getCityName(Graph graph, int cityID)
{
	for (int i = 0; i < graph.cityCount; i++)
		if (i == cityID)
			return graph.cities[i];
	return INVALID_CITY_NAME
}

void sortDistance(Graph graph) //, int * output, int size)
{
	//convert distance to 1D array
	int size = graph.cityCount * graph.cityCount;
	int * output = (int*)malloc(sizeof(int) * size);
	int i, j;
	for (i = 0; i < graph.cityCount; i++)
		for (j = 0; j < graph.cityCount; j++)
			output[graph.cityCount * i + j] = graph.distance[i][j];

	//sort
	qsort(output, size - 1, sizeof(int), cmpfunc);
}

// text input preprocess
void MENU_USER(){
	printf("\n\tUS & Canada cities flight suggestion system\n");
	printf("---------------------------------\n\n");
	printf("1. PRINT ALL FLIGHTS\n\n");
	printf("2. PRINT ALL FLIGHT FROM A CITY\n\n");
	printf("3. FIND SHORTEST TRAVEL ROUTE BETWEEN TWO CITIES\n\n");
	printf("---------------------------------\n\n");
}

void MENU_ADMINISTRATOR(){
	printf("\n\tUS & Canada cities flight admin system\n");
	printf("---------------------------------\n\n");
	printf("1. PRINT ALL FLIGHTS\n\n");
	printf("2. PRINT ALL FLIGHT FROM A CITY\n\n");
	printf("3. SORT ALL DISTANCES BETWEEN TWO CITIES\n\n");
	printf("4. MODIFY MAP: ADD/REMOVE FLIGHT\n\n");
	printf("---------------------------------\n\n");
}
	
int main(){

}







