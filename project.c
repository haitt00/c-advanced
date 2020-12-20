#include <stdio.h>
#include "jrb.h"
#include "dllist.h"
#include <stdlib.h>
#include <string.h>
#include "flightmaking.h"
#define INFINITIVE_VALUE 10000000
#define INVALID_NAME "none"
#define INPUT_FILE "flight10.txt"
#define DESTINATION_NUMBER 5

typedef struct {
    JRB airports;
    JRB flights;
} Graph;

Graph createGraph();
void dropGraph(Graph graph);
void printAllFlightAndAirport(Graph graph);
void printAllFlightFromAirport(Graph graph, char * airport);
void printTopCheapestFlight(Graph graph, int top);

void addNewAirport(Graph graph, char * airport, int currentID);
int getAiportID(Graph graph, char * airport);
char * getAiportName(Graph graph, int airportID);

void addFlight(Graph graph, char * source, char * destination, int ticket_price, int * currentID);
// do we need this?
// int availableFlight(Graph graph, char * source, char * destination);
int addFlightByFile(Graph graph, char * fileName);

int main(){
    // test purpose
    Graph g = createGraph();
    if(addFlightByFile(g, INPUT_FILE) == IO_ERROR){
        printf("Failed to read file");
        return IO_ERROR;
    }

    printf("%d", getAiportID(g, "Cadjehoun Airport"));
    // int currentID = 12;
    // addFlight(g, "ha", "noi", 12, &currentID);
    // addFlight(g, "ha", "lan", 12, &currentID);
    //  addFlight(g, "lan", "69", 12, &currentID);
    //   addFlight(g, "69", "lan", 12, &currentID);
    //  printf("%d", currentID);
    dropGraph(g);
}

Graph createGraph(){
    Graph g;
    g.airports = make_jrb();
    g.flights = make_jrb();
    return g;
}
void dropGraph(Graph graph){
    jrb_free_tree(graph.airports);
    jrb_free_tree(graph.flights);
}

int getAiportID(Graph graph, char * airportName){
    JRB tmp;
    jrb_traverse(tmp, graph.airports){
        if(strcmp(airportName, jval_s(tmp->val)) == 0)
            return jval_i(tmp->key);}
    return INFINITIVE_VALUE;
}

char * getAiportName(Graph graph, int airportID){
    JRB tmp = jrb_find_int(graph.airports, airportID);
    if(tmp == NULL)
        return INVALID_NAME;
    return jval_s(tmp->val);
}

void addNewAirport(Graph graph, char * airport, int currentID){
    jrb_insert_int(graph.airports, currentID, new_jval_s(airport));
    JRB tmp = make_jrb();
    jrb_insert_int(graph.flights, currentID, new_jval_v(tmp));
}

// đang bị bug
void addFlight(Graph graph, char * source, char * destination, int ticket_price, int * currentID){
    printf("current id %d\n", *currentID);
    JRB tmp;
    int getSourceID = getAiportID(graph, source);
    printf("%s %d\n", source, getSourceID);
    if(getSourceID == INFINITIVE_VALUE){
        addNewAirport(graph, source, *currentID);
        getSourceID = *currentID;
        (*currentID)++;
    }
    int getDestinationID = getAiportID(graph, destination);
    printf("%s %d\n", destination, getDestinationID);
    if(getDestinationID == INFINITIVE_VALUE){
        addNewAirport(graph, destination, *currentID);
        getDestinationID = *currentID;
        (*currentID)++;
    }
    tmp = jrb_find_int(graph.flights, getSourceID);
    tmp = (JRB)jval_v(tmp->val);
    jrb_insert_int(tmp, getDestinationID, new_jval_i(ticket_price));
    tmp = jrb_find_int(tmp, getDestinationID);
    printf("current id %d\n", *currentID);
}

// đang bị bug
int addFlightByFile(Graph graph, char * fileName){
    // open file
    FILE * fp = fopen(fileName, "rt");
    if(fp == NULL)
        return IO_ERROR;

    // init
    char source[MAX_AIRPORT_NAME_SIZE];
    char destination[MAX_AIRPORT_NAME_SIZE];
    int currentID = 0;
    int ticketPrice = 0;
    int dash = 0;   // for parsing a line
    char tempValue[MAX_AIRPORT_NAME_SIZE];
    int tempValueChar = 0;
    char currentChar = ' ';

    // run through the file
    while(currentChar != EOF){
        currentChar = fgetc(fp);
        if(currentChar == '\n'){
            tempValue[tempValueChar] = '\0';
            ticketPrice = atoi(tempValue);
            printf("source: \"%s\" destination: \"%s\" price: \"%d\"\n", source, destination, ticketPrice);
            addFlight(graph, source, destination, ticketPrice, &currentID);
            dash = 0;
            tempValueChar = 0;
        }else{
            // string split
            if(currentChar != '-'){
                tempValue[tempValueChar] = currentChar;
                tempValueChar++;
            }else{
                if(dash == 0){          // meaning this is a source airport
                    tempValue[tempValueChar] = '\0';
                    strcpy(source, tempValue);
                    dash++;
                    tempValueChar = 0;  // reset
                }
                else if(dash == 1){     // meaning this is a destination airport
                    tempValue[tempValueChar] = '\0';
                    strcpy(destination, tempValue);
                    dash++;
                    tempValueChar = 0;  // reset
                }
            }
        }
    }
    fclose(fp);
    return IO_SUCCESS;
}

