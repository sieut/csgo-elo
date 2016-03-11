
#include "weightClasses.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;

// TeamList
TeamList::~TeamList()
{
	Node *temp;
	while (head != NULL)
	{
		temp = head;
		head = head->next;
		delete temp;
	}
}

bool TeamList::Has(Team *tptr)
{
	if (!head)
		return false;
	Node *temp = head;
	// loop through the end or till find tptr
	while (temp != NULL && temp->team != tptr)
	{
		temp = temp->next;
	}
	// can't find tptr
	if (temp->team == tptr)
		return true;
	else 
		return false;
}

void TeamList::Insert(Team* tptr)
{
	if (Has(tptr))
		return;
	Node *node = new Node;
	node->team = tptr;
	node->next = head;
	head = node;
	size++;
}

/*int TeamList::Delete(Team* tptr)
{
	// list is empty
	if (!head)
		return 0;

	// tptr is at the head
	if (head->team == tptr)
	{
		Node *temp = head;	
		head = (head)->next;p
		delete temp;		
		size--;
		return 1;
	}

	Node *temp = head;
	// loop through the end or till find p
	while (temp->next != NULL && temp->next->team != tptr)
	{
		temp = temp->next;
	}
	// can't find tptr
	if (temp->next == NULL)
		return 0;

	// find tptr
	Node *temp2 = temp->next; 	
	temp->next = temp->next->next;	
	delete temp2
	size--;				
	return 1;
}*/

//TeamWithNeighbor

TeamWithNeighbor::TeamWithNeighbor() : Team()
{
	numNeighbor = 0;
}

TeamWithNeighbor::TeamWithNeighbor(const string& nameInput) : Team(nameInput)
{
	numNeighbor = 0;
}

double TeamWithNeighbor::AverageNeighbor() const
{
	Node *temp = neighbor.head;
	double sumRating = 0;
	while (temp != NULL)
	{
		sumRating += temp->team->Rating();
		temp = temp->next;
	}
	return sumRating / numNeighbor;
}

void TeamWithNeighbor::AddNeighbor(TeamWithNeighbor *teamPtr)
{
	neighbor.Insert(teamPtr);
	numNeighbor = neighbor.size;
}

ostream& operator<<(ostream& os, const TeamWithNeighbor& t)
{
	os << (const Team &) t << " " << t.AverageNeighbor();
}

//MatchWithWeight
int MatchWithWeight::tmin = 0;
int MatchWithWeight::tmax = -1;

MatchWithWeight::MatchWithWeight() : Match()
{
}


MatchWithWeight::MatchWithWeight(const string& input) : Match(input)
{
}

double MatchWithWeight::Weight() const
{
	double weight = (static_cast<double>(1 + Week() - tmin)/(1 + tmax - tmin));
	return weight * weight;
}