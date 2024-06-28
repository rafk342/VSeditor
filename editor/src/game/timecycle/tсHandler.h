#pragma once

#include <map>
#include <iostream>
#include <string>
#include <vector>
#include <array>

#include "tccycle.h"
#include "xml/tcXmlParser.h"
#include "crypto/joaat.h"

class time�ycleHandler
{
	using CyclesArray_t = std::array<tcCycle*, WEATHER_TC_FILES_COUNT>;
	using W_NamesArray_t = std::vector<std::string>;

	CyclesArray_t	m_CyclesArray = {{nullptr}};
	W_NamesArray_t	m_WeatherNames;
public:
	tcXmlParser		m_XmlParser;

	time�ycleHandler();
	std::string	GetCycleName(int index);
	const auto&	GetWeatherNamesVec() { return this->m_WeatherNames; }
	tcCycle* GetCycle(int index)	 { return m_CyclesArray[index]; }
};



