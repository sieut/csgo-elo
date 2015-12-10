f = open('data_raw2', 'r')
fOut = open('data_sorted2', 'w')

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

stringList.reverse()
for dataLine in stringList:
	fOut.write(dataLine)

f.close()
fOut.close()