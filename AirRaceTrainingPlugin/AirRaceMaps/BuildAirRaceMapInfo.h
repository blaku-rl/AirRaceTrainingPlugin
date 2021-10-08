#pragma once
#include "BaseAirRaceMap.h"
#include <vector>
#include <memory>

class BuildAirRaceMapInfo
{
public:
	static std::vector<std::shared_ptr<BaseAirRaceMap>> BuildAll();

	//All new air race maps should only need to be added into here
private:
	static std::shared_ptr<BaseAirRaceMap> BuildPanicsAirRace();
	static std::shared_ptr<BaseAirRaceMap> BuildPanicsAirRaceHard();
};