import sys
from bs4 import BeautifulSoup
import requests
import re

sys.setrecursionlimit(1500)

# Get matches, as strings
match_list = []
with open('data_updated', 'r') as inF:
	for line in inF:
		match_list.append(line)
match_list.reverse()

teamInfo = {}
with open('team_rating', 'r') as inF:
	for line in inF:
		words = [word for word in line.split()]
		teamInfo[words[0]] = [float(words[1]), int(words[2])]
		print(teamInfo[words[0]])

page_counter = 6980
pages = 0

team_abb = {'Na\'Vi':'NatusVincere', 'LG':'Luminosity', 'C9':'Cloud9', 'VP':'Virtus.pro', 'E-Frag':'E-frag.net', 'FSid3': 'FlipSid3', 'NME':'Enemy', 'mouz': 'mousesports', 'Fnatic': 'fnatic', 'HR': 'HellRaisers', 'Dignitas':'dignitas', 'coL':'compLexity', 'WinterFox': 'Winterfox'}

outF = open('lounge_odds', 'w')
while pages <= 1000:
	try:
		r = requests.get("http://www.csgolounge.com/match?m=" + str(page_counter))
	except:
		continue

	soup = BeautifulSoup(r.text, 'html.parser')

	try:
		teamA_tag = soup.find(style = "width: 45%; float: left; text-align: right")
		teamB_tag = soup.find(style = "width: 45%; float: left; text-align: left")
		teamA = str(teamA_tag.find('b').string)
		teamB = str(teamB_tag.find('b').string)
		teamA_odd = str(teamA_tag.find('i').string)
		teamB_odd = str(teamB_tag.find('i').string)
	except:
		page_counter = page_counter - 1

	# Fix team names
	winner = 0
	if teamA[-5:] == '(win)': 
		teamA = teamA[:-6]
		winner = 1
	elif teamB[-5:] == '(win)': 
		teamB = teamB[:-6]
		winner = 2
	try:
		teamA = team_abb[teamA]
	except KeyError:
		pass
	try:
		teamB = team_abb[teamB]
	except KeyError:
		pass

	display_tag = soup.find_all(class_ = "half")
	date_tag = display_tag[2]
	date = str(date_tag['title'])					# Format for date: Monday 23rd November 2015

	match_format = str(display_tag[1].string)		# Best of 1,2,3

	try:
		if teamInfo[teamA][1] >= 100 and teamInfo[teamB][1] >= 100:
			if winner == 1: teamA = teamA + '(win)'
			elif winner == 2: teamB = teamB + '(win)'
			outF.write(teamA + " " + teamB + " " + teamA_odd + " " + teamB_odd + " " + match_format + " " + date + '\n')
			pages = pages + 1
	except KeyError:
		pass

	print(teamA + " " + teamB)
	page_counter = page_counter -1

outF.close()