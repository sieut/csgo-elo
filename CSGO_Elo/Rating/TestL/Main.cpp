#include "Hashtable.h"
#include "Team.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

double K1 = 25.9907;
double K2 = 0.06746;

double KellyBetSize(double expected, double odd, int format);
void UpdateRatingHelper(HashTable& table, const Match& match, bool autoAdd, ostream& outfile);
void CalCulateExpectedScore(const Team* tA, const Team* tB, double& expectedA, double& expectedB);

//Additional functions (put here because one of them has to be after class Bet)
void InputRosterData(vector<RosterInfo>& rosterData); 
bool SetAutoAdd();

int main() {
	HashTable table;

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
        return 0;
    }

    //Feature: add unknown team automatically or manually?
    //autoAdd = true -- automatically, false -- manually
    bool autoAdd = true; //Can use SetAutoAdd() to be manually specified;

    //ofstream outfile("LDataOurRating.txt");
    ofstream outfile("LDataNormalElo.txt");
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
        
        getline(infile, line);
    }

    infile.close();
    outfile.close();

	return 0;
}

