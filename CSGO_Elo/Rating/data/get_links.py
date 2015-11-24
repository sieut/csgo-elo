from selenium import webdriver
from selenium.webdriver.common.action_chains import ActionChains
from bs4 import BeautifulSoup
import re
import sys

sys.setrecursionlimit(1500)

fOut = open('match_links', 'w')

driver = webdriver.Firefox()
driver.get("http://www.hltv.org/matches/archive/")

month_days = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]

for i in range(1,12):
	date_range = "2015-" + str(i) + "-16 to 2015-" + str(i) + "-" + str(month_days[i-1])
	driver.execute_script("document.getElementById('calendar').value = '" + date_range + "'")
	button = driver.find_element_by_id("search")
	button.click()
	button.click()
	button.click()
	button.click()
	action_chains = ActionChains(driver)
	action_chains.double_click(button).perform()
	action_chains.double_click(button).perform()
	action_chains.double_click(button).perform()
	html_source = driver.page_source
	soup = BeautifulSoup(html_source, 'html.parser')
	hot_matches = soup.find(id = "matches")
	old_link = ""
	for tag in hot_matches.find_all(href = re.compile("/match/")):
		link = tag['href']
		if link != old_link:
			fOut.write(link + '\n')
			old_link = link

	date_range = "2015-" + str(i) + "-01 to 2015-" + str(i) + "-15"
	driver.execute_script("document.getElementById('calendar').value = '" + date_range + "'")
	button = driver.find_element_by_id("search")
	button.click()
	button.click()
	button.click()
	button.click()
	action_chains = ActionChains(driver)
	action_chains.double_click(button).perform()
	action_chains.double_click(button).perform()
	action_chains.double_click(button).perform()
	html_source = driver.page_source
	soup = BeautifulSoup(html_source, 'html.parser')
	hot_matches = soup.find(id = "matches")
	old_link = ""
	for tag in hot_matches.find_all(href = re.compile("/match/")):
		link = tag['href']
		if link != old_link:
			fOut.write(link + '\n')
			old_link = link

fOut.close()
driver.close()