import random

cities_file_name = 'usca312_name.txt'
flights_file_name = 'usca312_flight_initial.txt' #output file

cities_file = open(cities_file_name)
content = cities_file.readlines()
flights_file = open(flights_file_name, "w");
NUMBER_OF_CITIES = 312

#get the indexes of cities with airport aka the first occured city of a state
cityWithAirportIndexList = []
appearedStateCodeList = []
countComment = 0
for i in range(len(content)):
	if content[i][0]=='#':
		countComment+=1
	else:
		content[i] = content[i].rstrip()
		stateCode = content[i].split(",")[1].strip();
		if(stateCode not in appearedStateCodeList):
			print(stateCode)
			cityWithAirportIndexList.append(i-countComment)
			appearedStateCodeList.append(stateCode)
print(cityWithAirportIndexList);

#generate the flights matrix
random.seed(99);
flights_matrix = [[0 for i in range(NUMBER_OF_CITIES)] for j in range(NUMBER_OF_CITIES)]
for i in range(NUMBER_OF_CITIES):
	for j in range(i):
		if i in cityWithAirportIndexList and j in cityWithAirportIndexList:
			if random.random() < 0.5:
				flights_matrix[i][j] = 1;
				flights_matrix[j][i] = 1;
				print(i, j)

#generate the output file
for i in range(NUMBER_OF_CITIES):
	for j in range(NUMBER_OF_CITIES):
		flights_file.write(str(flights_matrix[i][j])+'\t');
	flights_file.write('\n');
flights_file.close()