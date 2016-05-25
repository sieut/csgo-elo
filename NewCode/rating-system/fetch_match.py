from match.py import Match

def getMatchData(fileName):
	with open(fileName, 'r') as file:
		matchData = []

		for line in file:
			words = line.split()

			teamA = words[0]
			teamB = words[1]
			scoreA = words[2]
			scoreB = words[3]
			time = words[7]

			matchData.append(Match(teamA, teamB, scoreA, scoreB, time))

		return matchData