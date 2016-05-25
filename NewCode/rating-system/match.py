class Match(object):
	"""docstring for Match"""

	def __init__(self, teamA, teamB, scoreA, scoreB, time):
		super(Match, self).__init__()
		self.teamA  = teamA			# This probably will be team names
		self.teamB  = teamB			# This probably will be team names
		self.scoreA = int(scoreA)
		self.scoreB = int(scoreB)
		self.time   = int(time)

	# Getters
	def getTeamA(self):
		return self.teamA
	def getTeamB(self):
		return self.teamB
	def getScoreA(self):
		return self.scoreA
	def getScoreB(self):
		return self.scoreB
	def getTime(self):
		return self.time