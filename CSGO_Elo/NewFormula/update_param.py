learningRate = 0.1

# @brief update 2 parameters in calculating factor k of the Elo formula
# @param package - param1, d(costfunc)/dparam1 * 1/m
# @return param1, param2
def UpdateParam(package):
	param1 = package[0] - learningRate * package[1]

	return param1