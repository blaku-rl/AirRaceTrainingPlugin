#pragma once
#pragma comment( lib, "bakkesmod.lib" )
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include <vector>
#include "CarPosition.h"

class AirRaceTrainingPlugin : public BakkesMod::Plugin::BakkesModPlugin {
private:
	bool isEnabled = false;
	bool isMoving = false;
	int counter = 0;
	std::shared_ptr<int> selectedCheckpoint;
	std::vector<CarPosition> currentMovingLocations;
	std::vector<CarPosition> SpawnToFirstCheckLocations;
	std::vector<CarPosition> SpawnToSecondCheckLocations;
	std::vector<CarPosition> SpawnToThirdCheckLocations;
	std::vector<CarPosition> SpawnToFourthCheckLocations;
	std::vector<CarPosition> SpawnToFifthCheckLocations;
	std::vector<CarPosition> SpawnToSixthCheckLocations;
	std::vector<CarPosition> SpawnToSeventhCheckLocations;
	std::vector<CarPosition> SpawnToEighthCheckLocations;
	std::vector<CarPosition> SpawnToNinthCheckLocations;
	std::vector<CarPosition> SpawnToTenthCheckLocations;
	std::vector<CarPosition> SpawnToLastCheckLocations;
public:
	void onLoad();
	void onUnload();
	void toggleEnabled(std::string oldValue, CVarWrapper newValue);
	void showEnabledStatus();
	bool validPluginState();
	bool isPanicsAirRace();
	void loadSpawnToCheckLocations();
	std::vector<CarPosition> combineLocations(std::vector<CarPosition> firstLocations, std::vector<CarPosition> secondLocations);
	void setupCarMove(std::vector<CarPosition> positions);
	void moveCarAlongPositions();
	void onMapLoad(std::string eventName);
	void onMapUnload(std::string eventName);
	void onInitialCarSpawn(std::string eventName);
	void moveCheckUp();
	void moveCheckDown();
	void moveToSelectedCheckpoint(std::string eventName);
	void log(std::string message, bool sendToChat);
};