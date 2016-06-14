# Python msut be run by sudo

from TorCtl import TorCtl
import urllib2
import random

USER_AGENTS = ['Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2226.0 Safari/537.36',
	'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_9_3) AppleWebKit/537.75.14 (KHTML, like Gecko) Version/7.0.3 Safari/7046A194A',
	'Mozilla/5.0 (Windows NT 6.1; WOW64; rv:40.0) Gecko/20100101 Firefox/40.1']

def getHtml(urls):
	results = []
	renew_connection()
	print request("http://icanhazip.com/")

	for url in urls:
		results.append(request(url))

	return results

def request(url):
	headers = {'User-Agent': USER_AGENTS[0],
		'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
		'Accept-Charset': 'ISO-8859-1,utf-8;q=0.7,*;q=0.3',
		'Accept-Encoding': 'none',
		'Accept-Language': 'en-US,en;q=0.8',
		'Connection': 'keep-alive'}
	def _set_urlproxy():
		proxy_support = urllib2.ProxyHandler({"http" : "127.0.0.1:8118"})
		opener = urllib2.build_opener(proxy_support)
		urllib2.install_opener(opener)
	_set_urlproxy()
	request=urllib2.Request(url, None, headers)
	return urllib2.urlopen(request).read()

def renew_connection():
    conn = TorCtl.connect(controlAddr="127.0.0.1", controlPort=9051, passphrase="TnS131997")
    conn.send_signal("NEWNYM")
    conn.close()