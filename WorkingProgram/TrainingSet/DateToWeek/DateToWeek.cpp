#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

int DateToWeek(const string &day, const string &month, const string &year);
int DateToDay(const string &day, const string &month, const string &year);

int main()
{
	string inFileName = "data_updated.txt";
	string outFileName = "data_week.txt";

	// open files
	ifstream infile(inFileName.c_str());
	if (!infile.is_open())
	{
		cerr << "Cannot open match file." << endl;
		return -1;
	}
	ofstream outfile(outFileName.c_str());

	// extract date using istringstream
	istringstream inSS;
	string teamAName, teamBName;
	int teamAScore, teamBScore;
	string day, month, year;

	// loop through matches
	string line;
	getline(infile, line);
	while (infile.good())
	{
		inSS.clear();
		inSS.str(line);

		inSS >> teamAName;
		inSS >> teamBName;
		inSS >> teamAScore;
		inSS >> teamBScore;
		inSS >> day;
		inSS >> month;
		inSS >> year;

		outfile << teamAName << " " << teamBName << " "
				<< teamAScore << " " << teamBScore << " "
				<< DateToWeek(day,month,year) << endl;

		getline(infile, line);
	}

	if (!infile.eof())
	{
		cerr << "Cannot read til the end (match)." << endl;
		return -1;
	}

	infile.close();
	outfile.close();

	return 0;
}

int DateToWeek(const string &day, const string &month, const string &year)
{
	return DateToDay(day, month, year) / 7;
}

int DateToDay(const string &day, const string &month, const string &year)
{
	int result = 0;
	// day section
	if (day.size() == 3)
	{
		result += day.at(0) - '0';
	}
	else if (day.size() == 4)
	{
		result += day.at(1) - '0';
		result += 10 * (day.at(0) - '0');
	}
	else
	{
		cerr << "Day Error" << endl;
		return -1;
	}

	// month
	int monthNum;
	if (month.substr(0,3) == "Dec") monthNum = 12;
	else if (month.substr(0,3) == "Nov") monthNum = 11;
	else if (month.substr(0,3) == "Oct") monthNum = 10;
	else if (month.substr(0,3) == "Sep") monthNum = 9;
	else if (month.substr(0,3) == "Aug") monthNum = 8;
	else if (month.substr(0,3) == "Jul") monthNum = 7;
	else if (month.substr(0,3) == "Jun") monthNum = 6;
	else if (month.substr(0,3) == "May") monthNum = 5;
	else if (month.substr(0,3) == "Apr") monthNum = 4;
	else if (month.substr(0,3) == "Mar") monthNum = 3;
	else if (month.substr(0,3) == "Feb") monthNum = 2;
	else if (month.substr(0,3) == "Jan") monthNum = 1;
	else
	{
		cerr << "Month Error" << endl;
		return -1;
	}

	switch (monthNum)
	{
		case 12:
			result += 30;
		case 11:
			result += 31;
		case 10:
			result += 30;
		case 9:
			result += 31;
		case 8:
			result += 31;
		case 7:
			result += 30;
		case 6:
			result += 31;
		case 5:
			result += 30;
		case 4:
			result += 31;
		case 3:
			result += 28;
		case 2:
			result += 31;
		case 1:
			break;
	}

	// year	(assume 20**)
	result += 365 * (year.at(3) - '5');
	result += 3650 * (year.at(2) - '1');

	return result;
}