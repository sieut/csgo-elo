import elo_system
import get_data
import update_param
import math
import random

outF = open('data/train_result', 'w')

elo_system.param1 = 31.1560947555

while True:
	trainF = open('data/data_train', 'r')
	testF = open('data/data_test', 'r')
	teamRating = {}
	# Train the rating
	for line in trainF:
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

		teamRating[teamA], teamRating[teamB] = elo_system.UpdateRating([Ra, Rb, actualScoreA])

	costSum = 0
	param1Sum = 0
	m = 0				# Number of test samples

	# Test the rating
	for line in testF:
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
		teamRating[teamA], teamRating[teamB] = elo_system.UpdateRating([Ra, Rb, actualScoreA])

		costSum = costSum + (expectedA - actualScoreA)**2
		param1Sum = param1Sum - ( (Ra - teamRating[teamA])**2 / elo_system.param1**3 )
		
	costSum = costSum/(2*m)
	param1Sum = param1Sum/m

	outF.write(str(elo_system.param1) +  " " + str(costSum) + '\n')
	print(str(elo_system.param1) +  " " + str(costSum))

	elo_system.param1 = update_param.UpdateParam([elo_system.param1, param1Sum])
	trainF.close()
	testF.close()