#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <math.h>

#define CITY_COUNT_INITIAL 312 
#define CITY_NAME_LENGTH 25
#define CITY_LIST_FILE "res/usca312_name_cleaned.txt"
#define FLIGHT_LIST_FILE "res/usca312_flight_initial.txt"
#define FLIGHT_LIST_FILE_MODI "res/usca312_flight_modified.txt"
#define DISTANCE_LIST_FILE "res/usca312_dist.txt"
#define CAR_SPEED 1
#define PLANE_SPEED 100
#define INVALID_CITY_ID -1
#define INVALID_CITY_NAME "INVALID"
#define INVALID_DISTANCE
#define INFINITE 1000000000

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
// make change to text file also
void addFlight(Graph g, char * source, char * destination);

void removeFlight(Graph g, char * source, char * destination);

// hai:
// generate flight file
void printShortestRoute(Graph graph, char* source, char* destination);
void printAllFlight(Graph graph);
void printAllFlightFromCity(Graph graph, char * city);

// khoi:
int getCityID(Graph graph, char * city);
char * getCityName(Graph graph, int cityID);
void sortDistance(Graph graph);
// text input preprocess
void MAIN_MENU(){
	printf("\n\tUS & Canada cities flight system\n");
	printf("---------------------------------\n\n");
	printf("1. OPEN MENU FOR USER\n\n");
	printf("2. OPEN MENU FOR ADMIN\n\n");
	printf("3. EXIT\n\n");
	printf("---------------------------------\n\n");
}
void MENU_USER(){
	printf("\n\tUS & Canada cities flight suggestion system\n");
	printf("---------------------------------\n\n");
	printf("1. PRINT ALL FLIGHTS\n\n");
	printf("2. PRINT ALL FLIGHT FROM A CITY\n\n");
    printf("3. PRINT ALL FLIGHT TO A CITY\n\n");
    printf("4. PRINT ALL FLIGHT FROM A STATE\n\n");
    printf("5. PRINT ALL FLIGHT TO A STATE\n\n");
	printf("6. FIND SHORTEST TRAVEL ROUTE BETWEEN TWO CITIES\n\n");
    printf("7. EXIT\n\n");
	printf("---------------------------------\n\n");
}
void MENU_ADMINISTRATOR(){
	printf("\n\tUS & Canada cities flight admin system\n");
	printf("---------------------------------\n\n");
    printf("0. PRINT ALL CITIES\n\n");
	printf("1. PRINT ALL FLIGHTS\n\n");
	printf("2. PRINT ALL FLIGHTS FROM A CITY\n\n");
    printf("3. PRINT ALL FLIGHT TO A CITY\n\n");
	printf("4. SORT ALL DISTANCES BETWEEN TWO CITIES\n\n");
	printf("5. MODIFY MAP: ADD/REMOVE FLIGHT\n\n");
    printf("6. PRINT ALL CITIES IN A STATE\n\n");
    printf("7. EXIT\n\n");
	printf("---------------------------------\n\n");
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
        g.connections[i] = (int*)malloc(cityCount * sizeof(int));
        g.distance[i] = (int*)malloc(cityCount * sizeof(int));
        g.hasflight[i] = (int*)malloc(cityCount * sizeof(int));
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
void trimWhiteSpace(char* input){
    int i, j;
    for(i = 0; input[i]==' '||input[i]=='\t';i++);
    
    for(j = 0; input[i]; i++){
        input[j++]=input[i];
    }
    input[j] = '\0';
    for(i=0;input[i]!='\0';i++){
        if(input[i]!=' ' && input[i]!='\t'){
            j = i;
        }
    }
    input[j+1] = '\0';
}
void toCityName(char* input){
    input = strlwr(input);
    for(int i = 0; i < strlen(input); i++){
        if(i == 0){
            input[i] = toupper(input[i]);            
        }
        else if(input[i-1] == ' '){
            input[i] = toupper(input[i]);
        }
    }
}
void getStateCodeFromUser(char * stateCode){
    printf("Enter The State Code: ");
    scanf(" %s[^\n]", stateCode);
    trimWhiteSpace(stateCode);
    for(int i = 0; i < strlen(stateCode); i++)
            stateCode[i] = toupper(stateCode[i]);            
}
void getCityNameFromUser(char * prompt1, char* prompt2, char* cityName){
    char stateCode[CITY_NAME_LENGTH];
    printf("%s: ", prompt1);
    scanf(" %s[^\n]", cityName);
    trimWhiteSpace(cityName);
    toCityName(cityName);
    printf("%s: ", prompt2);
    // getch();
    scanf(" %s[^\n]", stateCode);
    trimWhiteSpace(stateCode);
    strcat(cityName, ", ");
    strcat(cityName, strupr(stateCode));
}
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
            // printf("%s\n",g.cities[cityID]); // debug purpose
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
                g.connections[source][dest] = ceil((double)g.distance[source][dest] / ((g.hasflight[source][dest]) ? PLANE_SPEED : CAR_SPEED));
                // printf("source %d destination %d connection %d\n", source, dest, g.connections[source][dest]); // debug purpose
        }
    return g;
}
int djisktra(int** cost,int source,int target, int * reversedPath)
{
    // printf("debug source: %d\n", source);
    // printf("debug target: %d\n", target);
    // printf("debug gostraight: %d\n", cost[source][target]);
    int dist[CITY_COUNT_INITIAL],prev[CITY_COUNT_INITIAL],selected[CITY_COUNT_INITIAL]={0},i,m,min,start,d,j;
    for(i=0;i< CITY_COUNT_INITIAL;i++)
    {
        dist[i] = INFINITE;
        prev[i] = -1;
    }
    start = source;
    selected[start]=1;
    dist[start] = 0;
    while(selected[target] ==0)
    {
        min = INFINITE;
        m = 0;
        for(i=0;i< CITY_COUNT_INITIAL;i++)
        {
            d = dist[start] +cost[start][i];
            if(d< dist[i]&&selected[i]==0)
            {
                dist[i] = d;
                prev[i] = start;
            }
            if(min>dist[i] && selected[i]==0)
            {
                min = dist[i];
                m = i;
            }
        }
        start = m;
        selected[start] = 1;
    }
    start = target;
    j = 0;
    while(start != -1)
    {
        // printf("start: %d\n", start); //debug
        reversedPath[j++] = start;
        // printf("append: %d\n", reversedPath[j-1]); //debug
        start = prev[start];
    }
    reversedPath[j] = -1;
    return dist[target];
}
void printShortestRoute(Graph graph, char* source, char* destination){
    if(strcmp(source, destination)==0){
        printf("The source and destination are the same!\n");
    }
    else{
        int idSource = getCityID(graph, source);
        int idDestination = getCityID(graph, destination);
        int reversedPath[CITY_COUNT_INITIAL];
        int result = djisktra(graph.connections, idSource, idDestination, reversedPath);
        int length = 0;
        while(reversedPath[length]!=-1){
            length++;
        }
        printf("Shortest route from %s to %s:\n", source, destination);
        for(int i = length - 1; i >= 0; i--){
            printf("(%s) ", getCityName(graph, reversedPath[i]));
            if(i>0){
                if(graph.hasflight[reversedPath[i]][reversedPath[i-1]]){
                    printf("<<<-- %d -->>> ", graph.connections[reversedPath[i]][reversedPath[i-1]]);
                }
                else{
                    printf("-- %d -- ", graph.connections[reversedPath[i]][reversedPath[i-1]]);
                }
            }
        }
        printf("\nTotal time: %d\n", result);
    }
    system("pause");
}
void printAllFlight(Graph graph){
    printf("All flights (only displayng first 100):\n");
    int count = 1;
    for(int i = 0; i < CITY_COUNT_INITIAL; i++){
        for(int j = 0; j < i; j++){
            if(count > 100){
                break;
            }
            if(graph.hasflight[i][j]==1){
            printf("%d. %s <<<------>>> %s\n", count++, getCityName(graph, i), getCityName(graph, j));
            }
        }
    }
    system("pause");
}

