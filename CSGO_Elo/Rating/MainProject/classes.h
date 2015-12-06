#ifndef CLASSES_H_INCLUDED
#define CLASSES_H_INCLUDED

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
	const string& Name() const { return name; }
	double Rating() const { return rating; }
	double K1weight() const;
	void AddRating(double added);       //also add numPlay
	void Reset();
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
	string date;
public:
	Match();
	Match(bool md, int winidx, int loseidx, double winActSc, string matchDate);
	Match(const string& input);
	int WinTeam() const { return winIndex; }
	int LoseTeam() const { return loseIndex; }
	double WinScore() const { return winActualScore; }
	bool isTie() const { return tie; }
	friend ostream& operator<<(ostream& os, const Match& m);
};


#endif // CLASSES_H_INCLUDED
