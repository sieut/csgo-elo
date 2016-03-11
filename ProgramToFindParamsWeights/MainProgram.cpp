#include "weightClasses.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <ios>
#include <iomanip>
using namespace std;

void CreateTeamVector(vector<TeamWithNeighbor>& teamData);
void CreateMatchVector(vector<MatchWithWeight>& matchData);
void CreateRosterVector(vector<RosterInfo>& rosterData);
void CalCulateExpectedScore(const Team& tA, const Team& tB, double& expectedA, double& expectedB);
double UpdateRating(const MatchWithWeight& match, vector<TeamWithNeighbor>& teamData, double eta, double lambda);
//implementation in another file

int main()
{
	vector<TeamWithNeighbor> teamData;
	CreateTeamVector(teamData);
	vector<MatchWithWeight> matchData;
	CreateMatchVector(matchData);
    //vector<RosterInfo> rosterData;
    //CreateRosterVector(rosterData);

    cout << "Tmax: ";
    cin >> MatchWithWeight::tmax;

    cout << "Name of output file (no .txt): ";
    cin >> outputFileName;
    outputFileName += ".txt";

    ofstream outFile(outputFileName.c_str());
    for (k1 = k1Start; k1 <= k1Stop; k1 += k1EachStep)
    {
        for (k2 = k2Start; k2 <= k2Stop; k2 += k2EachStep)
        {
            loseProb = 0.0;
            //int rosterInfoIdx = 0;      // NEW: Index over roster changes vector

            for (int i = 0; i < matchData.size(); i++)
            {
                /*while (rosterData.at(rosterInfoIdx).Date() == matchData.at(i).Date()) {     // NEW: Adjust teams' numPlay due to roster change
                    teamData.at(rosterData.at(rosterInfoIdx).Index()).AdjustNumPlay();
                    rosterInfoIdx += 1;
                }*/
                loseProb += UpdateRating(matchData.at(i), teamData, k1, k2);
            }

            loseProb = loseProb / matchData.size();

            if (loseProb < lowestLoseProb)
            {
                lowestLoseProb = loseProb;
                keepK1 = k1;
                keepK2 = k2;
            }

            outFile << fixed << setprecision(7);
            outFile << k1 << " " << k2 << " " << loseProb << endl;

            //Reset Team'stats
            for (int i = 0; i < teamData.size(); i++)
            {
                teamData.at(i).Reset();
            }
        }
    }

    cout << "Lowest L: " << lowestLoseProb << endl;
    cout << "at k1: " << keepK1 << " at k2: " << keepK2 << endl;



/*
    for (int i = 0; i < matchData.size(); i++)
    {
        UpdateRating(matchData.at(i), teamData, k1, k2);
//        outFile << "Match#" << i + 1 << endl;
//        outFile << teamData.at(matchData.at(i).WinTeam()) << endl;
//        outFile << teamData.at(matchData.at(i).LoseTeam()) << endl;
//        if (matchData.at(i).LoseTeam() == 3 || matchData.at(i).WinTeam() == 3)
//        {
//            outFile << "Match#" << i + 1 << endl;
//            outFile << teamData.at(matchData.at(i).WinTeam()) << endl;
//            outFile << teamData.at(matchData.at(i).LoseTeam()) << endl;
//        }
    }
    for (int i = 0; i < teamData.size(); i++)
    {
        outFile << teamData.at(i) << endl;
    }*/

	return 0;
}

double LearningRate(int currentIter, int maxIter) {
    return pow((1 + 0.1*maxIter) / (currentIter + 0.1*maxIter), 0.602);
}

void CalCulateExpectedScore(const TeamWithNeighbor& tA, const TeamWithNeighbor& tB, 
    double& expectedA, double& expectedB)
{
	expectedA = 1 / ( 1 + exp(tB.Rating() - tA.Rating()) );
	expectedB = 1 - expectedA;
}

//Return added lose probability
double UpdateRating(const MatchWithWeight& match, vector<TeamWithNeighbor>& teamData, 
    double eta, double lambda)
{
    //teamA = teamData.at(match.WinTeam), teamB = teamData.at(match.LostTeam)
    double expectedA, expectedB;
    TeamWithNeighbor tA = teamData.at(match.WinTeam());
    TeamWithNeighbor tB = teamData.at(match.WinTeam());
    CalCulateExpectedScore(tA, tB, expectedA, expectedB);

    if (match.isTie())
    {
        //k1 weight is built in AddRating
        teamData.at(match.WinTeam()).AddRating(-eta * (match.Weight() * (expectedA - 0.5) * expectedA * (1 - expectedA) 
                + (lambda / tA.NumNeighbor()) * 
                (tA.Rating() - tA.AverageNeighbor())));
        teamData.at(match.LoseTeam()).AddRating(-eta * (match.Weight() * (expectedB - 0.5) * expectedB * (1 - expectedB) 
                + (lambda / tB.NumNeighbor()) * 
                (tB.Rating() - tB.AverageNeighbor())));
        return match.Weight() * pow((expectedA - 0.5), 2);
    }
    else
    {
        teamData.at(match.WinTeam()).AddRating(-eta * (match.Weight() * (expectedA - 1) * expectedA * (1 - expectedA) 
                + (lambda / tA.NumNeighbor()) * 
                (tA.Rating() - tA.AverageNeighbor())));
        teamData.at(match.LoseTeam()).AddRating(-eta * (match.Weight() * (expectedB - 0) * expectedB * (1 - expectedB) 
                + (lambda / tB.NumNeighbor()) * 
                (tB.Rating() - tB.AverageNeighbor())));
        return match.Weight() * pow((expectedA - 1.0), 2);
    }

}

void CreateTeamVector(vector<TeamWithNeighbor>& teamData)
{
    string inFileName = "teamList_updated.txt";
    //cout << "Print \"teamList.txt\": ";
    //cin >> inFileName;

    ifstream infile(inFileName.c_str());
    if (!infile.is_open())
    {
        cerr << "Cannot open team file." << endl;
        return;
    }
    string line;
    getline(infile, line);
    while (infile.good())
    {
        teamData.push_back(TeamWithNeighbor(line));
        getline(infile, line);
    }

    if (!infile.eof())
    {
        cerr << "Cannot read til the end (team)." << endl;
        return;
    }
}

void CreateMatchVector(vector<MatchWithWeight>& matchData)
{
    string inFileName = "matchIndex_updated.txt";
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
        matchData.push_back(MatchWithWeight(line));
        getline(infile, line);
    }

    if (!infile.eof())
    {
        cerr << "Cannot read til the end (match)." << endl;
        return;
    }
}

void CreateRosterVector(vector<RosterInfo>& rosterData) {
    string inFileName = "rosterChange.txt";
    //cout << "Print \"rosterChange.txt\": ";
    //cin >> inFileName;

    ifstream infile(inFileName.c_str());
    if (!infile.is_open()) {
        cerr << "Cannot open roster file." << endl;
        return;
    }

    string line;
    getline(infile, line);
    while (infile.good()) {
        rosterData.push_back(RosterInfo(line));
        getline(infile, line);
    }

    if (!infile.eof()) {
        cerr << "Cannot read til the end." << endl;
        return;
    }
}

