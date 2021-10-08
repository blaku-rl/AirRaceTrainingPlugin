#include "BaseAirRaceMap.h"
#include <memory>

BaseAirRaceMap::BaseAirRaceMap()
{
}

BaseAirRaceMap::BaseAirRaceMap(std::string name, std::string title, CarPosition firstRing, std::vector<CarPosition> spawns, std::vector<Checkpoint> checkpoints)
{
	mapName = name;
	mapTitle = title;
	firstRingTrigger = firstRing;
	spawnLocations = spawns;
	checkpointLocations = checkpoints;

	selectedSpawn = std::make_shared<int>(0);
	selectedCheckpoint = std::make_shared<int>(0);
}