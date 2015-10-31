# @brief get data from a line in the data file
# @param data - a string containing teamA - teamB - score - date
# @return teamAName, teamBName, actual score of team A, actual score of team B
def ReadData(data):
	words = [word for word in data.split()]
	teamA = words[0]
	teamB = words[1]

	teamAScore = int(words[2])
	teamBScore = int(words[3])

	actualScoreA = 0
	actualScoreB = 0
	if teamAScore > 16 or teamBScore > 16:	# overtimes are considered ties
		score = 0.5
	elif teamAScore == teamBScore:
		score = 0.5
	elif teamAScore > teamBScore:
		actualScoreA = teamAScore / (teamAScore + teamBScore) + 0.05
		actualScoreB = 1 - actualScoreA
	elif teamBScore > teamAScore:
		actualScoreB = teamBScore / (teamAScore + teamBScore) + 0.05
		actualScoreA = 1 - actualScoreB

	return teamA, teamB, actualScoreA, actualScoreB 