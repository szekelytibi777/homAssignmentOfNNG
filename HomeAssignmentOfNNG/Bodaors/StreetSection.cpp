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
			int hs1 = atoi(tokens[21].c_str());
			int hs2 = atoi(tokens[22].c_str());
			leftSection.from = hs1 <= hs2 ? hs1 : hs2;
			leftSection.to = hs2 >= hs1 ? hs2 : hs1;
		}
		if (!schemeRight.empty()) {
			rightSection.name = name;
			rightSection.scheme = schemeRight[0];
			 
			int hs1 = atoi(tokens[24].c_str());
			int hs2 = atoi(tokens[25].c_str());
			rightSection.from = hs1 <= hs2 ? hs1 : hs2;
			rightSection.to = hs2 >= hs1 ? hs2 : hs1;
		}
	}
	lineCount++;
}