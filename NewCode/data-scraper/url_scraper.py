import sys
import re
import psycopg2
import requests
from bs4 import BeautifulSoup

def scrapeurl():
	# Debug dbInfo
	#dbInfo = "dbname='testdb' user='postgres' host='localhost' password='TnS131997'"
	# RDS dbInfo
	dbInfo = "dbname='csgodata' user='ueisiost' host='csgoinstance.cnlaxgorpmfr.us-west-2.rds.amazonaws.com:5432csgoinstance.cnlaxgorpmfr.us-west-2.rds.amazonaws.com' port='5432' password='TnS131997'"

	try:
	    conn = psycopg2.connect(dbInfo)
	except:
	    sys.exit('Cannot connect to database')
	insertCur = conn.cursor()
	selectCur = conn.cursor('selectcur')
	try:
		selectCur.execute("""select * from url order by id desc limit 100""")
	except Exception, e:
		print(str(e))
		sys.exit()
	lasthundred = selectCur.fetchall()
	selectCur.close()

	r = requests.get('http://www.hltv.org/matches/archive/')
	soup = BeautifulSoup(r.text, 'html.parser')

	urlContainer = soup.find_all(class_='hotmatchbox')[2].table
	urls = urlContainer.find_all(href = re.compile('/match/'))
	urls = urls[::-1]
	old_link = ''
	for url in urls:
		if (url['href'] != old_link):
			old_link = url['href'].encode('ascii','ignore')

			existed = False
			for row in lasthundred:
				if (old_link == row[1]):
					existed = True
					break
			if existed: continue

			try:
				insertCur.execute("""insert into url (url) values (%s)""", [old_link])
				conn.commit()
				print(old_link)
			except Exception, e:
				print(str(e))

	insertCur.close()
	conn.close()

def test():
	print "Testing"

if __name__ == "__main__":
	sys.exit("Not for running from terminal")