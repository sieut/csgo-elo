import requests
import re
import sys
from bs4 import BeautifulSoup

sys.setrecursionlimit(1500)
f = open('match_links', 'w')
months = ['january', 'february', 'march', 'april', 'may', 'june', 'july', 'august', 'september', 'october', 'november', 'december']
years = ['2015']

for year in years:
	for month in months:
		url = "http://www.hltv.org/matches/archive/" + year +"/" + month + "/"
		r = requests.get(url)
		soup = BeautifulSoup(r.text, 'html.parser')

		table = soup.find(id = "matches")

		old_link = ''
		for tag in table.find_all(href = re.compile("/match/")):
			link = tag['href']

			if link == old_link:
				continue
			else:
				f.write(link)
				f.write('\n')
				old_link = link

f.close()