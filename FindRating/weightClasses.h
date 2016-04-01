#ifndef WEIGHTCLASSES_H_INCLUDED
#define WEIGHTCLASSES_H_INCLUDED

#include "basicClasses.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class TeamList
{
private:
	//typedef struct node Node;
	struct Node { Team* team; int numMatch; Node* next; };
	Node *head;
	int size;
public:
	TeamList() { head = NULL; size = 0; }
	~TeamList();

	// find team, return Node of the team or NULL if not found
	Node* teamNode(Team *tptr) const;

	// insert the team / plus the numMatch if already have
    void Insert(Team *tptr);
    
    // care only Q1 - max (min - Q1 deleted)
    double Average(int numPlay) const;

    int TotalMatches() const;

    int Size() const { return size; }

    //int Delete(Team *tptr);
};

class TeamWithNeighbor : public Team
{
private:
	int numNeighbor;
	int totalMatch;
	TeamList neighbor;
public:
	TeamWithNeighbor();
	TeamWithNeighbor(const string& nameInput);
	int NumNeighbor() const { return numNeighbor; }
	double AverageNeighbor() const;
	void AddNeighbor(TeamWithNeighbor *teamPtr);
	void SumUpMatches();
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