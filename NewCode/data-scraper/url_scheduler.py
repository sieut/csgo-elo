import url_scraper
import sys
import time
import logging
from apscheduler.schedulers.background import BackgroundScheduler

def main():
	logging.basicConfig()
	sys.setrecursionlimit(1500)

	scheduler = BackgroundScheduler()
	scheduler.add_job(url_scraper.scrapeurl, 'interval', hours=8)
	scheduler.start()
	while True:
		time.sleep(10)
	scheduler.shutdown()

if __name__ == "__main__":
	main()