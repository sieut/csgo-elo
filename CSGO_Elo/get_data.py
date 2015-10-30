# @brief get data from a line in the data file
# @param data - a string containing teamA - teamB - score - date
# @return teamAName, teamBName, score for team A, absolute value of round difference
def ReadData(data):
	words = [word for word in data]
	teamA = words[0]
	teamB = words[1]

	teamAScore = int(words[2])
	teamBScore = int(words[3])

	if teamAScore > 16 or teamBScore > 16:	# overtimes are considered ties
		score = 0.5
	elif teamAScore > teamBScore:
		score = 1
	elif teamBScore > teamAScore:
		score = 0
	elif teamAScore == teamBScore:
		score = 0.5

	return teamA, teamB, score, abs(teamAScore - teamBScore)