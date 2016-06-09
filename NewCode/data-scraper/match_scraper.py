import sys
import re
import requests
import psycopg2
from bs4 import BeautifulSoup
from dateutil.parser import parse as dateParse

def scrapematch():
	# Debug dbInfo
	dbInfo = "dbname='testdb' user='postgres' host='localhost' password='TnS131997'"
	# RDS dbInfo
	#dbInfo = "dbname='csgodata' user='ueisiost' host='csgoinstance.cnlaxgorpmfr.us-west-2.rds.amazonaws.com' port='5432' password='TnS131997'"

	start = 0
	limit = 10

	try:
	    conn = psycopg2.connect(dbInfo)
	except Exception, e:
	    sys.exit(str(e))

	lastMatchCur = conn.cursor('lastmatch')
	try:
		lastMatchCur.execute("""select urlid from match order by id desc limit 1""")
		lastMatch = lastMatchCur.fetchall()
		# Should be the id of first URL, but it's 1 on AWS
		if len(lastMatch) == 0: start = 0
		else: start = lastMatch[0][0]
		lastMatchCur.close()
	except Exception, e:
		sys.exit(str(e))

	urlCur = conn.cursor('url')
	try:
		urlCur.execute("""select * from url where id > %s limit %s""", [start, limit])
		urls = urlCur.fetchall()
		urlCur.close()
	except Exception, e:
		sys.exit(str(e))

	insertCur = conn.cursor()
	for url in urls:
		results = getdata(url[1])
		if (results == None): continue
		else:
			try:
				for result in results:
					insertCur.execute("""insert into match (teama, teamb, scorea, scoreb, days, urlid)
						values (%s, %s, %s, %s, %s, %s)""", [result[0], result[1], result[2], result[3], result[4], url[0]])
					conn.commit()
			except Exception, e: print(str(e) + ' at id: ' + str(url[0]))
	insertCur.close()
	conn.close()


def getdata(url):
	result = []

	r = requests.get('http://www.hltv.org' + url)
	soup = BeautifulSoup(r.text, 'html.parser')
	center = soup.find(class_='centerFade')

	# There will be 4 items in teamNames, they are duplicated
	teamNames = center.find_all(href=re.compile('&teamid='))
	try:
		teamA = teamNames[0].text.encode('ascii', 'ignore')
		teamB = teamNames[1].text.encode('ascii', 'ignore')
		teamA = teamA.replace(' ', '')
		teamB = teamB.replace(' ', '')
	except Exception, e:
		# Sometimes team names are weird, these are small teams, ignore them
		return None

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
		result.append([teamA, teamB, teamAScore, teamBScore, date])

	return result

def dateparser(date):
	words = date.split()

	originDate = dateParse('2016-01-01')
	monthKeys = {'January': '01', 'February': '02', 'March': '03', 'April': '04', 'May': '05', \
		'June': '06', 'July': '07', 'August': '08', 'September': '09', 'October': '10', \
		'November': '11', 'December': '12'}

	day = words[0][:-2]
	month = words[2]
	year = words[3]

	if len(day)==1: day = '0' + day
	month = monthKeys[month]
	date = year + '-' + month + '-' + day
	date = dateParse(date)

	delta = date - originDate
	return delta.days

if __name__ == "__main__":
	scrapematch()