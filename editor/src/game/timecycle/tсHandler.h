#pragma once

#include <map>
#include <iostream>
#include <string>
#include <vector>
#include <array>

#include "tcdef.h"
#include "tccycle.h"
#include "xml/tcXmlParser.h"
#include "tcdef.h"
#include "crypto/joaat.h"

class time�ycleHandler
{
	std::array<tcCycle*, WEATHER_TC_FILES_COUNT> cyclesArray = {{nullptr}};
	std::vector<std::string> weather_names;

public:
	tcXmlParser			xmlParser;

						time�ycleHandler();
	const std::string	GetCycleName(int index);
	const auto&			GetWeatherNamesVec()	{ return this->weather_names; }
	tcCycle*			GetCycle(int index)		{ return cyclesArray[index]; }
};



