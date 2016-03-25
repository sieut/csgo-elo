#include "weightClasses.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
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

TeamList::Node *TeamList::teamNode(Team *tptr) const
{
	if (!head)
		return NULL;
	Node *temp = head;
	// loop through the end or till find tptr
	while (temp != NULL)
	{
		if (temp->team == tptr)
			return temp;

		temp = temp->next;
	}
	// can't find tptr
	return NULL;
}

void TeamList::Insert(Team* tptr)
{
	Node* teamN = teamNode(tptr);
	if (teamN)
	{
		(teamN->numMatch)++;
	}
	else
	{
		Node *node = new Node;
		node->team = tptr;
		node->numMatch = 1;

		node->next = head;
		head = node;
		size++;
	}
}

double TeamList::Average() const
{
	vector<int> numMatchData;
	// loop through
	Node *temp = head;
	while (temp != NULL)
	{	
		numMatchData.push_back(temp->numMatch);
		temp = temp->next;
	}

	sort(numMatchData.begin(), numMatchData.end());

	//int Q1value = numMatchData.at(static_cast<int>((numMatchData.size() + 1)/4.0));
	int Q2value = numMatchData.at(static_cast<int>((numMatchData.size() + 1)/2.0));
	//int Q3value = numMatchData.at(static_cast<int>(3.0 * (numMatchData.size() + 1)/4.0));
	//cout << "Q1: " << Q1value << endl;
	//cout << "Q2: " << Q2value << endl;
	//cout << "Q3: " << Q3value << endl;
	int tempNumMatch;
	double sumRating = 0.0;
	temp = head;
	int countSize = 0;
	while (temp != NULL)
	{
		tempNumMatch = temp->numMatch;
		//cout << temp->team->Name() << " " << tempNumMatch << endl;
		if (tempNumMatch > Q2value)
		{
			//cout << "yes" << endl;
			sumRating += temp->team->Rating();
			countSize++;
		}
		temp = temp->next;
	}
	return sumRating / countSize;
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
	if (NumNeighbor() == 0)
		return 1200.0;
	return neighbor.Average();
}

void TeamWithNeighbor::AddNeighbor(TeamWithNeighbor *teamPtr)
{
	neighbor.Insert(teamPtr);
	numNeighbor = neighbor.Size();
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