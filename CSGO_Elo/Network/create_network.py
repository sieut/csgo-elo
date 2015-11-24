from collections import defaultdict

graph = defaultdict(dict)

with open('data/data_updated', 'r') as dataFile:
	for line in dataFile:
		words = [word for word in line.split()]

		try:
			graph[words[0]][words[1]] += 1
			graph[words[1]][words[0]] += 1
		except KeyError:
			graph[words[0]][words[1]] = 1
			graph[words[1]][words[0]] = 1

with open('data/graph.csv', 'w') as graphFile:
	for team in graph:
		graphFile.write(',' + team)
	graphFile.write('\n')

	for team in graph:
		graphFile.write(team)
		for team2 in graph[team]:
			graphFile.write(',' + str(graph[team][team2]))
		graphFile.write('\n')