void printAllCity(Graph graph){
    printf("All cities:\n");
    int count = 1;
    for(int i = 0; i < CITY_COUNT_INITIAL; i++){
            printf("%d. %s\n", count++, graph.cities[i]);
        }
    system("pause");
}
void printAllFlightFromCity(Graph graph, char * city){
    printf("All flights from %s: \n", city);
    int cityId = getCityID(graph, city);
    int count = 1;
    for(int i = 0; i < CITY_COUNT_INITIAL; i++){
        if(graph.hasflight[i][cityId]==1){
            printf("%d. <<<------>>> %s\n", count++, getCityName(graph, i));
        }
    }
    if(count == 1)
        printf("There is none\n");
    system("pause");
}

void printAllFlightToCity(Graph graph, char * city){
    printf("All flights to %s: \n", city);
    int cityId = getCityID(graph, city);
    int count = 1;
    for(int i = 0; i < CITY_COUNT_INITIAL; i++){
        if(graph.hasflight[cityId][i]==1){
            printf("%d. <<<------>>> %s\n", count++, getCityName(graph, i));
        }
    }
    if(count == 1)
        printf("There is none\n");
    system("pause");
}

//khoi
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
    return INVALID_CITY_NAME;
}
void swap(int* a, int * order, int i, int j){
    int temp = a[i];
    a[i] = a[j];
    a[j] = temp;
    int orderi = order[i];
    int orderj = order[j];
    order[i] = orderj;
    order[j] = orderi;
}

