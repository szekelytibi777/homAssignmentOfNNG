#include "StreetSection.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
using namespace std;
int lineCount = 0;
void StreetSection::CreateSectionFromDBLine(const string& line, StreetSection& leftSection, StreetSection& rightSection) {
	vector<std::string> tokens;
	tokens.reserve(34);
	string token;
	istringstream row(line);
	while (getline(row, token, ','))
	{
		token.erase(remove(token.begin(), token.end(), '\"'), token.end());
		tokens.push_back(token);
	}
	string name = tokens[16] + " " + tokens[17];
	if (name != " ") {
		string schemeLeft = tokens[20];
		string schemeRight = tokens[23];
		if (!schemeLeft.empty()) {
			leftSection.name = name;
			leftSection.scheme = schemeLeft[0];
			leftSection.from = atoi(tokens[21].c_str());
			leftSection.to = atoi(tokens[22].c_str());
		}
		if (!schemeRight.empty()) {
			leftSection.name = name;
			leftSection.scheme = schemeRight[0];
			leftSection.from = atoi(tokens[24].c_str());
			leftSection.to = atoi(tokens[25].c_str());
		}
	}

	lineCount++;
}