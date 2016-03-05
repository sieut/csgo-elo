#include "Team.h"
#include "HashTable.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <ios>
#include <iomanip>

using namespace std;

// MatchUpdate prototype //no roster
int UpdateRating(HashTable& table, const vector<Match> &matchData);
void InputTeamList(HashTable& table);
void CreateMatchVector(vector<Match>& matchData);

double K1;
double K2;

int main()
{
    HashTable *table = new HashTable();
    InputTeamList(*table);
    vector<Match> matchData;
    CreateMatchVector(matchData);

    cout << "tmin of Match: ";
    cin >> Match::tmin;
    cout << "tmax of Match: ";
    cin >> Match::tmax;
    
    double k1Start, k1Stop;
    double k2Start, k2Stop;
    int k1Step, k2Step;
    string outputFileName;

    cout << "k1-" << endl;
    cout << "start: ";
    cin >> k1Start;
    cout << "stop: ";
    cin >> k1Stop;
    cout << "step: ";
    cin >> k1Step;
    cout << "k2-" << endl;
    cout << "start: ";
    cin >> k2Start;
    cout << "stop: ";
    cin >> k2Stop;
    cout << "step: ";
    cin >> k2Step;

    cout << "Name of output file (no .txt): ";
    cin >> outputFileName;
    outputFileName += ".txt";

    double k1EachStep = (k1Stop - k1Start) / k1Step;
    double k2EachStep = (k2Stop - k2Start) / k2Step;

    double loseProb = 0.0;
    double lowestLoseProb = 100.0;
    double keepK1, keepK2;

    ofstream outFile(outputFileName.c_str());
    for (K1 = k1Start; K1 <= k1Stop; K1 += k1EachStep)
    {
        for (K2 = k2Start; K2 <= k2Stop; K2 += k2EachStep)
        {
            loseProb = UpdateRating(*table, matchData) / matchData.size();

            if (loseProb < lowestLoseProb)
            {
                lowestLoseProb = loseProb;
                keepK1 = K1;
                keepK2 = K2;
            }

            outFile << fixed << setprecision(7);
            outFile << K1 << " " << K2 << " " << loseProb << endl;

            //Reset Team'stats
            table->~HashTable();
            table = new HashTable();
            InputTeamList(*table);
        }
    }

    cout << "Lowest L: " << lowestLoseProb << endl;
    cout << "at k1: " << keepK1 << " at k2: " << keepK2 << endl;

	return 0;
}

void InputTeamList(HashTable& table)
{
    //Open file
    string inputFileName = "data_week";
    //cout << "Input data file (no .txt): ";
    //cin >> inputFileName;
    inputFileName += ".txt";

    ifstream infile(inputFileName.c_str());

    if (!infile.is_open())
    {
    	cerr << inputFileName << " cannot be opened" << endl;
    	return;
    }

    //Run through file
    string temp;
    getline(infile, temp);
    while (infile.good())
    {
        table.Insert(temp); //insert using insert method
        getline(infile, temp);
    }
    infile.close();

    //cout << "Input data successfully" << endl << endl;
}

void CreateMatchVector(vector<Match>& matchData)
{
    string inFileName = "data_week.txt";
    //cout << "Print \"matchIndex.txt\": ";
    //cin >> inFileName;

    ifstream infile(inFileName.c_str());
    if (!infile.is_open())
    {
        cerr << "Cannot open match file." << endl;
        return;
    }
    string line;
    getline(infile, line);
    while (infile.good())
    {
        matchData.push_back(Match(line));
        getline(infile, line);
    }

    if (!infile.eof())
    {
        cerr << "Cannot read til the end (match)." << endl;
        return;
    }
}