#ifndef HASHTABLE_H_INCLUDED
#define HASHTABLE_H_INCLUDED

#include "Team.h"
#include <iostream>
#include <string>
using namespace std;

static const int T_SIZE = 1543;

class TeamLinkedList {
private:
    Team* head;

public:
    TeamLinkedList();
    ~TeamLinkedList();
    Team* Search(const string& teamName) const;
    void Insert(Team* tptr);
    void Delete(Team* tptr);
    void Delete(const string& teamName);
    friend ostream& operator<<(ostream& os, const TeamLinkedList& tll);
};

class HashTable {
private:
    TeamLinkedList table[T_SIZE];
    int HF(const string& teamName) const;     //hash function

public:
    HashTable();
    Team* Search(const string& teamName) const;
    void Insert(Team* tptr);
    void Insert(const string& input);
    void Insert(const string& teamName, double rt, int play = 0);
    void Delete(Team* tptr);
    void Delete(const string& teamName);
    friend ostream& operator<<(ostream& os, const HashTable& ht);
};

#endif // HASHTABLE_H_INCLUDED
