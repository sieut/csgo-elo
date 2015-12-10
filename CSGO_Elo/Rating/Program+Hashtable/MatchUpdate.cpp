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

void UpdateRatingHelper(HashTable& table, const Match& match);
void CalCulateExpectedScore(const Team* tA, const Team* tB, double& expectedA, double& expectedB);
bool CheckTeam(const HashTable& table, const string& teamName, bool print);

void UpdateRating(HashTable& table, string& lastMatch)
{
    cout << lastMatch << endl;
    //keep roster file
    ifstream infile;
    string line;
    vector<RosterInfo> rosterData;
    string rosterFileName;
    cout << "Roster file (no .txt) or print \"no\": ";
    cin >> rosterFileName;
    if (rosterFileName != "no")
    {
        rosterFileName += ".txt";
        infile.open(rosterFileName.c_str());

        if (!infile.is_open())
        {
            cerr << "Cannot open roster file." << endl;
            return;
        }

        getline(infile, line);
        while (infile.good())
        {
            rosterData.push_back(RosterInfo(line));
            getline(infile, line);
        }
        infile.close();
    }

    //run through matches
    string matchFileName;
    cout << "Match file (no .txt): ";
    cin >> matchFileName;
    matchFileName += ".txt";
    infile.open(matchFileName.c_str());
    if (!infile.is_open())
    {
        cerr << "Cannot open roster file." << endl;
        return;
    }
    int rosterInfoIdx = 0;

    getline(infile, line);
    while (infile.good())
    {
        Match matchtemp(line);
        cout << "line: " << line << endl;
        while (rosterInfoIdx < rosterData.size() && rosterData.at(rosterInfoIdx).Date() == matchtemp.Date())
        {
            table.Search(rosterData.at(rosterInfoIdx).Team())->AdjustNumPlay();
            rosterInfoIdx++;
        }
        UpdateRatingHelper(table, matchtemp);
        lastMatch = line;
        getline(infile, line);
    }
    infile.close();
    lastMatch = "The last match: " + lastMatch;

    cout << "Data are updated (don't forget to print)" << endl << endl;
}

void UpdateRatingHelper(HashTable& table, const Match& match)
{
    //Check if we have the teams in our record. Is this a new team?
    if (!CheckTeam(table, match.WinTeam(), false))
    {
    	/*
        cout << "UpdateRating: we don't have the team " << match.WinTeam() << endl;
        cout << "Want to \"add\" it or \"exit\" the function: ";
        string command;
        cin >> command;
        while (command != "add" && command != "exit")
        {
            cout << "type again!";
            cin >> command;
        }
        if (command == "exit")
            return;
	*/
        table.Insert(match.WinTeam(), 1200.0);
        cout << "AddTeam " << match.WinTeam() << " successfully" << endl;
    }

    if (!CheckTeam(table, match.LoseTeam(), false))
    {
    	/*
        cout << "UpdateRating: we don't have the team " << match.LoseTeam() << endl;
        cout << "Want to \"add\" it or \"exit\" the function: ";
        string command;
        cin >> command;
        while (command != "add" && command != "exit")
        {
            cout << "type again!";
            cin >> command;
        }
        if (command == "exit")
            return;
	*/
        table.Insert(match.LoseTeam(), 1200.0);
        cout << "AddTeam " << match.LoseTeam() << " successfully" << endl;
    }

    //teamA - win, teamB - lose
    Team* tAptr = table.Search(match.WinTeam());
    Team* tBptr = table.Search(match.LoseTeam());

    double expectedA, expectedB;
    CalCulateExpectedScore(tAptr, tBptr, expectedA, expectedB);

    if (match.IsTie())
    {
        //k1 weight is built in AddRating
        tAptr->AddRating(K1 * (1 - 2 * expectedA));
        tBptr->AddRating(K2 * (1 - 2 * expectedB));
    }
    else
    {
        tAptr->AddRating(K1 * (1 + match.WinScore() + K2 - 2 * expectedA));
        tBptr->AddRating(K1 * (1 - (match.WinScore() + K2) - 2 * expectedB));
    }
}

void CalCulateExpectedScore(const Team* tA, const Team* tB, double& expectedA, double& expectedB)
{
	expectedA = 1 / ( 1 + pow(10, (tB->Rating() - tA->Rating()) / 400) );
	expectedB = 1 - expectedA;
}

void ComputeResult(HashTable& table)
{
    string team1, team2;
    cout << "team1 name: ";
    cin >> team1;
    cout << "team2 name: ";
    cin >> team2;

    //Check if we have the teams in our record. Is this a new team?
    if (!CheckTeam(table, team1, false))
    {
        cout << "UpdateRating: we don't have the team " << team1 << endl;
        cout << "Go back" << endl << endl;
        return;
    }

    if (!CheckTeam(table, team2, false))
    {
        cout << "UpdateRating: we don't have the team " << team2 << endl;
        cout << "Go back" << endl << endl;
        return;
    }

    Team* t1ptr = table.Search(team1);
    Team* t2ptr = table.Search(team2);

    double expectedA, expectedB;
    CalCulateExpectedScore(t1ptr, t2ptr, expectedA, expectedB);

    int result = static_cast<int>(round(expectedA * 100));
    cout << "Result (" << team1 << ":" << team2 << ")--" << endl
         << result << ":" << 100 - result << endl << endl;

}
