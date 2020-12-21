cities_file_name = 'usca312_name.txt'
cities_cleaned_file_name = 'usca312_name_cleaned.txt'

cities_file = open(cities_file_name)
content = cities_file.readlines()
cities_cleaned_file = open(cities_cleaned_file_name, "w")

cityWithAirportIndexList = []
appearedStateCodeList = []
countComment = 0
for i in range(len(content)):
	if content[i][0]!='#':
		content[i] = content[i].rstrip()
		result = content[i].split(",")
		city = result[0].strip()
		stateCode = result[1].strip()
		cities_cleaned_file.write(city+','+stateCode+'\n');
cities_cleaned_file.close()