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
param1 = None

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
def UpdateRating(package):
	ratingPackage = [package[0], package[1]]
	expectedA, expectedB = CalculateExpectedScore(ratingPackage)

	newRa = package[0] + param1*(package[2] - expectedA)
	newRb = package[1] + param1*(1 - package[2] - expectedB)

	return newRa, newRb