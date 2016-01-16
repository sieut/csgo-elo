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
void UpdateRatingHelper(HashTable& table, const Match& match, bool autoAdd);
void CalCulateExpectedScore(const Team* tA, const Team* tB, double& expectedA, double& expectedB);


class Bet {
public:
	Bet();
	Bet(const string&);
	string Date() const { return date; }
	double PerformBet(const HashTable& table, ostream& fileStream);
private:
	string teamA;
	string teamB;
	char winner;
	double oddA;
	int format;
	string date;
};

Bet::Bet(const string& input) {
	istringstream inSS(input);
	int a,b;

	inSS >> teamA;
	inSS >> teamB;
	inSS >> winner;

	inSS >> a;
	inSS >> b;
	oddA = static_cast<double>(a)/(a+b);

	inSS >> format;
	getline(inSS, date);
}

double Bet::PerformBet(const HashTable& table, ostream& fileStream) {
	Team* tAptr = table.Search(teamA); //use method Search of HashTable class
    Team* tBptr = table.Search(teamB);

   	double result = 0.0;

    if (tAptr != NULL && tBptr != NULL) {
	    double expectedA, expectedB;
	    CalCulateExpectedScore(tAptr, tBptr, expectedA, expectedB); //calculate expected score

	    fileStream << date << "," << teamA << " - " << teamB << ",BO" << format << "," 
	    	<< expectedA << " - " << expectedB << "," << oddA << " - " << 1-oddA << ",";

	    if (tAptr->NumPlay() >= 100 && tBptr->NumPlay() >= 100) {
		    if (KellyBetSize(expectedA, oddA, format) > 0.01) {
		    	fileStream << teamA;
		    	switch (winner) {
		    		case 'a':
		    			result = KellyBetSize(expectedA, oddA, format) * ( (1-oddA)/oddA ) * 10;
		    		case 'b':
		    			result = -KellyBetSize(expectedA, oddA, format) * 10;
		    	}
		    }
		    else if (KellyBetSize(expectedB, 1-oddA, format) > 0.01) {
		    	fileStream << teamB;
		    	switch (winner) {
		    		case 'a':
		    			result = -KellyBetSize(expectedB, 1-oddA, format) * 10;
		    		case 'b':
		    			result = KellyBetSize(expectedB, 1-oddA, format) * ( oddA/(1-oddA) ) * 10;
		    	}
		    }
		}

		fileStream << "," << result << endl;
	}
    return result;
}

//Additional functions (put here because one of them has to be after class Bet)
void InputRosterData(vector<RosterInfo>& rosterData); 
void InputBetData(vector<Bet>& betData);
bool SetAutoAdd();

double KellyBetSize(double expected, double odd, int format) {
	switch (format) {
		case 1:
			expected = expected;
		case 2:			// Not sure about the formula for BO2s
			return 0;
		case 3:
			expected = expected * expected + 2.0 * expected * expected * (1.0 - expected);
		case 5:
			expected = expected * expected * expected + 3.0 * expected * expected * expected * (1.0 - expected)
						+ 6.0 * expected * expected * expected * (1.0 - expected) * (1.0 - expected);
	}
	return ( expected * ( (1.0-odd)/odd + 1.0 ) - 1.0 ) / ( 3.0 * (1.0-odd)/odd );
}

int main() {
	HashTable table;

	//KEEP all roster data from roster file to "rosterData"
    vector<RosterInfo> rosterData;
    InputRosterData(rosterData);

    // Open bet file, get bet data
    vector<Bet> betData; //keep bet data
    InputBetData(betData);

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
    bool autoAdd = SetAutoAdd();

    ofstream outFile("track_bet.txt");

    //RUN through matches
    int rosterInfoIdx = 0;
    int betIdx = 0;
    double profit = 0.0;
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
        // Perform bet
        while (betIdx < betData.size() && betData.at(betIdx).Date() == matchtemp.Date()) {
        	profit += betData.at(betIdx).PerformBet(table, outFile);
        	betIdx++;
        }
        UpdateRatingHelper(table, matchtemp, autoAdd);  //UpdateRating!
        getline(infile, line);
    }
    outFile << profit;
    infile.close();
    outFile.close();

	return 0;
}

void InputBetData(vector<Bet>& betData)
{
    ifstream infile;
    string betFileName;
    cout << "Bet file (no .txt): ";
    cin >> betFileName;
    betFileName += ".txt";
    infile.open(betFileName.c_str());

    if (!infile.is_open())
    {
        cerr << "Cannot open bet file." << endl;
        return;
    }

    string line;
    getline(infile, line);
    while (infile.good())
    {
        betData.push_back(Bet(line));
        getline(infile, line);
    }
    infile.close();
}