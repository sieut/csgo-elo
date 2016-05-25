class Neighbor(object):
	"""docstring for Neighbor"""

	def __init__(self, name):
		super(Neighbor, self).__init__()
		self.name  = name
		self.match = 0

	def incrementMatch(self):
		self.match++

	# Getters
	def getName(self):
		return self.name
	def getMatch(self):
		return self.match