#include "classes.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
using namespace std;

Team::Team()
{
	name = "--null--";
	rating = 1200;
	numPlay = 0;
}

Team::Team(const string& nameInput)
{
	name = nameInput;
	rating = 1200;
	numPlay = 0;
}

double Team::K1weight() const
{
    double result;
    if (numPlay <= 40)
        result = 2.0;
    else if (numPlay <= 100)
        result = 1.0;
    else if (numPlay <= 150)
        result = 0.5;
    else
        result = 0.25;

    return result;
}

void Team::AddRating(double added)
{
    rating += K1weight() * added;
    numPlay++;
}
void Team::Reset()
{
    numPlay = 0;
    rating = 1200;
}

ostream& operator<<(ostream& os, const Team& t)
{
	os << t.name << "  " << t.rating << " " << t.numPlay;
	return os;
}


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

Match::Match(const string& input)
{
	istringstream inSS;

	inSS.clear();
	inSS.str(input);

	inSS >> tie;
	inSS >> winIndex;
	inSS >> loseIndex;
	inSS >> winActualScore;
}

ostream& operator<<(ostream& os, const Match& m)
{
	os << static_cast<int>(m.tie)  << " " << m.winIndex << " "
	   << m.loseIndex << " " << m.winActualScore;
    return os;
}

void CreateTeamVector(vector<Team>& teamData)
{
	string inFileName;
	cout << "Teamlist file(with \".txt\"): ";
	cin >> inFileName;

	ifstream infile(inFileName.c_str());
	if (!infile.is_open())
	{
		cerr << "Cannot open team file." << endl;
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
		cerr << "Cannot read til the end (team)." << endl;
		return;
	}
}

void CreateMatchVector(vector<Match>& matchData)
{
	string inFileName;
	cout << "Matchlist file(with \".txt\"): ";
	cin >> inFileName;

	ifstream infile(inFileName.c_str());
	if (!infile.is_open())
	{
		cerr << "Cannot open match file." << endl;
		return;
	}
	string line;
	getline(infile, line);
	while (infile.good())
	{
		matchData.push_back(Match(line));
		getline(infile, line);
	}

	if (!infile.eof())
	{
		cerr << "Cannot read til the end (match)." << endl;
		return;
	}
}
