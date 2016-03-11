#include "basicClasses.h"
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

void Team::AdjustNumPlay() {
	if (numPlay >= 150) numPlay = 140;
	else if (numPlay >= 100) numPlay = 90;
	else if (numPlay >= 40) numPlay = 30;
}

ostream& operator<<(ostream& os, const Team& t)
{
	os << t.name << "  " << t.rating << " " << t.numPlay;
	return os;
}


Match::Match()
{
	tie = false;
	winIndex = loseIndex = winActualScore = week = -1;
}

Match::Match(bool md, int winidx, int loseidx, double winActSc, int wk)
{
	tie = md;
	winIndex = winidx;
	loseIndex = loseidx;
	winActualScore = winActSc;
	week = wk;
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
	inSS >> week;
}

ostream& operator<<(ostream& os, const Match& m)
{
	os << static_cast<int>(m.tie)  << " " << m.winIndex << " "
	   << m.loseIndex << " " << m.winActualScore << "" << m.week;
    return os;
}


RosterInfo::RosterInfo() {
	teamIndex = 0;
	date = "";
}

RosterInfo::RosterInfo(const string& input) {
	istringstream inSS(input);

	inSS >> teamIndex;
	getline(inSS, date);
}

void CreateTeamVector(vector<Team>& teamData)
{
	string inFileName = "teamList.txt";
	//cout << "Print \"teamList.txt\": ";
	//cin >> inFileName;

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
	string inFileName = "matchIndex.txt";
	//cout << "Print \"matchIndex.txt\": ";
	//cin >> inFileName;

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

void CreateRosterVector(vector<RosterInfo>& rosterData) {
	string inFileName = "rosterChange.txt";
	//cout << "Print \"rosterChange.txt\": ";
	//cin >> inFileName;

	ifstream infile(inFileName.c_str());
	if (!infile.is_open()) {
		cerr << "Cannot open roster file." << endl;
		return;
	}

	string line;
	getline(infile, line);
	while (infile.good()) {
		rosterData.push_back(RosterInfo(line));
		getline(infile, line);
	}

	if (!infile.eof()) {
		cerr << "Cannot read til the end." << endl;
		return;
	}
}
