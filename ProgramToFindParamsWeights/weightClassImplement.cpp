#include "weightClasses.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

void constructHeap(int *array, int size);
void maxHeapify(int *array, int pos);
int extractMax(int *array, int& size);
void swap(int *array, int p1, int p2);
int Parent(int pos);
int Left(int pos);
int Right(int pos);

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

double TeamList::Average(int numPlay) const
{
	Node *temp = head;
	double average = 0.0;

	while (temp != NULL) {
		average += (temp->team)->Rating() * (static_cast<double>(temp->numMatch) / numPlay);
		temp = temp->next;
	}
	return average;
}

int TeamList::TotalMatches() const {
	Node *temp = head;
	int total = 0;
	while (temp != NULL) {
		total += (temp->numMatch);
		temp = temp->next;
	}

	return total;
}

//TeamWithNeighbor

TeamWithNeighbor::TeamWithNeighbor() : Team()
{
	numNeighbor = 0;
}

TeamWithNeighbor::TeamWithNeighbor(const string& nameInput) : Team(nameInput)
{
	numNeighbor = 0;
	totalMatch = 0;
}

double TeamWithNeighbor::AverageNeighbor() const
{
	if (NumNeighbor() == 0)
		return 1200.0;
	return neighbor.Average(totalMatch);
}

void TeamWithNeighbor::AddNeighbor(TeamWithNeighbor *teamPtr)
{
	neighbor.Insert(teamPtr);
	numNeighbor = neighbor.Size();
}

void TeamWithNeighbor::SumUpMatches() {
	totalMatch = neighbor.TotalMatches();
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