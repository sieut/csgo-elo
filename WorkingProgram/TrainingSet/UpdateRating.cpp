// Function for main calculation
// mostly adapted from MatchUpdate.cpp

#include "Team.h"
#include "Hashtable.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
using namespace std;

extern double K1;
extern double K2;

void CalCulateExpectedScore(const Team* tA, const Team* tB, double& expectedA, double& expectedB);
int UpdateRatingFromMatch(HashTable& table, const Match& match);

// UpdateRating : the main function for updating rating
// -- will call UpdateRatingHelper, and UpdateRatingHelper will call CalCulateExpectedScored
int UpdateRating(HashTable& table, const vector<Match> &matchData)
{
    //RUN through matches
    double loseProb = 0.0;
    for (int i = 0; i < matchData.size(); i++)
    {
        loseProb += UpdateRatingFromMatch(table, matchData.at(i));
    }
    return loseProb;
}

int UpdateRatingFromMatch(HashTable& table, const Match& match)
{
    //teamA - win, teamB - lose
    Team* tAptr = table.Search(match.WinTeam()); //use method Search of HashTable class
    Team* tBptr = table.Search(match.LoseTeam());

    double expectedA, expectedB;
    CalCulateExpectedScore(tAptr, tBptr, expectedA, expectedB); //calculate expected score


    double result = 1 + match.WinScore() + K2;
    //add rating
    if (match.IsTie())
    {
        //k1 weight is built in AddRating
        tAptr->AddRating(K1 * (1 - 2 * expectedA));
        tBptr->AddRating(K1 * (1 - 2 * expectedB));
        return pow((expectedA - 0.5), 2);
    }
    else
    {
        tAptr->AddRating(K1 * (match.Weight() * (expectedA - result) * expectedA * (1 - expectedA)));
        tBptr->AddRating(K1 * -(match.Weight() * (expectedA - result) * expectedA * (1 - expectedA)));
        return pow((expectedA - 1.0), 2);
    }
}

//output : expectedA, expectedB
void CalCulateExpectedScore(const Team* tA, const Team* tB, double& expectedA, double& expectedB)
{
	expectedA = 1 / ( 1 + pow(10, (tB->Rating() - tA->Rating()) / 400) );
	expectedB = 1 - expectedA;
}
