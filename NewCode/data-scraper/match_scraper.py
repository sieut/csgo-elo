import sys
import re
import requests
from bs4 import BeautifulSoup

def scrapematch(url):
	r = requests.get('www.htlv.org/match' + url)
	soup = BeautifulSoup(r.text, 'html.parser')
	center = soup.find(class_='centerFade')

	# There will be 4 items in teamNames, they are duplicated
	teamNames = center.find_all(href=re.compile('&teamid='))
	try:
		teamA = teamNames[0].text.encode('ascii', 'ignore')
		teamB = teamNames[1].text.encode('ascii', 'ignore')
	except:
		# Sometimes team names are weird, these are small teams, ignore them
		return

	# This is just ugly
	# Date format is weird, need to parse it
	date = center\
		.find(style='text-align:center;font-size: 18px;display:flex;flex-direction: row;justify-content: center;align-items: center')\
		.find('span').text