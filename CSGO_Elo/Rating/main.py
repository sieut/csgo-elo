import elo_system
import get_data

rosterChanges = []
inF = open('data/roster_change', 'r')
for line in inF: rosterChanges.append(line)
inF.close()

param1Start = float(input("Start of k1: "))
param1End = float(input("End of k1: "))
param2Start = float(input("Start of k2: "))
param2End = float(input("End of k2: "))

param1Step = (param1End - param1Start)/35
param2Step = (param2End - param2Start)/35

elo_system.param1 = param1Start
get_data.param2 = param2Start
outF = open('train_record.csv', 'w')

while True:
	inF = open('data/data_updated', 'r')

	# Variable teamInfo
	#	- Keys - team's name
	#	- Values - [rating, matches]
	teamInfo = {}
	rosterChangesIdx = 0

	loss = 0
	totalMatch = 0

	# Rating process
	for line in inF:
		teamA, teamB, actualScoreA, actualScoreB = get_data.ReadData(line)

		# Get information of team A
		try:
			AInfo = teamInfo[teamA]
			Ra = AInfo[0]
			AMatches = AInfo[1]
		except KeyError:
			Ra = float(1200)
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

		while True:
			rosterChangesWords = [word for word in rosterChanges[rosterChangesIdx].split()]
			matchWords = [word for word in line.split()]
			if rosterChangesWords[1] == matchWords[4] and rosterChangesWords[2] == matchWords[5] and rosterChangesWords[3] == matchWords[6]:
				teamInfo[rosterChangesWords[0]] = elo_system.UpdateMatchesPlayed(teamInfo[rosterChangesWords[0]])
				rosterChangesIdx += 1
			else: break

		expectedA, expectedB = elo_system.CalculateExpectedScore([Ra, Rb])
		teamInfo[teamA][0], teamInfo[teamB][0] = elo_system.UpdateRating(AInfo, BInfo, actualScoreA, actualScoreB)
		teamInfo[teamA][1] += 1
		teamInfo[teamB][1] += 1

		wtl = 0.5
		if actualScoreA > 0.5: wtl = 1
		else: wtl = 0
		loss += (expectedA - wtl)*(expectedA - wtl)
		totalMatch += 1

	outF.write(str(elo_system.param1) + "," + str(get_data.param2) + "," + str(float(loss)/float(totalMatch)) + '\n')
	inF.close()

	if get_data.param2 > param2End:
		get_data.param2 = param2Start
		elo_system.param1 += param1Step
	elif elo_system.param1 > param1End:
		break
	else:
		get_data.param2 += param2Step
