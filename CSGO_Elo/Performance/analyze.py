import elo_system
import get_data

elo_system.param1 = 28.7
get_data.param2 = 0.0219

teamRating = {}
with open('data/train_data', 'r') as dataFile:
	for line in dataFile:
		teamA, teamB, actualScoreA, actualScoreB = get_data.ReadData(line)

		try:
			Ra = teamRating[teamA]
		except KeyError:
			teamRating[teamA] = 1200
			Ra = 1200
		try:
			Rb = teamRating[teamB]
		except KeyError:
			teamRating[teamB] = 1200
			Rb = 1200

		teamRating[teamA], teamRating[teamB] = elo_system.UpdateRating(Ra, Rb, actualScoreA, actualScoreB)

with open('data/test_data', 'r') as dataFile:
	outF = open('data/analyze_data', 'w')

	for line in dataFile:
		teamA, teamB, actualScoreA, actualScoreB = get_data.ReadData(line)

		try:
			Ra = teamRating[teamA]
		except KeyError:
			teamRating[teamA] = 1200
			Ra = 1200
		try:
			Rb = teamRating[teamB]
		except KeyError:
			teamRating[teamB] = 1200
			Rb = 1200

		expectedA, expectedB = elo_system.CalculateExpectedScore([Ra, Rb])
		if actualScoreA >= actualScoreB: outF.write(str(expectedA) + '\n')
		else: outF.write(str(expectedB) + '\n')

		teamRating[teamA], teamRating[teamB] = elo_system.UpdateRating(Ra, Rb, actualScoreA, actualScoreB)

	outF.close()