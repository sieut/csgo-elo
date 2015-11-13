import elo_system
import get_data
import math

inF = open('data/data_updated', 'r')
outF = open('data/team_rating', 'w')

elo_system.param1 = 28.7
get_data.param2 = 0.0219

teamInfo = {}

for line in inF:
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

morethan200 = 0
for team in teamInfo:
	if teamInfo[team][1] >= 100:
		morethan200 += 1
		print(team)
	outF.write(team + " " + str(teamInfo[team][0]) + " " + str(teamInfo[team][1]) + '\n')

print (morethan200)