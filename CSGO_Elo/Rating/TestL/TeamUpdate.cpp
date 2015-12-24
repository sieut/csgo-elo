//This file contains
//   3. manage team

#include "Team.h"
#include "Hashtable.h"
#include <iostream>
#include <string>
using namespace std;

//4 functions the main UpdateTeam will call
bool CheckTeam(const HashTable& table, const string& teamName, bool print);
void AddTeam(HashTable& table);
void DeleteTeam(HashTable& table);
void ChangeName(HashTable& table);
void ChangeTeamData(HashTable& table);

//UpdateTeam : main function for updating team (name)
void UpdateTeam (HashTable& table)
{
    cout << "What to do with TEAM (case-sensitive): " << endl
         << "1) \"check\"  "
         << "2) \"add\"  "
         << "3) \"delete\"  "
         << "4) \"changeName\"  "
         << "5) \"changeTeamData\"  "
         << "6) \"back\"  " << endl;

    string command;
    cin >> command;
    while (command != "back")
    {
        if (command == "check")
        {
            //only function that asks teamName here
            string teamName;
            cout << "teamName: ";
            cin >> teamName;
            CheckTeam(table, teamName, true);
        }
        else if (command == "add") AddTeam(table);
        else if (command == "delete") DeleteTeam(table);
        else if (command == "changeName") ChangeName(table);
        else if (command == "changeTeamData") ChangeTeamData(table);
        else
            cout << "type again!" << endl;

        cout << "What to do with TEAM (case-sensitive): " << endl
             << "1) \"check\"  "
             << "2) \"add\"  "
             << "3) \"delete\"  "
             << "4) \"changeName\"  "
             << "5) \"changeTeamData\"  "
             << "6) \"back\"  " << endl;
        cin >> command;
    }
    cout << "back to main" << endl;
}

// returns true if have this team, false if don't have this team
// if print = yes means print, no means not print
bool CheckTeam(const HashTable& table, const string& teamName, bool print)
{
    Team* result = table.Search(teamName);  //uses Search method from HashTable class
    if (result == NULL) // no this team
    {
        if (print)
            cout << "No this team name in the data" << endl << endl;
        return false;
    }
    else    // print teamData
    {
        if (print)
            cout << *result << endl << endl;
        return true;
    }
}

void AddTeam(HashTable& table)
{
    string teamName;
    cout << "teamName: ";
    cin >> teamName;
    if (CheckTeam(table, teamName, false))  //have this team
    {
        cout << "AddTeam failed: already have " << teamName << endl << endl;
    }
    else
    {
        table.Insert(teamName, 1200.0); //insert new team with rating 1200, numPlay 0
        cout << "AddTeam " << teamName << " successfully" << endl << endl;
    }
}

void DeleteTeam(HashTable& table)
{
    string teamName;
    cout << "teamName: ";
    cin >> teamName;
    if (!CheckTeam(table, teamName, false)) //don't have this team
    {
        cout << "DeleteTeam failed: " << teamName << "does not exist" << endl << endl;
    }
    else
    {
        table.Delete(teamName);
        cout << "DeleteTeam " << teamName << " successfully" << endl << endl;
    }
}

void ChangeName(HashTable& table)
{
    string nameFrom, nameTo;
    cout << "from: ";
    cin >> nameFrom;
    cout << "to: ";
    cin >> nameTo;
    if (!CheckTeam(table, nameFrom, false)) //don't have this team
    {
        cout << "ChangeName failed: " << nameFrom << "does not exist" << endl << endl;
    }
    else
    {
        Team* teamptr = table.Search(nameFrom);
        table.Insert(nameTo, teamptr->Rating(), teamptr->NumPlay());
        table.Delete(teamptr);
        cout << "ChangeName successfully" << endl << endl;
    }
}

void ChangeTeamData(HashTable& table)
{
    string teamName;
    cout << "teamName: ";
    cin >> teamName;
    Team* result = table.Search(teamName);  //uses Search method from HashTable class
    double newRating;
    int newNumPlay;
    if (result == NULL) // no this team
        cout << "No this team name in the data" << endl << endl;
    else    // print teamData
    {
        cout << "Current data: " << *result << endl << endl;
        cout << "New rating?: ";
        cin >> newRating;
        cout << "New numPlay?: ";
        cin >> newNumPlay;
        result->ChangeData(newRating, newNumPlay);
    }
}
