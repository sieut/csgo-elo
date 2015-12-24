//This file contains 2 functions
//   1. update rating
//   2. compute result -- the end of this file

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

void UpdateRatingHelper(HashTable& table, const Match& match, bool add, ostream& outfile);
void CalCulateExpectedScore(const Team* tA, const Team* tB, double& expectedA, double& expectedB);
bool CheckTeam(const HashTable& table, const string& teamName, bool print);
void InputRosterData(vector<RosterInfo>& rosterData);
bool SetAutoAdd();
void CheckTeamAdd(HashTable& table, const string& teamName, bool autoAdd);
void UpdateRatingFromMatch(HashTable& table, const Match& match, ostream& outfile);

// UpdateRating : the main function for updating rating
// -- will call UpdateRatingHelper, and UpdateRatingHelper will call CalCulateExpectedScored
void UpdateRating(HashTable& table, string& lastMatch, ostream& outfile)
{
    cout << lastMatch << endl;

    //KEEP all roster data from roster file to "rosterData"
    vector<RosterInfo> rosterData;
    InputRosterData(rosterData);

    //Open match file
    ifstream infile;
    string matchFileName;
    cout << "Match file (no .txt): ";
    cin >> matchFileName;
    matchFileName += ".txt";
    infile.open(matchFileName.c_str());
    if (!infile.is_open())
    {
        cerr << "Cannot open match file." << endl;
        return;
    }

    //Feature: add unknown team automatically or manually?
    //autoAdd = true -- automatically, false -- manually
    bool autoAdd = SetAutoAdd();

    //RUN through matches
    int rosterInfoIdx = 0;
    string line;
    getline(infile, line);
    while (infile.good())
    {
        Match matchtemp(line);
        cout << "line: " << line << endl;
        //if roster date == match date --> AdjustNumPlay
        while (rosterInfoIdx < rosterData.size() && rosterData.at(rosterInfoIdx).Date() == matchtemp.Date())
        {
            table.Search(rosterData.at(rosterInfoIdx).Team())->AdjustNumPlay();
            rosterInfoIdx++;
        }
        UpdateRatingHelper(table, matchtemp, autoAdd, outfile);  //UpdateRating!
        lastMatch = line;   //keep the last line to put at the beginning of output file
        getline(infile, line);
    }
    infile.close();
    lastMatch = "The last match: " + lastMatch; //put at the beginning of output file

    cout << "Data are updated (don't forget to print)" << endl << endl;
}

void UpdateRatingHelper(HashTable& table, const Match& match, bool autoAdd, ostream& outfile)
{
    //Check if we have the teams in our record. Is this a new team?
    CheckTeamAdd(table, match.WinTeam(), autoAdd);
    CheckTeamAdd(table, match.LoseTeam(), autoAdd);
    UpdateRatingFromMatch(table, match, outfile);
}

void InputRosterData(vector<RosterInfo>& rosterData)
{
    ifstream infile;
    string line;
    string rosterFileName;
    cout << "Roster file (no .txt) or print \"no\": ";
    cin >> rosterFileName;
    if (rosterFileName != "no") //rosterFileName = no means no roster file
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
}

bool SetAutoAdd()
{
    string addString;
    bool autoAdd = false;
    cout << "Add all unknown teams automatically? \"yes\" or \"no\": ";
    cin >> addString;
    while (addString != "yes" && addString != "no")
    {
        cout << "type again" << endl;
        cin >> addString;
    }
    if (addString == "yes")
        autoAdd = true;
    return autoAdd;
}

void UpdateRatingFromMatch(HashTable& table, const Match& match, ostream& outfile)
{
    //teamA - win, teamB - lose
    Team* tAptr = table.Search(match.WinTeam()); //use method Search of HashTable class
    Team* tBptr = table.Search(match.LoseTeam());

    double expectedA, expectedB;
    CalCulateExpectedScore(tAptr, tBptr, expectedA, expectedB); //calculate expected score
    double L;

    //add rating
    if (match.IsTie())
    {
        //k1 weight is built in AddRating
        tAptr->AddRating(K1 * (0.5 - expectedA));
        tBptr->AddRating(K1 * (0.5 - expectedB));
        //tAptr->AddRating(K1 * (1 - 2 * expectedA));
        //tBptr->AddRating(K1 * (1 - 2 * expectedB));
    }
    else
    {

        outfile << "Match: " << match << endl;
        outfile << "Before: Winner: " << *tAptr << " Loser: " << *tBptr << endl;
        tAptr->AddRating(K1 * (1 - expectedA));
        tBptr->AddRating(K1 * (-expectedB));
        //tAptr->AddRating(K1 * (1 + match.WinScore() + K2 - 2 * expectedA));
        //tBptr->AddRating(K1 * (1 - (match.WinScore() + K2) - 2 * expectedB));
        outfile << "After:  Winner: " << *tAptr << " Loser: " << *tBptr << endl;
        L = pow((expectedA - 1.0), 2);
        outfile << "L: " << L << endl;
        outfile << endl;

    }
}

void CheckTeamAdd(HashTable& table, const string& teamName, bool autoAdd)
{
    if (!CheckTeam(table, teamName, false)) //Check if we have the winning team
    {
    	if (autoAdd == false)   //if not autoAdd - asks user
        {
            cout << "UpdateRating: we don't have the team " << teamName << endl;
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
        }
        table.Insert(teamName, 1200.0);
        cout << "AddTeam " << teamName << " successfully" << endl;
    }
}

//output : expectedA, expectedB
void CalCulateExpectedScore(const Team* tA, const Team* tB, double& expectedA, double& expectedB)
{
	expectedA = 1 / ( 1 + pow(10, (tB->Rating() - tA->Rating()) / 400) );
	expectedB = 1 - expectedA;
}

//ComputeResult : main function for computing result
// -- will ask for two team names and print the odd
// output format:
// Result (team1:team2)--
// team1odd:team2odd
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

    Team* t1ptr = table.Search(team1);  //use Search method from HashTable class
    Team* t2ptr = table.Search(team2);

    double expectedA, expectedB;
    CalCulateExpectedScore(t1ptr, t2ptr, expectedA, expectedB); //find expected score of each team

    int result = static_cast<int>(round(expectedA * 100));
    cout << "Result (" << team1 << ":" << team2 << ")--" << endl
         << result << ":" << 100 - result << endl << endl;

}
