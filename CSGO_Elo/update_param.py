learningRate = 0.7

# @brief update 2 parameters in calculating factor k of the Elo formula
# @param package - param1, param2, d(costfunc)/dparam1 * 1/m, d(costfunc)/dparam2 * 1 / m
# @return param1, param2
def UpdateParam(package):
	param1 = package[0] - learningRate * package[2]
	param2 = package[1] - learningRate * package[3]

	return param1, param2