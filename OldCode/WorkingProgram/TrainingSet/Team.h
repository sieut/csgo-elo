#ifndef TEAM_H_INCLUDED
#define TEAM_H_INCLUDED

// 3 classes: Team, Match, RosterInfo
#include <iostream>
#include <string>
using namespace std;

class Team {
private:
	string name;
	double rating;
	int numPlay;
	Team **neighbor;
	int numNeighbor;
	double K1weight() const;
public:
	Team();
	Team(const string& teamName);
	const string& Name() const { return name; }
	double Rating() const { return rating; }
	int NumPlay() const { return numPlay; }
	int NumNeighbor() const { return numNeighbor; }

	void AddRating(double added);       //also add numPlay
	void Reset();                       //Reset to 1200, 0

	void AdjustNumPlay();		// Change numPlay due to roster change
	friend ostream& operator<<(ostream& os, const Team& t);

	//public pointer members to make object a doubly linked list element
	Team* prev;
	Team* next;
};

class Match {
private:
	char mode;		//(w = win, t = tie, f = fail)
	string winName;
	string loseName;
	double winActualScore;		//exclude k2 part
	int week;
	double weight;
public:
	Match();
	Match(const string& input);     //input expected to be a line from input file
	string WinTeam() const { return winName; }
	string LoseTeam() const { return loseName; }
	double WinScore() const { return winActualScore; }
	bool IsTie() const { return mode == 't'; }
	char Mode() const { return mode; }
	int Week() const { return week; }
	double Weight() const { return weight; }
	friend ostream& operator<<(ostream& os, const Match& m);
	static int tmin;
	static int tmax;
};

#endif // TEAM_H_INCLUDED