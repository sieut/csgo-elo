// InterpretData - for each line read from the data file, call this function to interpret the data in the line
//	@param - data - a line read from the data file
// 	@param - k2 - the second constant that we're finding
//	@return - teamA's name in teamA, teamB's name in teamB, actual scores of 2 teams
void InterpretData(string data, string& teamA, string& teamB, double& actualScoreA, double& actualScoreB, const double& k2) {
	istringstream inSS;
	int teamAScore, teamBScore;

	// Initiate string stream
	inSS.clear();
	inSS.str(data);

	// Get teams' names
	inSS >> teamA;
	inSS >> teamB;

	// Get result
	inSS >> teamAScore;
	inSS >> teamBScore;
											// ATTENTION:
	if (teamAScore + teamBScore < 16) {		// Sometimes teams drop the match, so the result is 1:0
		actualScoreA = 0;					// Include a condition in your code that, if
		actualScoreB = 0;					// both actual scores are 0 then skip to the next match
		return;
	} else if (teamAScore > 16 || teamBScore > 16 || teamAScore == teamBScore) {		// In this case, the match went to overtime
		actualScoreA = 0.5;																// It is considered a tie
		actualScoreB = 0.5;
		return;
	} else if (teamAScore > teamBScore) {
		actualScoreA = static_cast<double>(teamAScore)/static_cast<double>(teamAScore + teamBScore) + k2;
		actualScoreB = 1 - actualScoreA;
		return;
	} else if (teamBScore > teamAScore) {
		actualScoreB = static_cast<double>(teamBScore)/static_cast<double>(teamAScore + teamBScore) + k2;
		actualScoreA = 1 - actualScoreB;
		return;
	}
}

// ExpectedScore - return to expectedA and expectedB probability of winning the match for team A and team B
//	@param - Ra - rating of team A
// 	@param - Rb - rating of team B
//	@return - expectedA and expectedB
void ExpectedScore(double& Ra, double& Rb, double& expectedA, double& expectedB) {
	expectedA = 1 / ( 1 + pow(10, (Rb - Ra) / 400) );
	expectedB = 1 - expectedA;
}

// UpdateRating - update ratings of team A and B after a match
//	@param - Ra - rating of team A
// 	@param - Rb - rating of team B
//	@param - actualScoreA - actual score of team A, interpreted by function InterpretData
//	@param - k1 - the first constant that we're looking for
//	@return - new Ra and Rb
void UpdateRating(double& Ra, double& Rb, double& actualScoreA, double& k1) {
	double expectedA, expectedB, wtl;

	// Get expected scores
	ExpectedScore(Ra, Rb, expectedA, expectedB);

	// Determine win/tie/lose from actualScoreA
	if (actualScoreA > 0.5) wtl = 1;
	else if (actualScoreA < 0.5) wtl = 0;
	else wtl = 0.5;

	// Update rating
	Ra = Ra + k1*(wtl + actualScoreA - 2*expectedA);
	Rb = Rb + k1*(1 - wtl + 1 - actualScoreA - 2*expectedB);
}