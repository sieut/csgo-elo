import elo_system
import get_data
import update_param
import math
import random

elo_system.param1 = 73.61
get_data.param2 = 0.066
outF = open('train_record11.csv', 'w')

while True:
	inF = open('data/data_updated', 'r')
	teamRating = {}

	rightGuess = 0
	m = 0				# Number of test samples

	# Rating
	for line in inF:
		m = m + 1
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
		teamRating[teamA], teamRating[teamB] = elo_system.UpdateRating(Ra, Rb, actualScoreA, actualScoreB)

		if ((expectedA - get_data.param2 - 0.5)*(actualScoreA - 0.5) > 0): rightGuess += 1
		elif (abs(expectedA - 0.5) < 0.04 and actualScoreA == 0.5): rightGuess += 1

	outF.write(str(elo_system.param1) + "," + str(get_data.param2) + "," + str(float(rightGuess)/float(m)) + '\n')
	
	print(float(rightGuess)/float(m))
	inF.close()

	if abs(get_data.param2 - 0.09) < 0.00001:
		get_data.param2 = 0.01
		elo_system.param1 += 0.01
	elif elo_system.param1 > 76:
		break
	else:
		get_data.param2 += 0.0001

outF.close()