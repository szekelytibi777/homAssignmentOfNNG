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
typedef struct Range {
    char scheme;
    int from;
    int to;
    Range(const StreetSection &ss = StreetSection())
    {
        scheme = ss.scheme;
        from = ss.from;
        to = ss.to;
    }
};
//typedef pair<int, int> Range;
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

void buildSectionMap() {
    for (const StreetSection& section : sections) {
        Ranges& ranges = sectionsMap[section.name];
        Range range(section);
        ranges.push_back(range);
    }
}

bool isOverlapped(const Range& range, const Range& range2, Range &overlap) {
    if (range.scheme != range2.scheme)
        return false;
    if (!(range.from < range2.from && range.to < range2.from))
        return false; // make sure the sections do not meet. BELLOW
    if (!(range.from > range2.to && range.to > range2.to))
        return false; // make sure the sections do not meet. ABOVE

    if (range.from >= range2.from && range.to <= range2.to) {
        overlap.from = range.from;
        overlap.to = range.to;
        overlap.scheme = (range.scheme != range2.scheme) ? 'M' : range.scheme;
    }
    else //if(range.from < range2.from)
    {
        overlap.from = range2.from;
        overlap.to = range.to;
    }
    overlap.scheme = (range.scheme != range2.scheme) ? 'M' : range.scheme;
    return true;
}

int processSections() {
    int count = 0;
    SectionMap::iterator it;
    for (it = sectionsMap.begin(); it != sectionsMap.end(); it++) {
        Ranges& ranges = it->second;
        Range overlap;
        Ranges::iterator jt;
        for (jt = ranges.begin(); jt != ranges.end(); jt++) {
            const Range &r1 = *jt;
            Ranges::iterator kt;
            for (kt = jt + 1; kt != ranges.end(); kt++) {
                const Range& r2 = *kt;
                if (isOverlapped(r1, r2, overlap)) {
                    cout << it->first << "  " << overlap.scheme << "  " << overlap.from << " " << overlap.to << endl;
                    count++;
                }
            }
        }
    }
    return count;
}

int main()
{
    SetConsoleOutputCP(28592);
    std::cout << "HomeAssignment solution of Székely Tibi!\n";

    readSectionsFromFile("network.mid");
    buildSectionMap();
    int numOverlapped = processSections();
    cout << "overlapped / all sections: " << numOverlapped << " / " << sections.size();
}

