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

void CreateScoreVector(vector<double>& scoreFrequency);
void CreateTeamVector(vector<TeamWithNeighbor>& teamData);
void CreateMatchVector(vector<MatchWithWeight>& matchData);
void CreateRosterVector(vector<RosterInfo>& rosterData);
void CalCulateExpectedScore(const Team& tA, const Team& tB, double& expectedA, double& expectedB);
double UpdateRating(const MatchWithWeight& match, vector<TeamWithNeighbor>& teamData, double eta, double lambda);
double UpdateRatingRMSE(const MatchWithWeight& match, vector<TeamWithNeighbor>& teamData, double eta, double lambda);
double LearningRate(int currentIter, int maxIter);
double NeighborLoss(vector<TeamWithNeighbor>& teamData, double lambda);
void MatchAddNeighbor (vector<TeamWithNeighbor>& teamData, vector<MatchWithWeight>& matchData);
//implementation in another file

vector<double> scoreFrequency;

int main()
{
    CreateScoreVector(scoreFrequency);
	vector<TeamWithNeighbor> teamData;
	CreateTeamVector(teamData);
	vector<MatchWithWeight> matchData;
	CreateMatchVector(matchData);
    MatchAddNeighbor(teamData, matchData);
    //vector<RosterInfo> rosterData;
    //CreateRosterVector(rosterData);

    MatchWithWeight::tmin = matchData.at(0).Week();
    MatchWithWeight::tmax = matchData.at(matchData.size() - 1).Week();

    string outputFileName;
    cout << "Name of output file (no .txt): ";
    cin >> outputFileName;
    outputFileName += ".txt";
    ofstream outFile(outputFileName.c_str());

    double eta, lambda;
    int maxIter;
    double etaStart, etaStop;
    double lambdaStart, lambdaStop;
    int etaStep, lambdaStep;


    cout << "eta-" << endl;
    cout << "start: ";
    cin >> etaStart;
    cout << "stop: ";
    cin >> etaStop;
    cout << "step: ";
    cin >> etaStep;
    cout << "lambda-" << endl;
    cout << "start: ";
    cin >> lambdaStart;
    cout << "stop: ";
    cin >> lambdaStop;
    cout << "step: ";
    cin >> lambdaStep;

    // 2nd process
    double keepRmse = 1000.0;
    double rmse;
    double keepFinalEta, keepFinalLambda;

    double etaEachStep = (etaStop - etaStart) / etaStep;
    if (etaStep == 1)
        etaEachStep = 50.0;

    double lambdaEachStep = (lambdaStop - lambdaStart) / lambdaStep;
    if (lambdaStep == 1)
        lambdaEachStep = 50.0;

    double loseProb = 0.0;
    double lowestLoseProb = 100000.0;
    int keepIter = 0;
    double keepEta = 0;
    double keepLambda = 0;
    double bestEta = 0.0;
    double loseProbEta = 10000.0;

    for (lambda = lambdaStart; lambda <= lambdaStop; lambda += lambdaEachStep)
    {
        loseProbEta = 100000.0;
        bestEta = 0.0;
        for (eta = etaStart; eta <= etaStop; eta += etaEachStep)
        {
            loseProb = 0.0;
            //int rosterInfoIdx = 0;      // NEW: Index over roster changes vector
            //eta = LearningRate(iter, maxIter);

            for (int i = 0; i < 8000; i++)
            {
                loseProb += UpdateRating(matchData.at(i), teamData, eta, lambda);
            }

            loseProb += NeighborLoss(teamData, lambda);

            if (loseProb < lowestLoseProb)
            {
                lowestLoseProb = loseProb;
                keepEta = eta;
                keepLambda = lambda;
            }

            if (loseProb < loseProbEta)
            {
                bestEta = eta;
                loseProbEta = loseProb;
            }

            //outFile << fixed << setprecision(7);
            //outFile << LearningRate(keepIter, maxIter) << " " << lambda << " " << loseProb << endl;

            //Reset Team'stats
            for (int i = 0; i < teamData.size(); i++)
            {
                teamData.at(i).Reset();
            }
        }

        outFile << lambda << " " << bestEta << " " << loseProbEta << endl;
        rmse = 0.0;
        for (int i = 8000; i < matchData.size(); i++)
        {
            rmse += UpdateRatingRMSE(matchData.at(i), teamData, bestEta, lambda);
        }
        rmse = sqrt(rmse / (matchData.size() - 8000));

        if (rmse < keepRmse)
        {
            keepRmse = rmse;
            keepFinalLambda = lambda;
            keepFinalEta = bestEta;
        }

        //Reset Team'stats
            for (int i = 0; i < teamData.size(); i++)
            {
                teamData.at(i).Reset();
            }

    }

    cout << "Lowest RMSE: " << keepRmse << endl;
    cout << "at eta: " << keepFinalEta << " at lambda: " << keepFinalLambda << endl;

	return 0;
}

