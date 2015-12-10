#include "Team.h"
#include "Hashtable.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <cmath>
#include <ios>
#include <iomanip>
using namespace std;

double K1 = 25.9907;
double K2 = 0.06746;

// HashTable prototype
void InputData(HashTable& table, string& lastMatch);
void PrintTable(const HashTable& table, const string& lastMatch);
// MatchUpdate prototype (Match+Roster)
void UpdateRating(HashTable& table, string& lastMatch);
void ComputeResult(HashTable& table);
// TeamUpdate prototype
void UpdateTeam (HashTable& table);

int main()
{
    HashTable table;
    string lastMatch;
    cout << "Load team data:" << endl;
    InputData(table, lastMatch);

    cout << "What You Can Do (case-sensitive): " << endl
         << "1) update \"rating\"   "
         << "2) compute \"result\"   " << endl
         << "3) manage \"team\"   "
         << "4) \"print\" updated data   "
         << "5) \"exit\"   " << endl;

    string command;
    cin >> command;
    while (command != "exit")
    {
        if (command == "rating") UpdateRating(table, lastMatch);
        else if (command == "result") ComputeResult(table);
        else if (command == "team") UpdateTeam(table);
        else if (command == "print") PrintTable(table, lastMatch);
        else cout << "type again!" << endl;

        cout << "What You Can Do (case-sensitive): " << endl
             << "1) update \"rating\"   "
             << "2) compute \"result\"   " << endl
             << "3) manage \"team\"   "
             << "4) \"print\" updated data   "
             << "5) \"exit\"   " << endl;
        cin >> command;
    }
    cout << "DONE!" << endl;

	return 0;
}
