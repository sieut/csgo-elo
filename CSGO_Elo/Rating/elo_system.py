# Ra = rating of team a ; Rb = rating of team b
# expectedA = expected score of team a vs team b ; expectedB = expected score of team b vs team a
# expectedScore = 1 / ( 1 + 10^((otherTeamRating - thisTeamRating) / 400) )
# actualScore = result of the match: 0: lost, 0.5: tied, 1: won
#
# Rating of each team is updated after every match
# newRating = oldRating - k(actualScore - expectedScore)
# param1 and param2 are the two parameters need to be determined by machine learning
#
# This file only calculates expectedScore, updates ratings with given parameters
# Info of teams will be on the main running file


# Initialize 2 parameters for the function of k
# This will be changed in the main running file
param1 = 37.8

# @brief calculate expected score of each team
# @param package - [rating of team A, rating of team B]
# @return TeamAScore, TeamBScore
def CalculateExpectedScore(package):
	Ra = package[0]
	Rb = package[1]

	expectedA = 1 / ( 1 + 10**((Rb - Ra) / 400) )
	expectedB = 1 - expectedA

	return expectedA, expectedB

# @brief update rating of each team given the result of the match
# @param package - [rating of team A, rating of team B, actual score of team A]
# @returen UpdatedTeamARating, UpdatedTeamBRating
def UpdateRating(AInfo, BInfo, actualScoreA, actualScoreB):
	ratingPackage = [AInfo[0], BInfo[0]]
	expectedA, expectedB = CalculateExpectedScore(ratingPackage)

	wtl = 0
	if actualScoreA > 0.5: wtl = 1
	elif actualScoreA == 0.5: wtl = 0.5

	rateA = param1
	rateB = param1
	if AInfo[1] <= 40:
		rateA *= 2
	elif AInfo[1] <= 100:
		pass
	elif AInfo[1] <= 150:
		rateA /= 2
	else: rateA /= 4
 	if BInfo[1] <= 40:
		rateB *= 2
	elif BInfo[1] <= 100:
		pass
	elif BInfo[1] <= 150:
		rateB /= 2
	else: rateB /= 4
 
	newRa = AInfo[0] + rateA * (wtl + actualScoreA - 2*expectedA)
	newRb = BInfo[0] + rateB * (1-wtl + actualScoreB - 2*expectedB)

	return newRa, newRb