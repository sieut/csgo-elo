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
	double AverageNeighbor();
	void AddNeighbor(TeamWithNeighbor *teamPtr);
};


class MatchWithWeight : public Match
{
private:
	double weight;
public:
	MatchWithWeight();
	MatchWithWeight(const string& input);
	double Weight() const { return weight; }
	static int tmin;
	static int tmax;
};

#endif