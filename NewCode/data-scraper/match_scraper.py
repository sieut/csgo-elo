import sys
import re
import requests
from bs4 import BeautifulSoup
from dateutil.parser import parse as dateParse

def scrapematch(url):
	r = requests.get('www.htlv.org/match' + url)
	soup = BeautifulSoup(r.text, 'html.parser')
	center = soup.find(class_='centerFade')

	# There will be 4 items in teamNames, they are duplicated
	teamNames = center.find_all(href=re.compile('&teamid='))
	try:
		teamA = teamNames[0].text.encode('ascii', 'ignore')
		teamB = teamNames[1].text.encode('ascii', 'ignore')
		teamA = teamA.replace(' ', '')
		teamB = teamB.replace(' ', '')
	except:
		# Sometimes team names are weird, these are small teams, ignore them
		return

	# This is just ugly
	# Date format is weird, need to parse it
	date = center\
		.find(style='text-align:center;font-size: 18px;display:flex;flex-direction: row;justify-content: center;align-items: center')\
		.find('span').text.encode('ascii', 'ignore')
	date = dateparser(date)

	# Score is also ugly
	# This page design is shit
	scores = center.find_all(style='margin-top: -7px;font-size: 12px;width:270px;border-top:0;')
	for score in scores:
		teamATag = score.find_next('span')
		teamBTag = teamATag.find_next('span')
		teamAScore = int(teamATag.text.encode('ascii', 'ignore'))
		teamBScore = int(teamBTag.text.encode('ascii', 'ignore'))
		# Then insert into table

def dateparser(date):
	words = date.split()

	originDate = dateParse('2016-01-01')
	monthKeys = {'January': '01', 'February': '02', 'March': '03', 'April': '04', 'May': '05', \
		'June': '06', 'July': '07', 'August': '08', 'September': '09', 'October': '10', \
		'November': '11', 'December': '12'}

	day = words[0][:-2]
	month = words[2]
	year = words[3]

	if day.length==1: day = '0' + day
	month = monthKeys[month]
	date = year + '-' + month + '-' + day
	date = dateParse(date)

	delta = date - originDate
	return delta.days