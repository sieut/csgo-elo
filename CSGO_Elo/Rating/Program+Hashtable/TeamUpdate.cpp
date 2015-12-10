#include "Team.h"
#include "Hashtable.h"
#include <iostream>
#include <string>
using namespace std;

bool CheckTeam(const HashTable& table, const string& teamName, bool print);
void AddTeam(HashTable& table, const string& teamName);
void DeleteTeam(HashTable& table, const string& teamName);
void ChangeName(HashTable& table, const string& nameFrom, const string& nameTo);

void UpdateTeam (HashTable& table)
{
    cout << "What to do with TEAM (case-sensitive): " << endl
         << "1) \"check\"  "
         << "2) \"add\"  "
         << "3) \"delete\"  "
         << "4) \"changeName\"  "
         << "5) \"back\"  " << endl;

    string command, teamName, teamName2;
    cin >> command;
    while (command != "back")
    {
        if (command == "check"){
            cout << "teamName: ";
            cin >> teamName;
            CheckTeam(table, teamName, true);
        }
        else if (command == "add"){
            cout << "teamName: ";
            cin >> teamName;
            AddTeam(table, teamName);
        }
        else if (command == "delete"){
            cout << "teamName: ";
            cin >> teamName;
            DeleteTeam(table, teamName);
        }
        else if (command == "changeName"){
            cout << "from: ";
            cin >> teamName;
            cout << "to: ";
            cin >> teamName2;
            ChangeName(table, teamName, teamName2);
        }
        else
            cout << "type again!" << endl;

        cout << "What to do with TEAM (case-sensitive): " << endl
             << "1) \"check\"  "
             << "2) \"add\"  "
             << "3) \"delete\"  "
             << "4) \"changeName\"  "
             << "5) \"back\"  " << endl;
        cin >> command;
    }
    cout << "back to main" << endl;
}

bool CheckTeam(const HashTable& table, const string& teamName, bool print)
{
    Team* result = table.Search(teamName);
    if (result == NULL)
    {
        if (print)
            cout << "No this team name in the data" << endl << endl;
        return false;
    }
    else
    {
        if (print)
            cout << *result << endl << endl;
        return true;
    }
}

void AddTeam(HashTable& table, const string& teamName)
{
    if (CheckTeam(table, teamName, false))
    {
        cout << "AddTeam failed: already have " << teamName << endl << endl;
    }
    else
    {
        table.Insert(teamName, 1200.0);
        cout << "AddTeam " << teamName << " successfully" << endl << endl;
    }
}

void DeleteTeam(HashTable& table, const string& teamName)
{
    if (!CheckTeam(table, teamName, false))
    {
        cout << "DeleteTeam failed: " << teamName << "does not exist" << endl << endl;
    }
    else
    {
        table.Delete(teamName);
        cout << "DeleteTeam " << teamName << " successfully" << endl << endl;
    }
}

void ChangeName(HashTable& table, const string& nameFrom, const string& nameTo)
{
    if (!CheckTeam(table, nameFrom, false))
    {
        cout << "ChangeName failed: " << nameFrom << "does not exist" << endl << endl;
    }
    else
    {
        Team* teamptr = table.Search(nameFrom);
        table.Insert(nameTo, teamptr->Rating(), teamptr->NumPlay());
        cout << "ChangeName successfully" << endl << endl;
    }
}
