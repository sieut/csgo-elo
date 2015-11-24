#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
using namespace std;

class Team {
private:
	string name;
public:
	Team();
	Team(const string& nameInput);
	const string& Name() const { return name; }
	bool operator>(const Team& t) const { return name > t.name; }
	bool operator<(const Team& t) const { return name < t.name; }
	bool operator==(const Team& t) const { return name == t.name; }
	friend ostream& operator<<(ostream& os, const Team& t);
};

Team::Team() 
{
	name = "--null--";
}

Team::Team(const string& nameInput)
{
	name = nameInput;
}

ostream& operator<<(ostream& os, const Team& t)
{
	os << t.name;
	return os;
}

void ExtractTeamVector(const string& input, vector<Team>& teamData);
void PrintTeamVectorToFile(ofstream& outfile, const vector<Team>& teamData);
bool AlreadyHave(const string& newTeamName, const vector<Team>& teamData);

void ExtractTeamVector(const string& input, vector<Team>& teamData)
{
	istringstream inSS;
	string teamAName, teamBName;

	inSS.clear();
	inSS.str(input);
	
	inSS >> teamAName;
	inSS >> teamBName;
	inSS.clear();

	if (!AlreadyHave(teamAName, teamData))
	{
		teamData.push_back(Team(teamAName));
		sort(teamData.begin(), teamData.end());
	}

	if (!AlreadyHave(teamBName, teamData))
	{
		teamData.push_back(Team(teamBName));
		sort(teamData.begin(), teamData.end());
	}
}

//use binary search to find if we already have that team.
bool AlreadyHave(const string& newTeamName, const vector<Team>& teamData)
{
	int first = 0, last = teamData.size() - 1;
	int mid;

	while (first <= last)
	{
		mid = (first + last) / 2;
		if (newTeamName > teamData.at(mid).Name())
			first = mid + 1;
		else if (newTeamName < teamData.at(mid).Name())
			last = mid - 1;
		else
			return true;
	}
	return false;
}

void PrintTeamVectorToFile(ofstream& outfile, const vector<Team>& teamData)
{
	int numTeam = teamData.size();
	for (int i = 0; i < numTeam; i++)
	{
		outfile << teamData.at(i) << endl;
	}
	
	//outfile << "There are " << numTeam << " teams" << endl;
}

int main()
{
	vector<Team> teamData;
	string inFileName;
	cin >> inFileName;

	ifstream infile(inFileName.c_str());
	if (!infile.is_open())
	{
		cerr << "Cannot open file." << endl;
		return 1;
	}

	string line;
	getline(infile, line);
	while (infile.good())
	{
		ExtractTeamVector(line, teamData);
		getline(infile, line);
	}
	
	if (!infile.eof())
	{
		cerr << "Cannot read til the end." << endl;
		return 1;
	}

	ofstream outfile("teamList.txt");
	PrintTeamVectorToFile(outfile, teamData);


	return 0;
}
