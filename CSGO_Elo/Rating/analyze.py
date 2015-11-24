import elo_system
import get_data

elo_system.param1 = 28.7
get_data.param2 = 0.0219

teamInfo = {}
with open('data/train_data', 'r') as dataFile:
	for line in dataFile:
		teamA, teamB, actualScoreA, actualScoreB = get_data.ReadData(line)

		# Get information of team A
		try:
			AInfo = teamInfo[teamA]
			Ra = AInfo[0]
			AMatches = AInfo[1]
		except KeyError:
			Ra = 1200
			AMatches = 0
			AInfo = [Ra, AMatches]
			teamInfo[teamA] = [Ra, AMatches]

		# Get information of team B
		try:
			BInfo = teamInfo[teamB]
			Rb = BInfo[0]
			BMatches = BInfo[1]
		except KeyError:
			Rb = 1200
			BMatches = 0
			BInfo = [Rb, BMatches]
			teamInfo[teamB] = [Rb, BMatches]

		teamInfo[teamA][0], teamInfo[teamB][0] = elo_system.UpdateRating(AInfo, BInfo, actualScoreA, actualScoreB)
		teamInfo[teamA][1] += 1
		teamInfo[teamB][1] += 1

with open('data/test_data', 'r') as dataFile:
	outF = open('data/analyze_data', 'w')

	for line in dataFile:
		teamA, teamB, actualScoreA, actualScoreB = get_data.ReadData(line)

		# Get information of team A
		try:
			AInfo = teamInfo[teamA]
			Ra = AInfo[0]
			AMatches = AInfo[1]
		except KeyError:
			Ra = 1200
			AMatches = 0
			AInfo = [Ra, AMatches]
			teamInfo[teamA] = [Ra, AMatches]

		# Get information of team B
		try:
			BInfo = teamInfo[teamB]
			Rb = BInfo[0]
			BMatches = BInfo[1]
		except KeyError:
			Rb = 1200
			BMatches = 0
			BInfo = [Rb, BMatches]
			teamInfo[teamB] = [Rb, BMatches]

		expectedA, expectedB = elo_system.CalculateExpectedScore([Ra, Rb])
		if actualScoreA >= actualScoreB: outF.write(str(expectedA) + '\n')
		else: outF.write(str(expectedB) + '\n')

		teamInfo[teamA][0], teamInfo[teamB][0] = elo_system.UpdateRating(AInfo, BInfo, actualScoreA, actualScoreB)
		teamInfo[teamA][1] += 1
		teamInfo[teamB][1] += 1

	outF.close()