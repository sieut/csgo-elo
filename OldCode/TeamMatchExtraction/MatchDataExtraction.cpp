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
	int day;
	int week;
public:
	Match();
	Match(bool md, int winidx, int loseidx, double winActSc, int _day, int wk);
	bool isTie() const { return tie; }
	friend ostream& operator<<(ostream& os, const Match& m);
};

Match::Match()
{
	tie = false;
	winIndex = loseIndex = winActualScore = week = -1;

}

Match::Match(bool md, int winidx, int loseidx, double winActSc, int _day, int wk)
{
	tie = md;
	winIndex = winidx;
	loseIndex = loseidx;
	winActualScore = winActSc;
	day = _day;
	week = wk;
}

ostream& operator<<(ostream& os, const Match& m)
{
	os << static_cast<int>(m.tie)  << " " << m.winIndex << " "
	   << m.loseIndex << " " << m.winActualScore << " " << m.day << " " << m.week;
    return os;
}

void CreateTeamVector(vector<Team>& teamData);
int FindTeamIndex(const string& name, const vector<Team>& teamData);
void ExtractMatchVector(const string& input, vector<Match>& matchData, const vector<Team>& teamData);
void PrintMatchVectorToFile(ofstream& outfile, const vector<Match>& matchData);
int DateToWeek(const string &day, const string &month, const string &year);
int DateToDay(const string &day, const string &month, const string &year);

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

	ofstream outfile("matchIndex_updated.txt");
	PrintMatchVectorToFile(outfile, matchData);

	return 0;
}

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

	cout << "Do not have this team: " << name << endl;
	return -1;
}

void ExtractMatchVector(const string& input, vector<Match>& matchData, const vector<Team>& teamData)
{
	istringstream inSS;
	string teamAName, teamBName;
	int teamAScore, teamBScore;
	string day, month, year;

	inSS.clear();
	inSS.str(input);

	inSS >> teamAName;
	inSS >> teamBName;
	inSS >> teamAScore;
	inSS >> teamBScore;
	inSS >> day;
	inSS >> month;
	inSS >> year;
	inSS.clear();

	int teamAIndex = FindTeamIndex(teamAName, teamData);
	int teamBIndex = FindTeamIndex(teamBName, teamData);
	int numDay = DateToDay(day, month, year);
	int week = DateToWeek(day, month, year);
	double winActualScore;
												// ATTENTION:
	if (teamAScore + teamBScore < 16) {		// Sometimes teams drop the match, so the result is 1:0
		// Include a condition in your code that, if
							// both actual scores are 0 then skip to the next match
		return;
	}
	else if (teamAScore > 16 || teamBScore > 16 || teamAScore == teamBScore) {		// In this case, the match went to overtime
		matchData.push_back(Match(true, teamAIndex, teamBIndex, 0.5, numDay, week));
		return;
	} else if (teamAScore > teamBScore) {
		winActualScore = static_cast<double>(teamAScore)/(teamAScore + teamBScore);
		matchData.push_back(Match(false, teamAIndex, teamBIndex, winActualScore, numDay, week));
		return;
	} else if (teamBScore > teamAScore) {
		winActualScore = static_cast<double>(teamBScore)/(teamAScore + teamBScore);
		matchData.push_back(Match(false, teamBIndex, teamAIndex, winActualScore, numDay, week));
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

int DateToWeek(const string &day, const string &month, const string &year)
{
	return DateToDay(day, month, year) / 7;
}

int DateToDay(const string &day, const string &month, const string &year)
{
	int result = 0;
	// day section
	if (day.size() == 3)
	{
		result += day.at(0) - '0';
	}
	else if (day.size() == 4)
	{
		result += day.at(1) - '0';
		result += 10 * (day.at(0) - '0');
	}
	else
	{
		cerr << "Day Error" << endl;
		return -1;
	}

	// month
	int monthNum;
	if (month.substr(0,3) == "Dec") monthNum = 12;
	else if (month.substr(0,3) == "Nov") monthNum = 11;
	else if (month.substr(0,3) == "Oct") monthNum = 10;
	else if (month.substr(0,3) == "Sep") monthNum = 9;
	else if (month.substr(0,3) == "Aug") monthNum = 8;
	else if (month.substr(0,3) == "Jul") monthNum = 7;
	else if (month.substr(0,3) == "Jun") monthNum = 6;
	else if (month.substr(0,3) == "May") monthNum = 5;
	else if (month.substr(0,3) == "Apr") monthNum = 4;
	else if (month.substr(0,3) == "Mar") monthNum = 3;
	else if (month.substr(0,3) == "Feb") monthNum = 2;
	else if (month.substr(0,3) == "Jan") monthNum = 1;
	else
	{
		cerr << "Month Error" << endl;
		return -1;
	}

	switch (monthNum)
	{
		case 12:
			result += 30;
		case 11:
			result += 31;
		case 10:
			result += 30;
		case 9:
			result += 31;
		case 8:
			result += 31;
		case 7:
			result += 30;
		case 6:
			result += 31;
		case 5:
			result += 30;
		case 4:
			result += 31;
		case 3:
			result += 28;
		case 2:
			result += 31;
		case 1:
			break;
	}

	// year	(assume 20**)
	result += 365 * (year.at(3) - '5');
	result += 3650 * (year.at(2) - '1');

	return result;
}


