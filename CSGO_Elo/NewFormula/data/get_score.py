import requests
import re
import sys
from bs4 import BeautifulSoup

sys.setrecursionlimit(1500)
f = open('temp_data', 'r')
outF = open('data_raw2', 'w')

for line in f:
	url = "http://www.hltv.org" + line
	r = requests.get(url)
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

		outF.write(team1 + " " + team2 + " " + first_score + " " + second_score + " " + date + "\n")


f.close()
outF.close()