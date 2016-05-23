oddsList = []
with open('lounge_odds', 'r') as inF:
	for line in inF: oddsList.append([word for word in line.split()])

months = {'1':'January', '2':'February', '3':'March', '4':'April', 
	'5':'May', '6':'June', '7':'July', '8':'August', '9':'September', 
	'10':'October', '11':'November', '12':'December'}
postFix = {'1':'st','21':'st','31':'st',
	'2':'nd','22':'nd',
	'3':'rd','23':'rd'}

with open('lounge_odds_fixed', 'w') as outF:	
	for match in oddsList:
		date = [int(word) for word in match[3].split('-')]
		date[1] = months[str(date[1])]
		try: date[2] = str(date[2]) + postFix[str(date[2])]
		except KeyError: date[2] = str(date[2]) + 'th'

		outF.write(match[0] + ' ' + match[1] + ' ' + match[2] + ' ' 
			+ match[5] + ' ' + match[6] + ' ' + match[7] + ' '
			+ date[2] + ' ' + date[1] + ' ' + str(date[0]) + '\n')