int cmpfunc(const void* a, const void* b) 
{
    return (*(int*)a - *(int*)b);
}
void quickSort_3way(int a[], int order[], int l, int r)
{
	if (r <= l)
		return;
	int i = l - 1, j = r, k;
	int p = l - 1, q = r;
	int v = a[r];
	while (1)
	{
		while (a[++i] < v);
		while (a[r] < a[--j])
			if (j == l)
				break;
		if (i >= j)
			break;
		swap(a, order, i, j);
		if (a[i] == a[r])
			swap(a, order, ++p, i);
		if (a[j] == a[r])
			swap(a, order, --q, j);
	}
	swap(a, order, i, r);
	j = i - 1;
	i = i + 1;
	for (k = l; k <= p; k++)
		swap(a, order, k, j--);
	for (k = r - 1; k >= q; k--)
		swap(a, order, k, i++);
	quickSort_3way(a, order, l, j);
	quickSort_3way(a, order, i, r);
}
void sortDistance(Graph graph)//, int * output, int size)
{
    //convert distance to 1D array
    int size = graph.cityCount * graph.cityCount;
    int * output = (int*)malloc(sizeof(int) * size);
    //save the initial order in a new array
    int * order = (int*)malloc(sizeof(int) * size);
    int i, j;
    for (i = 0; i < graph.cityCount; i++)
        for (j = 0; j < graph.cityCount; j++){
            output[graph.cityCount * i + j] = graph.distance[i][j];
            order[graph.cityCount * i + j] = graph.cityCount * i + j;
        }
    // int cityStart[] = 
    //sort
    // qsort(output, size - 1, sizeof(int), cmpfunc);
    quickSort_3way(output, order, 0, size - 1);
    for (i = 0; i < size; i++){
        //printf("%d\n", order[i]);
		printf("%s - %s: %d (km)\n", graph.cities[order[i] / graph.cityCount], graph.cities[order[i] % graph.cityCount], output[i]);
    }
    system("pause");
}

