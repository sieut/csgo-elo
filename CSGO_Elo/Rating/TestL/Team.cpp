//This file contains all methods in Team, Match and RosterInfo class

#include "Team.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;

// Team class
Team::Team()
{
	name = "**NULL**";
	rating = 1200;
	numPlay = 0;
	prev = next = NULL;
}

Team::Team(const string& nameInput)
{
    istringstream inSS(nameInput);
    inSS >> name;
    inSS >> rating;
    inSS >> numPlay;
	prev = next = NULL;
}

Team::Team(const string& teamName, double rt, int play)
{
    name = teamName;
    rating = rt;
    numPlay = play;
    prev = next = NULL;
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

void Team::ChangeData(double newRating, int newNumPlay)
{
    rating = newRating;
    numPlay = newNumPlay;
}

void Team::AdjustNumPlay()
{
	if (numPlay >= 150)
        numPlay = 140;
	else if (numPlay >= 100)
        numPlay = 90;
	else if (numPlay >= 40)
        numPlay = 30;
}

ostream& operator<<(ostream& os, const Team& t)
{
	os << t.name << "  " << t.rating << " " << t.numPlay;
	return os;
}

// Match class
Match::Match()
{
	mode = 'w';
	winName = loseName = "*-null-*";
	winActualScore = -1;
}

Match::Match(char md, string win, string lose, double winActSc, string matchDate)
{
	mode = md;
	winName = win;
	loseName = lose;
	winActualScore = winActSc;
	date = matchDate;
}

Match::Match(const string& input)
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
	getline(inSS, date);  //date got here

	inSS.clear();
												// ATTENTION:
	if (teamAScore + teamBScore < 16)
    {		// Sometimes teams drop the match, so the result is 1:0
		// Include a condition in your code that, if
							// both actual scores are 0 then skip to the next match
        mode = 'f';
        winName = teamAName;
        loseName = teamBName;
        winActualScore = -1.0;
	}
	else if (teamAScore > 16 || teamBScore > 16 || teamAScore == teamBScore)
	{		// In this case, the match went to overtime
		mode = 't';
		winName = teamAName;
        loseName = teamBName;
		winActualScore = 0.5;

	}
	else if (teamAScore > teamBScore)
    {
		winActualScore = static_cast<double>(teamAScore)/(teamAScore + teamBScore);
		mode = 'w';
		winName = teamAName;
		loseName = teamBName;

	}
	else if (teamBScore > teamAScore)
    {
		winActualScore = static_cast<double>(teamBScore)/(teamAScore + teamBScore);
		mode = 'w';
		winName = teamBName;
		loseName = teamAName;
	}
}

ostream& operator<<(ostream& os, const Match& m)
{
	os << m.mode  << " " << m.winName << " "
	   << m.loseName << " " << m.winActualScore << " " << m.date;
    return os;
}

// RosterInfo class
RosterInfo::RosterInfo()
{
	teamName = "-*null*-";
	date = "";
}

RosterInfo::RosterInfo(const string& input)
{
	istringstream inSS(input);

	inSS >> teamName;
	getline(inSS, date);
}

