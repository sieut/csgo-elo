import elo_system
import get_data
import math

inF = open('data/data_updated', 'r')
outF = open('data/team_rating', 'w')

elo_system.param1 = 28.7
get_data.param2 = 0.0219

m = 0
rightGuess = 0
teamInfo = {}
# Train the rating
for line in inF:
	m = m + 1
	teamA, teamB, actualScoreA, actualScoreB = get_data.ReadData(line)

	try:
		Ra = teamInfo[teamA]
	except KeyError:
		teamInfo[teamA] = 1200
		Ra = 1200
	try:
		Rb = teamInfo[teamB]
	except KeyError:
		teamInfo[teamB] = 1200
		Rb = 1200

	expectedA, expectedB = elo_system.CalculateExpectedScore([Ra, Rb])
	teamInfo[teamA], teamInfo[teamB] = elo_system.UpdateRating(Ra, Rb, actualScoreA, actualScoreB)

	print (teamA + " " + teamB + " " + str(expectedA) + " " + str(expectedB) + " " + str(actualScoreA) + " " + str(actualScoreB))

	if ((expectedA - get_data.param2 - 0.5)*(actualScoreA - 0.5) > 0): rightGuess += 1
	elif (abs(expectedA - 0.5) < get_data.param2 and actualScoreA == 0.5): rightGuess += 1

for team in teamInfo:
	outF.write(team + " " + str(teamInfo[team]) + '\n')