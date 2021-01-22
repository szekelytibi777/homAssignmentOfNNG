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
    int overlapCount;
    Range(const StreetSection &ss = StreetSection())
    {
        scheme = ss.scheme;
        from = ss.from;
        to = ss.to;
        overlapCount = 0;
    }
}Range;

bool operator==(const Range& a, const Range& b)//used by std::find(Ranges, Range)
{
    return a.scheme == b.scheme && a.from == b.from && a.to == b.to;
}
//typedef pair<int, int> Range;
typedef vector< Range > Ranges;
typedef map<string, Ranges> SectionMap;
SectionMap sectionsMap;
SectionMap overlappedSections;

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
    if (range.from < range2.from && range.to < range2.from)
        return false; // make sure the sections do not meet. BELLOW
    if (range.from > range2.to && range.to > range2.to)
        return false; // make sure the sections do not meet. ABOVE

    if (range.from >= range2.from && range.to <= range2.to) {
        overlap.from = range.from;
        overlap.to = range.to;
    }
    else //if(range.from < range2.from)
    {
        overlap.from = range2.from;
        overlap.to = range.to;
    }
    overlap.scheme = (range.scheme != range2.scheme) ? 'M' : range.scheme;
    return true;
}

void addOverlappedSection(const string& streetName, Range& section) {
    Ranges& overlappedRanges = overlappedSections[streetName];
    if (find(overlappedRanges.begin(), overlappedRanges.end(), section) == overlappedRanges.end()) {
        overlappedRanges.push_back(section);
    }
}

int processSections() {
    int count = 0;
    SectionMap::iterator it;
    for (it = sectionsMap.begin(); it != sectionsMap.end(); it++) {
        Ranges& ranges = it->second;
        Ranges::iterator jt;
        for (jt = ranges.begin(); jt != ranges.end(); jt++) {
            Range &r1 = *jt;
            Ranges::iterator kt;
            for (kt = jt+1; kt != ranges.end(); kt++) {
                Range& r2 = *kt;
                Range overlap;

                if (isOverlapped(r1, r2, overlap)) {
                    addOverlappedSection(it->first, overlap);
                }
            }
        }
    }
    return count;
}

void printOverlappedSections() {
    for (SectionMap::iterator it = overlappedSections.begin(); it != overlappedSections.end(); it++) {
        Ranges& sections = it->second;
        for (Range& s : sections) {
            cout << it->first << "  " << s.scheme << "  " << s.from << " " << s.to << endl;
        }
    }
}

int main()
{
    SetConsoleOutputCP(28592);
    std::cout << "HomeAssignment solution of Székely Tibi!\n";

    readSectionsFromFile("network.mid");
    buildSectionMap();
    processSections();
    printOverlappedSections();
    cout << "overlapped / all sections: " << overlappedSections.size() << " / " << sections.size();
}

