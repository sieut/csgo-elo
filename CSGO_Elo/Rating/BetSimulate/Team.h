#ifndef TEAM_H_INCLUDED
#define TEAM_H_INCLUDED

#include <iostream>
#include <string>
using namespace std;

class Team {
private:
	string name;
	double rating;
	int numPlay;
public:
	Team();
	Team(const string& nameInput);
	Team(const string& teamName, double rt, int play = 0);
	const string& Name() const { return name; }
	double Rating() const { return rating; }
	int NumPlay() const { return numPlay; }
	double K1weight() const;
	void AddRating(double added);       //also add numPlay
	void Reset();
	void AdjustNumPlay();		// Change numPlay due to roster change
	bool operator>(const Team& t) const { return name > t.name; }
	bool operator<(const Team& t) const { return name < t.name; }
	bool operator==(const Team& t) const { return name == t.name; }
	friend ostream& operator<<(ostream& os, const Team& t);
	Team* prev;
	Team* next;
};

class Match {
private:
	char mode;		//(w = win, t = tie, f = fail)
	string winName;
	string loseName;
	double winActualScore;		//exclude k2 part
	string date;
public:
	Match();
	Match(char md, string win, string lose, double winActSc, string matchDate);
	Match(const string& input);
	string WinTeam() const { return winName; }
	string LoseTeam() const { return loseName; }
	double WinScore() const { return winActualScore; }
	bool IsTie() const { return mode == 't'; }
	char Mode() const { return mode; }
	string Date() const { return date; }
	friend ostream& operator<<(ostream& os, const Match& m);
};

// NEW CLASS: Holds roster changes information
// teamIndex - index of team that has major roster change
// date - date of change
class RosterInfo {
private:
	string teamName;
	string date;
public:
	RosterInfo();
	RosterInfo(const string& input);
	string Team() const { return teamName; }
	string Date() const { return date; }
};

#endif // TEAM_H_INCLUDED

