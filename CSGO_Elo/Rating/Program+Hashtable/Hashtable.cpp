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
//
void TeamLinkedList::Insert(Team* tptr)
{
    tptr->next = head;
    if (head != NULL)
        head->prev = tptr;
    head = tptr;
    tptr->prev = NULL;
}

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

void HashTable::Insert(Team* tptr)
{
    table[HF(tptr->Name())].Insert(tptr);
}

void HashTable::Insert(const string& input)
{
    Team* teamptr = new Team(input);
    Insert(teamptr);
}

void HashTable::Insert(const string& teamName, double rt, int play)
{
    Team* teamptr = new Team(teamName, rt, play);
    Insert(teamptr);
}

void HashTable::Delete(Team* tptr)
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

// Other functions
void InputData(HashTable& table, string& info)
{
    string inputFileName;
    cout << "Input data file (no .txt): ";
    cin >> inputFileName;
    inputFileName += ".txt";

    ifstream infile(inputFileName.c_str());

    if (!infile.is_open())
    {
    	cerr << inputFileName << " cannot be opened" << endl;
    	cerr << "Exit!" << endl;
    	exit(EXIT_FAILURE);
    }

    getline(infile, info);
    cout << "INFO: " << info << endl;

    string temp;
    getline(infile, temp);
    while (infile.good())
    {
        table.Insert(temp);
        getline(infile, temp);
    }
    infile.close();

    cout << "Input data successfully" << endl << endl;
}

void PrintTable(const HashTable& table, const string& lastMatch)
{
    string outputFileName;
    string date;

    cout << "Name of output file (no .txt): ";
    cin >> outputFileName;
    outputFileName += ".txt";

    ofstream outfile(outputFileName.c_str());
    outfile << lastMatch << endl;
    outfile << table;       //no endline needed
    outfile.close();

    cout << "Print data successfully" << endl << endl;
}
