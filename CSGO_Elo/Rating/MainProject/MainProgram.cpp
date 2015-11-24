#include "classes.h"
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

void CreateTeamVector(vector<Team>& teamData);
void CreateMatchVector(vector<Match>& matchData);
void CalCulateExpectedScore(const Team& tA, const Team& tB, double& expectedA, double& expectedB);
double UpdateRating(const Match& match, vector<Team>& teamData, double k1, double k2);
//implementation in another file

int main()
{
	vector<Team> teamData;
	CreateTeamVector(teamData);
	vector<Match> matchData;
	CreateMatchVector(matchData);

	double k1 = 28.7;
    double k2 = 0.0219;

    double loseProb = 0.0;

    ofstream outFile("TestResult.txt");
    for (k1 = 20; k1 <= 40; k1 += 4/7.0)
    {
        for (k2 = 0.02; k2 <= 0.08; k2 += 3/1750.0)
        {
            loseProb = 0.0;
            for (int i = 0; i < matchData.size(); i++)
            {
                loseProb += UpdateRating(matchData.at(i), teamData, k1, k2);
            }

            loseProb = loseProb / matchData.size();
            outFile << fixed << setprecision(7);
            outFile << k1 << " " << k2 << " " << loseProb << endl;

            //Reset Team'stats
            for (int i = 0; i < teamData.size(); i++)
            {
                teamData.at(i).Reset();
            }
        }
    }



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

void CalCulateExpectedScore(const Team& tA, const Team& tB, double& expectedA, double& expectedB)
{
	expectedA = 1 / ( 1 + pow(10, (tB.Rating() - tA.Rating()) / 400) );
	expectedB = 1 - expectedA;
}

//Return added lose probability
double UpdateRating(const Match& match, vector<Team>& teamData, double k1, double k2)
{
    //teamA = teamData.at(match.WinTeam), teamB = teamData.at(match.LostTeam)
    double expectedA, expectedB;
    CalCulateExpectedScore(teamData.at(match.WinTeam()), teamData.at(match.LoseTeam()), expectedA, expectedB);

    if (match.isTie())
    {
        //k1 weight is built in AddRating
        teamData.at(match.WinTeam()).AddRating(k1 * (1 - 2 * expectedA));
        teamData.at(match.LoseTeam()).AddRating(k2 * (1 - 2 * expectedB));
        return pow((expectedA - 0.5), 2);
    }
    else
    {
        teamData.at(match.WinTeam()).AddRating(k1 * (1 + match.WinScore() + k2 - 2 * expectedA));
        teamData.at(match.LoseTeam()).AddRating(k1 * (1 - (match.WinScore() + k2) - 2 * expectedB));
        return pow((expectedA - 1.0), 2);
    }

}
