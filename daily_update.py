import requests
import re
import sys
from bs4 import BeautifulSoup
import get_data
import elo_system

sys.setrecursionlimit(1500)

# Get last match's link
lastMatchFile = open('data/last_match', 'r')
lastMatchLink = lastMatchFile.read().splitlines()
lastMatchFile.close()

linkList = []
newMatches = []
teamInfo = {}

# Get new matches' links
r = requests.get("http://www.hltv.org/matches/archive/")
soup = BeautifulSoup(r.text, 'html.parser')

hotMatches = soup.find(id = "matches")
oldLink = ""
for tag in hotMatches.find_all(href = re.compile("/match/")):
	link = tag['href']
	if link == lastMatchLink[0]:
		break
	elif link != oldLink:
		linkList.append(link)
		print(link)
		oldLink = link

# Write last match's link into lastMatchFile
lastMatchFile = open('data/last_match', 'w')
lastMatchFile.write(linkList[0])
lastMatchFile.close()

linkList.reverse()

# Get the new scores
for link in linkList:
	r = requests.get("http://www.hltv.org" + link)
	soup = BeautifulSoup(r.text, 'html.parser')

	try:
		teams = soup(attrs = {'class' : 'nolinkstyle'})
		team1 = str(teams[0].string)
		team2 = str(teams[1].string)

		team1 = team1.replace(" ","")
		team2 = team2.replace(" ","")
	except UnicodeEncodeError:
		continue

	date_tag = soup(style = "font-size:14px;")
	date = date_tag[0].string.replace(" of ", " ")

	scores = soup(style = "margin-top: -7px;font-size: 12px;width:270px;border-top:0;")
	for tag in scores:
		first_score_tag = tag.find_next("span")
		second_score_tag = first_score_tag.find_next("span")
		first_score = first_score_tag.string
		second_score = second_score_tag.string

		with open('data/data_updated', 'a') as dataFile:
			dataFile.write(team1 + " " + team2 + " " + first_score + " " + second_score + " " + date + "\n")
		newMatches.append(team1 + " " + team2 + " " + first_score + " " + second_score)

# Read team info
teamFile = open('data/team_rating', 'r')

for line in teamFile:
	words = [word for word in line.split()]
	teamInfo[words[0]] = [float(words[1]), int(words[2])]

teamFile.close()

# Update ratings
for line in newMatches:
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

	teamInfo[teamA][0], teamInfo[teamB][0] = elo_system.UpdateRating(AInfo, BInfo, actualScoreA, actualScoreB)
	teamInfo[teamA][1] += 1
	teamInfo[teamB][1] += 1

# Write new ratings back to file
teamFile = open('data/team_rating', 'w')

for team in teamInfo:
	teamFile.write(team + " " + str(teamInfo[team][0]) + " " + str(teamInfo[team][1]) + '\n')

teamFile.close()