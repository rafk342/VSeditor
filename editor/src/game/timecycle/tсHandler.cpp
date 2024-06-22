#include "t�Handler.h"
#include <sstream>
#include <format>


time�ycleHandler::time�ycleHandler()
{
	m_WeatherNames.reserve(15);

	static void* tcMngr = gmAddress::Scan("48 C1 E0 03 41 80 E1 01 C6 44 24 20 00")
		.GetRef(22)
		.To<void*>();

	for (size_t i = 0; i < WEATHER_TC_FILES_COUNT; i++)
	{
		m_CyclesArray[i] = (tcCycle*) ((*(u8**)((u8*)(tcMngr) + 0x68)) + (i * 0x57e0));
		m_WeatherNames.push_back(std::move(GetCycleName(i)));
	}
}

std::string time�ycleHandler::GetCycleName(int index)
{		
	static std::map<u32, std::string> NamesMap =
	{
		{rage::joaat("EXTRASUNNY")	, "EXTRASUNNY"	},
		{rage::joaat("CLEAR")		, "CLEAR"		},
		{rage::joaat("CLOUDS")		, "CLOUDS"		},
		{rage::joaat("SMOG")		, "SMOG"		},
		{rage::joaat("FOGGY")		, "FOGGY"		},
		{rage::joaat("OVERCAST")	, "OVERCAST"	},
		{rage::joaat("RAIN")		, "RAIN"		},
		{rage::joaat("THUNDER")		, "THUNDER"		},
		{rage::joaat("CLEARING")	, "CLEARING"	},
		{rage::joaat("NEUTRAL")		, "NEUTRAL"		},
		{rage::joaat("SNOW")		, "SNOW"		},
		{rage::joaat("BLIZZARD")	, "BLIZZARD"	},
		{rage::joaat("HALLOWEEN")	, "HALLOWEEN"	},
		{rage::joaat("SNOWLIGHT")	, "SNOWLIGHT"	},
		{rage::joaat("XMAS")		, "XMAS"		},
	};
	u32 n_hash = m_CyclesArray[index]->GetCycleNameHash();
	
	if (NamesMap.contains(n_hash)) {
		return NamesMap.at(n_hash);
	} else {
		return std::format("Unknown tcCycle name /hash : 0x{:08X}", n_hash);
	}
}

