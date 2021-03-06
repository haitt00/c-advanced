#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CITY_COUNT_INITIAL 3//12 
#define CITY_NAME_LENGTH 20
#define CITY_LIST_FILE "city_test.txt"
#define FLIGHT_LIST_FILE "flight_test.txt"
#define DISTANCE_LIST_FILE "dist_test.txt"
#define INVALID_CITY_ID -1
#define CAR_SPEED 1
#define PLANE_SPEED 100

typedef struct {
	int cityCount;
	char** cities;
    int** hasflight;
    int** distance;
    int** connections;
} Graph;
// linh:
Graph createGraph(int cityCount);
Graph readGraphFromFile();
void dropGraph(Graph graph);
// make change to flight.txt
void addFlight(Graph graph, char * source, char * destination);
void removeFLight(Graph graph, char * source, char * destination);

// return INVALID_CITY_ID if not found
int getCityID(Graph graph, char * city);

int main(){
    // test purpose
    Graph g = readGraphFromFile();
    if(g.cities == 0){
        printf("Something went wrong with reading input file!\n");
        return 0;
    }
    dropGraph(g);
}

Graph createGraph(int cityCount){
    Graph g;
    g.cityCount = cityCount;
    g.cities = (char**)malloc(cityCount * sizeof(char*));
    g.connections = (int**)malloc(cityCount * sizeof(int*));
    g.distance = (int**)malloc(cityCount * sizeof(int*));
    g.hasflight = (int**)malloc(cityCount * sizeof(int*));
    for(int i = 0; i < cityCount; i++){
        g.cities[i] = (char*)malloc(CITY_NAME_LENGTH * sizeof(char));
        g.connections[i] = (int*)malloc(CITY_NAME_LENGTH * sizeof(int));
        g.distance[i] = (int*)malloc(CITY_NAME_LENGTH * sizeof(int));
        g.hasflight[i] = (int*)malloc(CITY_NAME_LENGTH * sizeof(int));
    }
    return g;
}

void dropGraph(Graph g){
    for(int i = 0; i < g.cityCount; i++){
        free(g.cities[i]);
        free(g.connections[i]);
        free(g.distance[i]);
        free(g.hasflight[i]);
    }
    free(g.cities);
    free(g.connections);
    free(g.distance);
    free(g.hasflight);
}


// TODO: if read file failed
Graph readGraphFromFile(){
    Graph g = createGraph(CITY_COUNT_INITIAL);
    
    char tempString[CITY_NAME_LENGTH];  // lưu giá trị đang đọc
    int tempChar = 0;                   // đang đọc đến giá trị nào của tempString rồi
    char currentChar = ' ';             // giá trị đọc xuyên file, reset thành ' ' mỗi khi đọc file mới  
    FILE * fp;

    // read city name
    fp = fopen(CITY_LIST_FILE, "rt");
    if(fp == NULL){
        printf("Failed to read List of City\n");
        return createGraph(0);
    }
    int cityID = 0;
    while (currentChar != EOF){
        currentChar = fgetc(fp);
        if(currentChar == '\n'){
            tempString[tempChar] = '\0';
            strcpy(g.cities[cityID], tempString);   
            printf("%s\n",g.cities[cityID]); // debug purpose
            tempChar = 0;
            cityID++;
        }else{
            tempString[tempChar] = currentChar;
            tempChar++;
        }
    }
    fclose(fp);

    // reset
    tempChar = 0;
    currentChar = ' ';
    int currentSource = 0;              // id của source
    int currentDestination = 0;         // id của destination

    // read distance
    fp = fopen(DISTANCE_LIST_FILE, "rt");
    if(fp == NULL){
        printf("Failed to read List of Distance\n");
        return createGraph(0);
    }
    int rightAfterNumber = 0;
    while (currentChar != EOF){
        currentChar = fgetc(fp);
        if(currentChar < 48 || currentChar > 57){ // not digit
            if (rightAfterNumber){
                rightAfterNumber = 0;
                tempString[tempChar] = '\0';
                int dist = atoi(tempString);
                if(currentDestination >= CITY_COUNT_INITIAL){
                    currentSource++;
                    currentDestination = 0;
                }
                g.distance[currentSource][currentDestination] = dist;
                // printf("source %d destination %d distance %d\n", currentSource, currentDestination, dist); // debug purpose
                currentDestination++;
                tempChar = 0;
            }
        }else{
            rightAfterNumber = 1;
            tempString[tempChar] = currentChar;
                tempChar++;
        }
    }
    fclose(fp);

    // reset
    tempChar = 0;
    currentChar = ' ';
    currentSource = 0;
    currentDestination = 0;
    rightAfterNumber = 0;

    // read flight
    fp = fopen(FLIGHT_LIST_FILE, "rt");
    if(fp == NULL){
        printf("Failed to read List of Flight\n");
        return createGraph(0);
    }
    while (currentChar != EOF){
        currentChar = fgetc(fp);
        if(currentChar < 48 || currentChar > 57){ // not digit
            if (rightAfterNumber){
                rightAfterNumber = 0;
                tempString[tempChar] = '\0';
                int hasFlight = atoi(tempString);
                if(currentDestination >= CITY_COUNT_INITIAL){
                    currentSource++;
                    currentDestination = 0;
                }
                g.hasflight[currentSource][currentDestination] = hasFlight;
                // printf("source %d destination %d flight %d\n", currentSource, currentDestination, hasFlight); // debug purpose
                currentDestination++;
                tempChar = 0;
            }
        }else{
            rightAfterNumber = 1;
            tempString[tempChar] = currentChar;
                tempChar++;
        }
    }
    fclose(fp);

    // make connection
    for(int source = 0; source < CITY_COUNT_INITIAL; source++)
        for(int dest = 0; dest < CITY_COUNT_INITIAL; dest++){
                g.connections[source][dest] = g.distance[source][dest] / ((g.hasflight[source][dest]) ? PLANE_SPEED : CAR_SPEED);
                //printf("source %d destination %d connection %d\n", source, dest, g.connections[source][dest]); // debug purpose
        }
    return g;
}