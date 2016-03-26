
#ifndef BASICCLASSES_H_INCLUDED
#define BASICCLASSES_H_INCLUDED

#include <iostream>
#include <string>
using namespace std;

class Team {
private:
	string name;
	int numPlay;
	double rating;
public:
	Team();
	Team(const string& nameInput);
	virtual ~Team() {}
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
};

class Match {
private:
	bool tie;		//(0 = good, 1 = tie)
	int winIndex;
	int loseIndex;
	double winActualScore;		//exclude k2 part
	int loseScore;
	int week;
public:
	Match();
	Match(bool md, int winidx, int loseidx, double winActSc, int wk);
	Match(const string& input);
	virtual ~Match() {}
	int WinTeam() const { return winIndex; }
	int LoseTeam() const { return loseIndex; }
	double WinScore() const { return winActualScore; }
	int LoseScore() const { return loseScore; }
	bool isTie() const { return tie; }
	int Week() const { return week; }
	friend ostream& operator<<(ostream& os, const Match& m);
};

// NEW CLASS: Holds roster changes information
// teamIndex - index of team that has major roster change
// date - date of change
class RosterInfo {
private:
	int teamIndex;
	string date;
public:
	RosterInfo();
	RosterInfo(const string& input);
	int Index() const { return teamIndex; }
	string Date() const { return date; }
};


#endif // BASICCLASSES_H_INCLUDED
