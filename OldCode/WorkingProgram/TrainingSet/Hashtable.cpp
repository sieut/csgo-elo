//This file contains
//  1. Definition of all methods in TeamLinkedList and HashTable class
//  2. InputData function  -- used from main
//  3. PrintTable function  -- used from main


#include "Hashtable.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <cmath>
using namespace std;

// TeamLinkedList Class
TeamLinkedList::TeamLinkedList()
{
    head = NULL;
}

TeamLinkedList::~TeamLinkedList()
{
    Team* teamptr;
    while (head != NULL)
    {
        teamptr = head;
        head = head->next;
        delete teamptr;
    }
}

//Search:
//   @input: teamName
//   @return Team pointer
Team* TeamLinkedList::Search(const string& teamName) const
{
    Team* teamptr = head;
    while (teamptr != NULL && teamptr->Name() != teamName)
    {
        teamptr = teamptr->next;
    }
    return teamptr;
}

//Insert:
//   @input: Team pointer
void TeamLinkedList::Insert(Team* tptr)
{
    tptr->next = head;
    if (head != NULL)
        head->prev = tptr;
    head = tptr;
    tptr->prev = NULL;
}

//Delete
//   @input: Team pointer
void TeamLinkedList::Delete(Team* tptr)
{
    if (tptr->prev != NULL)
        tptr->prev->next = tptr->next;
    else
        head = tptr->next;
    if (tptr->next != NULL)
        tptr->next->prev = tptr->prev;
}

void TeamLinkedList::Delete(const string& teamName)
{
    Delete(Search(teamName));
}

ostream& operator<<(ostream& os, const TeamLinkedList& tll)
{
    Team* teamptr = tll.head;
    while (teamptr != NULL)
    {
        os << *teamptr << endl;
        teamptr = teamptr->next;
    }
    return os;
}

// HashTable Class
HashTable::HashTable()
{
}

HashTable::~HashTable()
{
    for (int i = 0; i < T_SIZE; i++)
    {
        table[i].~TeamLinkedList();
    }
}

//HF: Hash table function: I just random the number of it make the team Name distributed
//   @input: teamName
//   @return: hash table index
int HashTable::HF(const string& teamName) const
{
    double value = 0;
    double mult;
    for (int i = 0; i < teamName.size(); i++)
	{
		mult = 1313.23 * pow(1.37, i);
		value += mult * static_cast<int>(teamName.at(i));
	}
    int Tindex = static_cast<int>(value) % T_SIZE;

    return Tindex;
}
Team* HashTable::Search(const string& teamName) const
{
    table[HF(teamName)].Search(teamName);
}

// Insert
void HashTable::Insert(Team* tptr)  //main one
{
    table[HF(tptr->Name())].Insert(tptr);
}

void HashTable::Insert(const string& input)
{
    Team* teamptr = new Team(input);
    Insert(teamptr);
}

// Delete
void HashTable::Delete(Team* tptr)  //main one
{
    table[HF(tptr->Name())].Delete(tptr);
}

void HashTable::Delete(const string& teamName)
{
    Delete(Search(teamName));
}

ostream& operator<<(ostream& os, const HashTable& ht)
{
    for (int i = 0; i < T_SIZE; i++)
    {
        os << ht.table[i];
    }
    return os;
}
