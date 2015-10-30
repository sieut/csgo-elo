inF = open('data_sorted', 'r')
outF = open('data_updated', 'w')
teamUpdateInfo = open('TeamChanges', 'r')

months = {'January': 0, 'February': 1, 'March': 2, 'April': 3, 'May': 4, 'June': 5, 'July': 6, 'August': 7, 'September': 8, 'October': 9, 'November': 10, 'December': 11}
days_of_months = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]

updateInfoDict = {}

#Calculate days since the start of the year
def getDays(day, month):
	day = int(day[:-2])
	month = months[month]

	daysPast = 0
	if month != 'January':
		for i in range(0,month):
			daysPast = daysPast + days_of_months[i]

	return daysPast + day

#Loop through the update information and calculate the days
#Store information needs to be updated in a dictionary - updateInfoDict
for line in teamUpdateInfo:
	words = [word for word in line.split()]
	updateInfoDict[words[1]] = [words[0], getDays(words[2], words[3])]

#Loop through input file to check if information needs to be updated
#Output to the output file
for line in inF:
	words = [word for word in line.split()]
	currentDay = getDays(words[4], words[5])
	
	try:				#Change first team's name if it's eligible
		if currentDay < updateInfoDict[words[0]][1]:
			words[0] = updateInfoDict[words[0]][0]
	except KeyError:
		pass

	try:				#Change second team's name if it's eligible
		if currentDay < updateInfoDict[words[1]][1]:
			words[1] = updateInfoDict[words[1]][0]
	except KeyError:
		pass

	for word in words:
		outF.write(word)
		outF.write(" ")
	outF.write('\n')