void removeFlight(Graph g, char * source, char * destination){
    int sourceID = getCityID(g, source); 
    int destID = getCityID(g, destination);
    if( g.hasflight[sourceID][destID] == 0 ){
        printf("Flight does not exist!\n");
        return;
    } 
    g.hasflight[sourceID][destID] = 0;
    g.hasflight[destID][sourceID] = 0;
    FILE * f = fopen(FLIGHT_LIST_FILE_MODI, "w");
    for(int i = 0; i < g.cityCount; i ++){
        for(int j = 0; j < g.cityCount; j++){
            fprintf(f, "%d ", g.hasflight[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
    printf("Remove flight succesfully!\n");
}


void addFlight(Graph g, char * source, char * destination){
    int sourceID = getCityID(g, source); 
    int destID = getCityID(g, destination);
    if( g.hasflight[sourceID][destID] == 1 ){
        printf("Flight's already existed!\n");
        return;
    } 
    g.hasflight[sourceID][destID] = 1;
    g.hasflight[destID][sourceID] = 1;
    FILE * f = fopen(FLIGHT_LIST_FILE_MODI, "w");
    for(int i = 0; i < g.cityCount; i ++){
        for(int j = 0; j < g.cityCount; j++){
            fprintf(f, "%d ", g.hasflight[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
    printf("Add flight succesfully!\n");
}

// all city in a state
// all flight from a state
// all flight to a state

int getCityList(Graph graph, char* stateCode, int * list){
    int res = 0;
    for (int i = 0; i < graph.cityCount; i++){
        if(strstr(graph.cities[i], stateCode) != NULL) {
            list[res++] = i;
        }
    }
    return res;
}

void printAllCityInState(Graph g, char * stateCode){
    int *cityList = (int*)malloc(sizeof(int)*g.cityCount);
    int size = getCityList(g, stateCode, cityList);
    if(size == 0)
        printf("Invalid State Code\n");
    else{
        printf("List of city in state with code %s\n", stateCode);
        int count = 1;
        for(int i = 0; i < size; i++){
            printf("%d. %s\n", count++, getCityName(g, cityList[i]));
        }
    }
}
 
void printAllFlightFromState(Graph g, char * stateCode){
    int *cityList = (int*)malloc(sizeof(int)*g.cityCount);
    int size = getCityList(g, stateCode, cityList);
    if(size == 0)
        printf("Invalid State Code\n");
    else{
        printf("List of flight from state with code %s\n", stateCode);
        int count = 1;
        for(int i = 0; i < size; i++){
        for(int j = 0; j < cityList[i]; j++){
            if(g.hasflight[cityList[i]][j]==1){
            printf("%d. %s <<<------>>> %s\n", count++, getCityName(g, cityList[i]), getCityName(g, j));
            }
        }
       
        }
         if (count == 1){
            printf("There is none\n");
        }
    }
}

void printAllFlightToState(Graph g, char * stateCode){
    int *cityList = (int*)malloc(sizeof(int)*g.cityCount);
    int size = getCityList(g, stateCode, cityList);
    if(size == 0)
        printf("Invalid State Code\n");
    else{
        printf("List of flight to state with code %s\n", stateCode);
        int count = 1;
        for(int i = 0; i < size; i++){
            for(int j = 0; j < cityList[i]; j++){
                if(g.hasflight[j][cityList[i]]==1){
                printf("%d. %s <<<------>>> %s\n", count++, getCityName(g, j), getCityName(g, cityList[i]));
                }
            }
        }
        if (count == 1){
            printf("There is none\n");
        }
    }
}


int main(){
	int choiceMenu;
	int choice;
    Graph g = readGraphFromFile();
    // system("pause");
	while(1){
		system("cls");
		MAIN_MENU();
		printf("Enter choice: ");
		scanf("%d", &choiceMenu);	
		if(choiceMenu==1){
            while(1){
                system("cls");
                MENU_USER();
                printf("Enter choice: ");
                scanf("%d", &choice);
                if(choice==1){
                    printAllFlight(g);
                }
                else if(choice==2){
                    char cityName[CITY_NAME_LENGTH];
                    getCityNameFromUser("Enter city name", "Enter state code", cityName);
                    if(getCityID(g, cityName)==INVALID_CITY_ID){
                        printf("City not found!\n");
                        system("pause");
                    }
                    else{
                        printAllFlightFromCity(g, cityName);
                    }
                }else if(choice==3){
                    char cityName[CITY_NAME_LENGTH];
                    getCityNameFromUser("Enter city name", "Enter state code", cityName);
                    if(getCityID(g, cityName)==INVALID_CITY_ID){
                        printf("City not found!\n");
                        system("pause");
                    }
                    else{
                        printAllFlightToCity(g, cityName);
                    }
                }else if(choice==4){
                    // print all flight from a state
                    char stateCode[CITY_NAME_LENGTH];
                    getStateCodeFromUser(stateCode);
                    printAllFlightFromState(g, stateCode);
                    system("pause");
                }else if(choice==5){
                    // print all flight to a state
                    char stateCode[CITY_NAME_LENGTH];
                    getStateCodeFromUser(stateCode);
                    printAllFlightToState(g, stateCode);
                    system("pause");
                }
                else if(choice==6){
                    char sourceCityName[CITY_NAME_LENGTH];
                    char destinationCityName[CITY_NAME_LENGTH];
                    getCityNameFromUser("Enter source city name", "Enter source state code", sourceCityName);
                    if(getCityID(g, sourceCityName)==INVALID_CITY_ID){
                        printf("City not found!\n");
                        system("pause");
                        continue;
                    }
                    getCityNameFromUser("Enter destination city name", "Enter destination state code", destinationCityName);
                    if(getCityID(g, destinationCityName)==INVALID_CITY_ID){
                        printf("City not found!\n");
                        system("pause");
                        continue;
                    }
                    printShortestRoute(g, sourceCityName, destinationCityName);
                }
                else if(choice==7){
                    break;
                }
            }	
		}
        else if(choiceMenu==2){
            while(1){
                system("cls");
                MENU_ADMINISTRATOR();
                printf("Enter choice: ");
                scanf("%d", &choice);
                if(choice==0){
                    printAllCity(g);
                }
                else if(choice==1){
                    printAllFlight(g);
                }
                else if(choice==2){
                    char cityName[CITY_NAME_LENGTH];
                    getCityNameFromUser("Enter city name", "Enter state code", cityName);
                    if(getCityID(g, cityName)==INVALID_CITY_ID){
                        printf("City not found!\n");
                        system("pause");
                    }
                    printAllFlightFromCity(g, cityName);
                }else if(choice==3){
                    char cityName[CITY_NAME_LENGTH];
                    getCityNameFromUser("Enter city name", "Enter state code", cityName);
                    if(getCityID(g, cityName)==INVALID_CITY_ID){
                        printf("City not found!\n");
                        system("pause");
                    }
                    printAllFlightToCity(g, cityName);
                }
                else if(choice==4){
                    sortDistance(g);
                }
                else if(choice==5){
                    while(1){
                    system("cls");
                    printf("---------------------------------\n\n");
                    printf("1. ADD FLIGHT TO DATABASE\n\n");
                    printf("2. REMOVE FLIGHT FROM DATABASE\n\n");
                    printf("3. EXIT\n\n");
                    printf("---------------------------------\n\n");
                    int modiChoice;
                    printf("Enter choice: ");
		            scanf("%d", &modiChoice);	
                    if(modiChoice == 1){
                        char sourceCityName[CITY_NAME_LENGTH];
                        char destinationCityName[CITY_NAME_LENGTH];
                        getCityNameFromUser("Enter source city name", "Enter source state code", sourceCityName);
                        if(getCityID(g, sourceCityName)==INVALID_CITY_ID){
                            printf("City not found!\n");
                            system("pause");
                            continue;
                        }
                        getCityNameFromUser("Enter destination city name", "Enter destination state code", destinationCityName);
                        if(getCityID(g, destinationCityName)==INVALID_CITY_ID){
                            printf("City not found!\n");
                            system("pause");
                            continue;
                        }
                        addFlight(g, sourceCityName, destinationCityName);
                        system("pause");
                    }else if(modiChoice == 2){
                        char sourceCityName[CITY_NAME_LENGTH];
                        char destinationCityName[CITY_NAME_LENGTH];
                        getCityNameFromUser("Enter source city name", "Enter source state code", sourceCityName);
                        if(getCityID(g, sourceCityName)==INVALID_CITY_ID){
                            printf("City not found!\n");
                            system("pause");
                            continue;
                        }
                        getCityNameFromUser("Enter destination city name", "Enter destination state code", destinationCityName);
                        if(getCityID(g, destinationCityName)==INVALID_CITY_ID){
                            printf("City not found!\n");
                            system("pause");
                            continue;
                        }
                        removeFlight(g, sourceCityName, destinationCityName);
                        system("pause");
                    }else if(modiChoice == 3){
                        break;
                    }
                    }
                }
                else if(choice==6){
                    // print all cities in a state
                    char stateCode[CITY_NAME_LENGTH];
                    getStateCodeFromUser(stateCode);
                    printAllCityInState(g, stateCode);
                    system("pause");
                }
                else if(choice==7){
                    break;
                }
            }
        }
        else if(choiceMenu==3){
            // printf("Exiting\n");
            // dropGraph(g);
            exit(0);
        }
	}
	return 0;
	// printf("hello world\n");
	// Graph g = readGraphFromFile();
	// printf("%d\n", g.cityCount);
	// printf("%s\n", g.cities[0]);

}

