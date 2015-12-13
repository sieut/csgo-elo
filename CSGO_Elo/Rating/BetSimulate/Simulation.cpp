#include "Hashtable.h"
#include "Team.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

double K1 = 25.9907;
double K2 = 0.06746;

void UpdateRatingHelper(HashTable& table, const Match& match, bool autoAdd);
void CalCulateExpectedScore(const Team* tA, const Team* tB, double& expectedA, double& expectedB);
double KellyBetSize(double expected, double odd, int format);

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

	inSS >> a; inSS >> b;
	oddA = static_cast<double>(a)/(a+b);

	inSS >> format;
	getline(inSS, date);
	date += " ";		// A bug in data file, trailing space
}

double Bet::PerformBet(const HashTable& table, ostream& fileStream) {
	Team* tAptr = table.Search(teamA); //use method Search of HashTable class
    Team* tBptr = table.Search(teamB);

    double expectedA, expectedB;
    CalCulateExpectedScore(tAptr, tBptr, expectedA, expectedB); //calculate expected score

    fileStream << date << "," << teamA << " - " << teamB;

    double result = 0.0;
    if (KellyBetSize(expectedA, oddA, format) > 0.01) {
    	fileStream << "," << teamA;
    	switch (winner) {
    		case 'a':
    			result = KellyBetSize(expectedA, oddA, format) * ( (1-oddA)/oddA ) * 10;
    		case 'b':
    			result = -KellyBetSize(expectedA, oddA, format) * 10;
    	}
    }
    else if (KellyBetSize(expectedB, 1-oddA, format) > 0.01) {
    	fileStream << "," << teamB;
    	switch (winner) {
    		case 'a':
    			result = -KellyBetSize(expectedB, 1-oddA, format) * 10;
    		case 'b':
    			result = KellyBetSize(expectedB, 1-oddA, format) * ( oddA/(1-oddA) ) * 10;
    	}
    }

    fileStream << "," << result << endl;
    return result;
}

double KellyBetSize(double expected, double odd, int format) {
	switch (format) {
		case 1:
			expected = expected;
		case 2:			// Not sure about the formula for BO2s
			expected = expected;
		case 3:
			expected = expected * expected + 2 * expected * expected * (1 - expected);
		case 5:
			expected = expected * expected * expected + 3 * expected * expected * expected * (1 - expected)
						+ 6 * expected * expected * expected * (1 - expected) * (1 - expected);
	}
	return ( expected * ( (1-odd)/odd + 1 ) - 1 ) / ( 3 * (1-odd)/odd );
}

int main() {
	HashTable table;

	//KEEP all roster data from roster file to "rosterData"
    ifstream infile;
    string line;
    vector<RosterInfo> rosterData; //keep roster data
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
            return 0;
        }

        getline(infile, line);
        while (infile.good())
        {
            rosterData.push_back(RosterInfo(line));
            getline(infile, line);
        }
        infile.close();
    }

    // Open bet file, get bet data
    vector<Bet> betData; //keep bet data
    string betFileName;
    cout << "Bet file (no .txt): ";
    cin >> betFileName;
    betFileName += ".txt";
    infile.open(betFileName.c_str());

    if (!infile.is_open())
    {
        cerr << "Cannot open bet file." << endl;
        return 0;
    }

    getline(infile, line);
    while (infile.good())
    {
        betData.push_back(Bet(line));
        getline(infile, line);
    }
    infile.close();


    //Open match file
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

    ofstream outFile("track_bet");

    //RUN through matches
    int rosterInfoIdx = 0;
    int betIdx = 0;
    double profit = 0.0;
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