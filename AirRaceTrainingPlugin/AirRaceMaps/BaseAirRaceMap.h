#pragma once
#include "CarPosition.h"
#include <vector>
#include <memory>

class BaseAirRaceMap 
{
public:
	BaseAirRaceMap();
	BaseAirRaceMap(std::string name, std::string title, CarPosition firstRing, std::vector<CarPosition> spawns, std::vector<Checkpoint> checkpoints);

	std::string mapName;
	std::string mapTitle;
	CarPosition firstRingTrigger;
	std::vector<CarPosition> spawnLocations;
	std::vector<Checkpoint> checkpointLocations;

	std::shared_ptr<int> selectedSpawn;
	std::shared_ptr<int> selectedCheckpoint;
};