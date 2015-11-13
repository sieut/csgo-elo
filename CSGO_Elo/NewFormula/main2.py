import elo_system
import get_data

elo_system.param1 = 60.02
get_data.param2 = 0.02
outF = open('train_record_2_6.csv', 'w')

while True:
	inF = open('data/data_updated', 'r')

	# Variable teamInfo
	#	- Keys - team's name
	#	- Values - [rating, matches]
	teamInfo = {}

	rightGuess = 0
	testSample = 0

	# Rating process
	for line in inF:
		testSample += 1
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
		teamInfo[teamA][0], teamInfo[teamB][0] = elo_system.UpdateRating(AInfo, BInfo, actualScoreA, actualScoreB)
		teamInfo[teamA][1] += 1
		teamInfo[teamB][1] += 1

		if ((expectedA - get_data.param2 - 0.5)*(actualScoreA - 0.5) > 0): rightGuess += 1
		elif (abs(expectedA - 0.5) < 0.04 and actualScoreA == 0.5): rightGuess += 1

	outF.write(str(elo_system.param1) + "," + str(get_data.param2) + "," + str(float(rightGuess)/float(testSample)) + '\n')
	
	print(float(rightGuess)/float(testSample))
	inF.close()

	if abs(get_data.param2 - 0.09) < 0.00001:
		get_data.param2 = 0.02
		elo_system.param1 += 0.02
	elif elo_system.param1 > 70:
		break
	else:
		get_data.param2 += 0.0001
