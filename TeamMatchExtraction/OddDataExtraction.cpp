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

class Match {
private:
	bool tie;		//(0 = good, 1 = tie)
	int winIndex;
	int loseIndex;
	double odd;
	int day;
public:
	Match();
	Match(bool md, int winidx, int loseidx, double winActSc, int _day);
	bool isTie() const { return tie; }
	friend ostream& operator<<(ostream& os, const Match& m);
};

Match::Match()
{
	tie = false;
	winIndex = loseIndex = winActualScore = day = -1;

}

Match::Match(bool md, int winidx, int loseidx, double winActSc, int _day)
{
	tie = md;
	winIndex = winidx;
	loseIndex = loseidx;
	winActualScore = winActSc;
	day = _day;
}

ostream& operator<<(ostream& os, const Match& m)
{
	os << static_cast<int>(m.tie)  << " " << m.winIndex << " "
	   << m.loseIndex << " " << m.winActualScore << " " << m.day << " " << m.week;
    return os;
}