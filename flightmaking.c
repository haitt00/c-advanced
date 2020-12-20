#include <stdio.h>
#include "flightmaking.h"

char airportNameList[MAX_AIRPORT_LIST_SIZE][MAX_AIRPORT_NAME_SIZE];
int airportListSize = 0;
FILE *fp;

int readAirportList(char * fileName){
    fp = fopen(fileName, "rt");
    if(fp == NULL){
        printf("Failed to read file\n");
        return IO_ERROR;
    }
    int currentCharInLine = 0;
    char currentCharInFile = ' ';
    while(currentCharInFile != EOF){
        currentCharInFile = fgetc(fp);
        if(currentCharInFile != '\n'){
            airportNameList[airportListSize][currentCharInLine] = (char)currentCharInFile;
            currentCharInLine++;
        }else{
            airportNameList[airportListSize][currentCharInLine] = '\0';
            airportListSize++;
            currentCharInLine = 0;
        }
    }
    fclose(fp);
    printf("Done reading file\n");
    return IO_SUCCESS;
}

int makeFlightList(char *fileName){
    fp = fopen(fileName, "w");
    if(fp == NULL){
        printf("Something went wrong\n");
        return IO_ERROR;
    }
    // make random ticket price between airports
    for(int i = 0; i < airportListSize; i++){
        for(int j = 0; j < airportListSize; j++){
            if(i!=j){
                int random = rand() % 7;
                if(random == 1 || random == 5){
                    int ticket_price = rand() % 981 + 20;
                    fprintf(fp, "%s-%s-%d\n", &airportNameList[i], &airportNameList[j], ticket_price);
                }
                else if(random == 3){
                    int ticket_price = 10000;
                    fprintf(fp, "%s-%s-%d\n", &airportNameList[i], &airportNameList[j], ticket_price);
                }
            }
        }
    }
    fclose(fp);
    printf("Done making file\n");
    return IO_SUCCESS;
}