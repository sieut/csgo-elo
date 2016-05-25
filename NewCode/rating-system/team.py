from neighbor.py import Neighbor

class Team(object):
	"""docstring for Team"""

	def __init__(self, name):
		super(Neighbor, self).__init__()
		self.name      = name
		self.rating    = 1200
		self.match     = 0
		self.neighbors = []

	def updateRating(self, delta=0):
		self.rating += delta
	def incrementMatch(self):
		self.match++

	def addNeighbor(self, neighborName):
		newNeighbor = Neighbor(neighborName)
		self.neighbors.append(newNeighbor)
	def averageNeighbor(self, teamList):
		average = 0.0
		for neighbor in self.neighbors:
			average += teamList[neighbor.getName()].getRating() * (neighbor.getMatch() / self.match)
		return average


	# Getters
	def getName(self):
		return self.name
	def getRating(self):
		return self.rating
	def getMatch(self):
		return self.match
	def getNeighbor(self):
		return self.neighbors