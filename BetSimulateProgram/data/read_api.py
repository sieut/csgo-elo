s = open('lounge_match','r').read()
matches = eval(s)
s = open('lounge_match_stat', 'r').read()
matchesStat = eval(s)

teamInfo = {}
with open('team_rating','r') as inF:
	for line in inF:
		words = [word for word in line.split()]
		teamInfo[words[0]] = [float(words[1]), int(words[2])]

odds = {}
for match in matchesStat: odds[match['match']] = [int(match['a']), int(match['b'])]

team_abb = {'Na\'Vi':'NatusVincere', 'LG':'Luminosity', 'C9':'Cloud9', 
	'VP':'Virtus.pro', 'E-Frag':'E-frag.net', 'FSid3': 'FlipSid3', 
	'NME':'Enemy', 'mouz': 'mousesports', 'Fnatic': 'fnatic', 
	'HR': 'HellRaisers', 'Dignitas':'dignitas', 'coL':'compLexity', 
	'WinterFox': 'Winterfox', 'TSM':'?'}

with open('lounge_odds','w') as outF:
	for match in matches:
		try:
			teamA = team_abb[match['a']]
			teamB = team_abb[match['b']]
		except KeyError: 
			teamA = match['a']
			teamB = match['b']

		try:
			if teamInfo[teamA][1] >= 100 and teamInfo[teamB][1] >= 100 and match['closed'] == '1':
				outF.write(teamA + ' ' + teamB + ' ' + match['winner']
					+ ' ' + match['when'] + ' ' + str(odds[match['match']][0])
					+ ' ' + str(odds[match['match']][1]) + ' ' + match['format'] + '\n')
		except KeyError: pass