void MatchAddNeighbor (vector<TeamWithNeighbor>& teamData, vector<MatchWithWeight>& matchData)
{
    for (unsigned int i = 0; i < matchData.size(); i++) {
        teamData.at(matchData.at(i).WinTeam()).AddNeighbor(&teamData.at(matchData.at(i).LoseTeam()));
        teamData.at(matchData.at(i).LoseTeam()).AddNeighbor(&teamData.at(matchData.at(i).WinTeam()));
    }
    for (unsigned int i = 0; i < teamData.size(); i++) {
        teamData.at(i).SumUpMatches();
    }
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
    TeamWithNeighbor *tA = &teamData.at(match.WinTeam());
    TeamWithNeighbor *tB = &teamData.at(match.LoseTeam());
    CalCulateExpectedScore(*tA, *tB, expectedA, expectedB);

    if (match.isTie())
    {
        tA->AddRating(-eta * (match.Weight() * (expectedA - 0.5) * expectedA * (1 - expectedA) 
                + (lambda / tA->NumNeighbor()) * 
                (tA->Rating() - tA->AverageNeighbor())));
        tB->AddRating(-eta * (match.Weight() * (expectedB - 0.5) * expectedB * (1 - expectedB) 
                + (lambda / tB->NumNeighbor()) * 
                (tB->Rating() - tB->AverageNeighbor())));
        return match.Weight() * pow((expectedA - 0.5), 2);
    }
    else
    {
        double scoreWeight = 1 - pow((1 + scoreFrequency[16 - match.LoseScore()] - scoreFrequency[0])/
            (1 + scoreFrequency[14] - scoreFrequency[0]), 2.0);
        tA->AddRating(-eta * scoreWeight * (match.Weight() * (expectedA - 1) * expectedA * (1 - expectedA) 
                + (lambda / tA->NumNeighbor()) * 
                (tA->Rating() - tA->AverageNeighbor())));
        tB->AddRating(-eta * scoreWeight * (match.Weight() * (expectedB - 0) * expectedB * (1 - expectedB) 
                + (lambda / tB->NumNeighbor()) * 
                (tB->Rating() - tB->AverageNeighbor())));
        return match.Weight() * pow((expectedA - 1.0), 2);
    }
}

double UpdateRatingRMSE(const MatchWithWeight& match, vector<TeamWithNeighbor>& teamData, double eta, double lambda)
{
    //teamA = teamData.at(match.WinTeam), teamB = teamData.at(match.LostTeam)
    double expectedA, expectedB;
    TeamWithNeighbor *tA = &teamData.at(match.WinTeam());
    TeamWithNeighbor *tB = &teamData.at(match.LoseTeam());
    CalCulateExpectedScore(*tA, *tB, expectedA, expectedB);

    if (match.isTie())
    {
        tA->AddRating(-eta * (match.Weight() * (expectedA - 0.5) * expectedA * (1 - expectedA) 
                + (lambda / tA->NumNeighbor()) * 
                (tA->Rating() - tA->AverageNeighbor())));
        tB->AddRating(-eta * (match.Weight() * (expectedB - 0.5) * expectedB * (1 - expectedB) 
                + (lambda / tB->NumNeighbor()) * 
                (tB->Rating() - tB->AverageNeighbor())));
        return pow((expectedA - 0.5), 2);
    }
    else
    {
        double scoreWeight = 1 - pow((1 + scoreFrequency[16 - match.LoseScore()] - scoreFrequency[0])/
            (1 + scoreFrequency[14] - scoreFrequency[0]), 2.0);
        tA->AddRating(-eta * scoreWeight * (match.Weight() * (expectedA - 1) * expectedA * (1 - expectedA) 
                + (lambda / tA->NumNeighbor()) * 
                (tA->Rating() - tA->AverageNeighbor())));
        tB->AddRating(-eta * scoreWeight * (match.Weight() * (expectedB - 0) * expectedB * (1 - expectedB) 
                + (lambda / tB->NumNeighbor()) * 
                (tB->Rating() - tB->AverageNeighbor())));
        return pow((expectedA - 1.0), 2);
    }

}

double NeighborLoss(vector<TeamWithNeighbor>& teamData, double lambda) {
    double result = 0.0;
    for (unsigned int i = 0; i < teamData.size(); i++) {
        //cout << teamData.at(i).Rating() << " " << teamData.at(i).AverageNeighbor() << endl;

        result += pow((teamData.at(i).Rating() - teamData.at(i).AverageNeighbor()), 2);
    }
    result *= lambda;
    return result;
}

void CreateScoreVector(vector<double>& scoreFrequency) {
    string inFileName = "scoreFrequency.txt";

    ifstream infile(inFileName.c_str());
    if (!infile.is_open()) {
        cerr << "Cannot open score frequency file." << endl;
        return;
    }

    double dummy;
    infile >> dummy;
    while (infile.good()) {
        scoreFrequency.push_back(dummy);
        infile >> dummy;
    }

    if (!infile.eof()) {
        cerr << "Cannot read til the end (score frequency)." << endl;
        return;
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