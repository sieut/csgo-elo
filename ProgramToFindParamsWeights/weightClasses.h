#ifndef WEIGHTCLASSES_H_INCLUDED
#define WEIGHTCLASSES_H_INCLUDED

#include "basicClasses.h"
#include <iostream>
#include <string>
using namespace std;

struct Node { Team* team; Node* next; };

class TeamList
{
public:
	
	Node *head;
	int size;
	TeamList() { head = NULL; size = 0; }
	~TeamList();
	bool Has(Team *tptr);
    void Insert(Team *tptr);
    //int Delete(Team *tptr);
};

class TeamWithNeighbor : public Team
{
private:
	int numNeighbor;
	TeamList neighbor;
public:
	TeamWithNeighbor();
	TeamWithNeighbor(const string& nameInput);
	int NumNeighbor() const { return numNeighbor; }
	double AverageNeighbor() const;
	void AddNeighbor(TeamWithNeighbor *teamPtr);
	friend ostream& operator<<(ostream& os, const TeamWithNeighbor& t);
};


class MatchWithWeight : public Match
{
private:
public:
	MatchWithWeight();
	MatchWithWeight(const string& input);
	double Weight() const;
	static int tmin;
	static int tmax;
};

#endif