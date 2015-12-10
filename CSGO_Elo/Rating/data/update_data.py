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

matches = []
for line in inF: matches.append(line)

#Loop through the update information and calculate the days
#Store information needs to be updated in a dictionary - updateInfoDict
for line in teamUpdateInfo:
	words = [word for word in line.split()]
	dayLimit = getDays(words[2], words[3])

	for match in matches:
		mWords = [word for word in match.split()]
		currentDay = getDays(mWords[4], mWords[5])
		if currentDay > dayLimit: break

		if mWords[0] == words[1]: mWords[0] = words[0]
		elif mWords[1] == words[1]: mWords[1] = words[0]

		for word in mWords: outF.write(word + ' ')
		outF.write('\n')

inF.close()
outF.close()
teamUpdateInfo.close()