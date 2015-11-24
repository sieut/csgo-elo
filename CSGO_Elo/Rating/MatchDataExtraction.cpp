#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
using namespace std;

class Team {
private:
	string name;
	int numPlay;
	double rating;
public:
	Team();
	Team(const string& nameInput);
	const string& Name() const { return name; }
	bool operator>(const Team& t) const { return name > t.name; }
	bool operator<(const Team& t) const { return name < t.name; }
	bool operator==(const Team& t) const { return name == t.name; }
	friend ostream& operator<<(ostream& os, const Team& t);
};

Team::Team()
{
	name = "--null--";
	rating = 1000;
}

Team::Team(const string& nameInput)
{
	name = nameInput;
	rating = 1000;
}

ostream& operator<<(ostream& os, const Team& t)
{
	os << t.name;
	return os;
}

//We have teamData now
class Match {
private:
	bool tie;		//(0 = good, 1 = tie)
	int winIndex;
	int loseIndex;
	double winActualScore;		//exclude k2 part
public:
	Match();
	Match(bool md, int winidx, int loseidx, double winActSc);
	bool isTie() const { return tie; }
	friend ostream& operator<<(ostream& os, const Match& m);
};

Match::Match()
{
	tie = false;
	winIndex = loseIndex = winActualScore = -1;
}

Match::Match(bool md, int winidx, int loseidx, double winActSc)
{
	tie = md;
	winIndex = winidx;
	loseIndex = loseidx;
	winActualScore = winActSc;
}

ostream& operator<<(ostream& os, const Match& m)
{
	os << static_cast<int>(m.tie)  << " " << m.winIndex << " "
	   << m.loseIndex << " " << m.winActualScore;
    return os;
}

void CreateTeamVector(vector<Team>& teamData);
int FindTeamIndex(const string& name, const vector<Team>& teamData);
void ExtractMatchVector(const string& input, vector<Match>& matchData);
void PrintMatchVectorToFile(ofstream& outfile, const vector<Team>& matchData);

void CreateTeamVector(vector<Team>& teamData)
{
	string inFileName;
	cout << "Teamlist file(with \".txt\"): ";
	cin >> inFileName;

	ifstream infile(inFileName.c_str());
	if (!infile.is_open())
	{
		cerr << "Cannot open file." << endl;
		return;
	}
	string line;
	getline(infile, line);
	while (infile.good())
	{
		teamData.push_back(Team(line));
		getline(infile, line);
	}

	if (!infile.eof())
	{
		cerr << "Cannot read til the end." << endl;
		return;
	}
}

//find teamIndex from teamName
//use binary search
int FindTeamIndex(const string& name, const vector<Team>& teamData)
{
	int first = 0, last = teamData.size() - 1;
	int mid;

	while (first <= last)
	{
		mid = (first + last) / 2;
		if (name == teamData.at(mid).Name())
			return mid;
		else if (name < teamData.at(mid).Name())
			last = mid - 1;
		else
			first = mid + 1;
	}

	cout << "Do not have this team" << endl;
	return -1;
}

void ExtractMatchVector(const string& input, vector<Match>& matchData, const vector<Team>& teamData)
{
	istringstream inSS;
	string teamAName, teamBName;
	int teamAScore, teamBScore;

	inSS.clear();
	inSS.str(input);

	inSS >> teamAName;
	inSS >> teamBName;
	inSS >> teamAScore;
	inSS >> teamBScore;
	inSS.clear();

	int teamAIndex = FindTeamIndex(teamAName, teamData);
	int teamBIndex = FindTeamIndex(teamBName, teamData);
	double winActualScore;
												// ATTENTION:
	if (teamAScore + teamBScore < 16) {		// Sometimes teams drop the match, so the result is 1:0
		// Include a condition in your code that, if
							// both actual scores are 0 then skip to the next match
		return;
	}
	else if (teamAScore > 16 || teamBScore > 16 || teamAScore == teamBScore) {		// In this case, the match went to overtime
		matchData.push_back(Match(true, teamAIndex, teamBIndex, 0.5));
		return;
	} else if (teamAScore > teamBScore) {
		winActualScore = static_cast<double>(teamAScore)/(teamAScore + teamBScore);
		matchData.push_back(Match(false, teamAIndex, teamBIndex, winActualScore));
		return;
	} else if (teamBScore > teamAScore) {
		winActualScore = static_cast<double>(teamBScore)/(teamAScore + teamBScore);
		matchData.push_back(Match(false, teamBIndex, teamAIndex, winActualScore));
		return;
	}
}

void PrintMatchVectorToFile(ofstream& outfile, const vector<Match>& matchData)
{
	int numMatch = matchData.size();
	for (int i = 0; i < numMatch; i++)
	{
		outfile << matchData.at(i) << endl;
	}

	//outfile << "There are " << numMatch << " matches" << endl;
}

int main()
{
	vector<Team> teamData;
	CreateTeamVector(teamData);
	vector<Match> matchData;

	string inFileName;
	cin >> inFileName;

	ifstream infile(inFileName.c_str());
	if (!infile.is_open())
	{
		cerr << "Cannot open file." << endl;
		return 1;
	}
	string line;
	getline(infile, line);
	while (infile.good())
	{
		ExtractMatchVector(line, matchData, teamData);
		getline(infile, line);
	}

	if (!infile.eof())
	{
		cerr << "Cannot read til the end." << endl;
		return 1;
	}

	ofstream outfile("matchIndex.txt");
	PrintMatchVectorToFile(outfile, matchData);

	return 0;
}


