f = open('data', 'r')
fOut = open('data_sorted', 'w')

currentMonth = 'January'
stringList = []
for line in f:
	words = [word for word in line.split()]
	month = words[5]

	if month == currentMonth:
		stringList.append(line)
	else:
		currentMonth = month
		stringList.reverse()
		for dataLine in stringList:
			fOut.write(dataLine)
		stringList = []