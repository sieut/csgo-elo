import sys
import re
import psycopg2
import requests
from bs4 import BeautifulSoup

sys.setrecursionlimit(1500)

try:
    conn = psycopg2.connect("dbname='testdb' user='postgres' host='localhost' password='TnS131997'")
    conn.autocommit = True
except:
    sys.exit('Cannot connect to database')
cur = conn.cursor()

r = requests.get('http://www.hltv.org/matches/archive/')
soup = BeautifulSoup(r.text, 'html.parser')

urlContainer = soup.find_all(class_='hotmatchbox')[2].table
urls = urlContainer.find_all(href = re.compile('/match/'))
old_link = ''
for url in urls:
	if (url['href'] != old_link):
		old_link = url['href'].encode('ascii','ignore')
		try:
			cur.execute("""insert into url (url) values (%s)""", [old_link])
			conn.commit()
		except Exception, e:
			print(str(e))

cur.close()
conn.close()