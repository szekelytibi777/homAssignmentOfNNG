// Bodaors.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include "StreetSection.h"
#include "Windows.h"
using namespace std;
vector<StreetSection> sections;
typedef pair<int, int> Range;
typedef vector< Range > Ranges;
typedef map<string, Ranges> SectionMap;
SectionMap sectionsMap;

void readSectionsFromFile(const string& pathName) {
    ifstream  dbFile(pathName);
    sections.reserve(800); // 673 sections exists in the example db
    if (dbFile.is_open()) {
        string line;
        while (getline(dbFile, line)) {
            StreetSection leftSection;
            StreetSection rightSection;
            StreetSection::CreateSectionFromDBLine(line, leftSection, rightSection);
            if(leftSection.scheme != ' ' && rightSection.name != " ")
                sections.push_back(leftSection);

            if (rightSection.scheme != ' ' && leftSection.name != " ")
                sections.push_back(rightSection);
        }
    }
}

bool isOverlapped(const Ranges& ranges, const Range& range, Range &overlap) {
    for (const Range& r : ranges) {
        if (range.first >= r.first && range.first <= r.second) {
            overlap.first = r.first;
            overlap.second = range.second;
            return true;
        }
        if (range.second >= r.first && range.second <= r.second) {
            overlap.first = r.first;
            overlap.second = range.second;
            return true;
        }
    }
    return false;
}

int processSections() {
    int count = 0;
    for (const StreetSection& section : sections) {
        Ranges& ranges = sectionsMap[section.name];
        Range range(section.from, section.to);
        Range overlap;
        if (isOverlapped( ranges, range, overlap)) {
            cout << section.name << ": " << section.scheme << " " << overlap.first << " "  << overlap.second << endl;
            count++;
        }
        ranges.push_back(range);
    }
    return count;
}

int main()
{
    SetConsoleOutputCP(28592);
    std::cout << "HomeAssignment solution of Székely Tibi!\n";

    readSectionsFromFile("network.mid");
    int numOverlapped = processSections();
    
    cout << "overlapped / all sections: " << numOverlapped << " / " << sections.size();
}

