#pragma once
#include <string>
class StreetSection
{
public:
	std::string name;
	int from;
	int to;
	char scheme;

	StreetSection(const std::string &_name="", char _scheme=' ', int _from=0, int _to=0)
		: name(_name)
		, scheme(_scheme)
		,from(_from)
		,to(_to)
	{}

	static void  CreateSectionFromDBLine(const std::string& line, StreetSection& leftSection, StreetSection &rightSection);
};

