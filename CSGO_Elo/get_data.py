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
		actualScoreA = 0.5
		actualScoreB = 0.5
	elif teamAScore == teamBScore:
		actualScoreB = 0.5
		actualScoreA = 0.5
	elif teamAScore > teamBScore:
		actualScoreA = float(teamAScore) / float(teamAScore + teamBScore) + 0.05
		actualScoreB = 1 - actualScoreA
	elif teamBScore > teamAScore:
		actualScoreB = float(teamBScore) / float(teamAScore + teamBScore) + 0.05
		actualScoreA = 1 - actualScoreB

	return teamA, teamB, actualScoreA, actualScoreB 