class Team(object):
	def __init__(self, name):
		super(Neighbor, self).__init__()
		self.name = name
		self.rating = 1200
		self.match = 0
		self.neighbors = []

	def updateRating(self, delta=0):
		self.rating += delta
	def incrementMatch(self):
		self.match++


	# Getters
	def getName(self):
		return self.name
	def getRating(self):
		return self.rating
	def getMatch(self):
		return self.match
	def getNeighbor(self):
		return self.neighbors