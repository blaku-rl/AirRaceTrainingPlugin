#include "pch.h"
#include "AirRaceTrainingPlugin.h"
#include "AirRaceMaps/BuildAirRaceMapInfo.h"
#include <bakkesmod/wrappers/kismet/SequenceWrapper.h>
#include <bakkesmod/wrappers/kismet/SequenceVariableWrapper.h>
#include <bakkesmod/wrappers/kismet/SequenceOpWrapper.h>
#include <bakkesmod/wrappers/kismet/SequenceObjectWrapper.h>

BAKKESMOD_PLUGIN(AirRaceTrainingPlugin, "A plugin designed for helping you train Air Race maps", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void AirRaceTrainingPlugin::onLoad()
{
	_globalCvarManager = cvarManager;
	isFirstSpawn = true;
	srand(time(NULL));

	//Cvars
	isEnabled = std::make_shared<bool>(true);
	cvarManager->registerCvar("airracetraining_enabled", "1", "True/False value for if the plugin should be enabled", true, true, 0, true, 1, true)
		.bindTo(isEnabled);
	chatboxEnabled = std::make_shared<bool>(true);
	cvarManager->registerCvar("airracetraining_chatbox_messages", "1", "True/False value for if messages should go to the chatbox", true, true, 0, true, 1, true)
		.bindTo(chatboxEnabled);
	teleportsEnabled = std::make_shared<bool>(false);
	cvarManager->registerCvar("airracetraining_teleportsenabled", "0", "True/False value for if teleporting to checkpoints should be enabled", true, true, 0, true, 1, true)
		.bindTo(teleportsEnabled);
	setSpawnEnabled = std::make_shared<bool>(false);
	cvarManager->registerCvar("airracetraining_setspawnenabled", "0", "True/False value for if the spawn platform should be set", true, true, 0, true, 1, true)
		.bindTo(setSpawnEnabled);
	randomizeSpawn = std::make_shared<bool>(false);
	cvarManager->registerCvar("airracetraining_randomizespawn", "0", "True/False value for if spawn platforms should be randomized", true, true, 0, true, 1, true)
		.bindTo(randomizeSpawn);

	//Generate cvars from air race maps
	allMapInfo = BuildAirRaceMapInfo::BuildAll();
	loadedMap = nullptr;
	for (auto& map : allMapInfo) {
		std::string mapName = map->mapName;
		int maxS = map->spawnLocations.size() - 1;
		int maxT = map->checkpointLocations.size() - 1;

		cvarManager->registerCvar("airracetraining_" + mapName + "_selectedspawn", "0", "The spawn platfrom you will be placed on in " + mapName, true, true, 0, true, maxS, true)
			.bindTo(map->selectedSpawn);
		cvarManager->registerCvar("airracetraining_" + mapName + "_selectedcheck", "0", "The checkpoint that will you will be teleported to in " + mapName, true, true, 0, true, maxT, true)
			.bindTo(map->selectedCheckpoint);
	}

	//Notifiers
	cvarManager->registerNotifier("airracetraining_movecheckdown", [this](std::vector<std::string> params) {
		this->MoveCheckDown();
		}, "Move the selected checkpoint to the previous one", PERMISSION_FREEPLAY);
	cvarManager->registerNotifier("airracetraining_movecheckup", [this](std::vector<std::string> params) {
		this->MoveCheckUp();
		}, "Move the selected checkpoint to the next one", PERMISSION_FREEPLAY);

	cvarManager->registerNotifier("airracetraining_toggle_enabled", [this](std::vector<std::string> params) {
		this->ToggleCvar(*isEnabled, "airracetraining_enabled", "Training");
		}, "Switch training on and off", PERMISSION_FREEPLAY);
	cvarManager->registerNotifier("airracetraining_toggle_chatbox", [this](std::vector<std::string> params) {
		this->ToggleCvar(*chatboxEnabled, "airracetraining_chatbox_messages", "Chatbox Messages");
		}, "Switch chatbox notifications on and off", PERMISSION_FREEPLAY);
	cvarManager->registerNotifier("airracetraining_toggle_teleporting", [this](std::vector<std::string> params) {
		this->ToggleCvar(*teleportsEnabled, "airracetraining_teleportsenabled", "Teleporting");
		}, "Switch teleporting on and off", PERMISSION_FREEPLAY);
	cvarManager->registerNotifier("airracetraining_toggle_setspawn", [this](std::vector<std::string> params) {
		this->ToggleCvar(*setSpawnEnabled, "airracetraining_setspawnenabled", "Set Spawn");
		}, "Switch setting your spawn on and off", PERMISSION_FREEPLAY);
	cvarManager->registerNotifier("airracetraining_toggle_randomizedspawn", [this](std::vector<std::string> params) {
		this->ToggleCvar(*randomizeSpawn, "airracetraining_randomizespawn", "Randomized Spawn");
		}, "Switch randomized spawn on and off", PERMISSION_FREEPLAY);

	//Permanent Hookevents
	gameWrapper->HookEvent("Function TAGame.GameEvent_Soccar_TA.Destroyed", bind(&AirRaceTrainingPlugin::OnMapUnload, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function GameEvent_Soccar_TA.Active.StartRound", std::bind(&AirRaceTrainingPlugin::OnCarSpawns, this, std::placeholders::_1));
}

void AirRaceTrainingPlugin::onUnload()
{
}

/*
* This provides a searchable way in the binding tab of bakkesmod to toggle the enabled status
* This has the added benefit of only running in the map when not paused
* It's also been easier for people not familiar with bakkesmod to get up and going
*/
void AirRaceTrainingPlugin::ToggleCvar(const bool& cvar, const std::string& cvarStr, const std::string& cvarName)
{
	if (loadedMap == nullptr || gameWrapper->IsPaused() || !gameWrapper->IsInFreeplay()) { return; }

	if (cvar) {
		cvarManager->executeCommand(cvarStr + " 0");
		Log("Air Race " + cvarName + " Disabled", true);
	}
	else {
		cvarManager->executeCommand(cvarStr + " 1");
		Log("Air Race " + cvarName + " Enabled", true);
	}
}

void AirRaceTrainingPlugin::ShowEnabledStatus()
{
	if (*isEnabled)
		Log("Air Race Training Enabled", true);
	else
		Log("Air Race Training Disabled", true);
}

bool AirRaceTrainingPlugin::ValidPluginState()
{
	return *isEnabled && gameWrapper->IsInFreeplay() && loadedMap != nullptr;
}

bool AirRaceTrainingPlugin::IsAnAirRaceMap()
{
	std::string mapName = gameWrapper->GetCurrentMap();
	for (int i = 0; i < mapName.length(); i++) {
		mapName[i] = std::tolower(mapName[i]);
	}

	//Check if the map is loaded under a different map name like Labs_Underpass_P.upk
	auto sequence = gameWrapper->GetMainSequence();
	std::string mapTitle = "";
	if (sequence.memory_address != NULL) {
		auto allVars = sequence.GetAllSequenceVariables(false);
		auto mapVar = allVars.find("MapName");
		if (mapVar != allVars.end() && mapVar->second.IsString())
			mapTitle = mapVar->second.GetString();
	}

	for (auto& map : allMapInfo)
	{
		if (map->mapName == mapName) {
			loadedMap = map;
			return true;
		}

		else if (map->mapTitle == mapTitle) {
			loadedMap = map;
			return true;
		}
	}

	loadedMap = nullptr;
	return false;
}

void AirRaceTrainingPlugin::OnMapUnload(std::string eventName)
{
	loadedMap = nullptr;
	isFirstSpawn = true;
}

void AirRaceTrainingPlugin::MoveCheckUp()
{
	if (!ValidPluginState() || gameWrapper->IsPaused()) { return; }

	auto mapCheckpoints = loadedMap->selectedCheckpoint;

	(*mapCheckpoints)++;
	if ((*mapCheckpoints) > loadedMap->checkpointLocations.size() - 1)
		*mapCheckpoints = 0;

	cvarManager->executeCommand("airracetraining_" + loadedMap->mapName + "_selectedcheck " + std::to_string(*mapCheckpoints));
	Log("#" + std::to_string((*mapCheckpoints) + 1) + " Checkpoint Selected", true);
}

void AirRaceTrainingPlugin::MoveCheckDown()
{
	if (!ValidPluginState() || gameWrapper->IsPaused()) { return; }

	auto mapCheckpoints = loadedMap->selectedCheckpoint;

	(*mapCheckpoints)--;
	if ((*mapCheckpoints) < 0)
		*mapCheckpoints = loadedMap->checkpointLocations.size() - 1;

	cvarManager->executeCommand("airracetraining_" + loadedMap->mapName + "_selectedcheck " + std::to_string(*mapCheckpoints));
	Log("#" + std::to_string((*mapCheckpoints) + 1) + " Checkpoint Selected", true);
}

void AirRaceTrainingPlugin::OnCarSpawns(std::string eventName)
{
	if (isFirstSpawn) {
		if (IsAnAirRaceMap()) {
			gameWrapper->SetTimeout([this](GameWrapper* gw) {
				this->ShowEnabledStatus();
				}, 6.0f);
			//Might make this float dependent on each map or a cvar people can change
		}
		isFirstSpawn = false;
		return;
	}

	if (!ValidPluginState()) { return; }

	if (*teleportsEnabled)
		MoveToSelectedCheckpoint();
	else if (*randomizeSpawn)
		RandomizeSpawn();
	else if (*setSpawnEnabled)
		SetSpawn(loadedMap->spawnLocations[*(loadedMap->selectedSpawn)]);
}

void AirRaceTrainingPlugin::MoveToSelectedCheckpoint()
{
	auto car = gameWrapper->GetLocalCar();
	if (car.IsNull()) { return; }

	car.SetLocation(loadedMap->firstRingTrigger.Location);
	car.SetRotation(loadedMap->firstRingTrigger.Rotation);

	gameWrapper->SetTimeout([this](GameWrapper* gw) {
		auto sequence = gameWrapper->GetMainSequence();
		if (sequence.memory_address == NULL) return;

		auto allVars = sequence.GetAllSequenceVariables(false);
		auto car = gameWrapper->GetLocalCar();
		if (car.IsNull()) { return; }

		auto& curCheckpoint = loadedMap->checkpointLocations[*(loadedMap->selectedCheckpoint)];
		allVars.find("Player1Count")->second.SetInt(curCheckpoint.RingCount);
		allVars.find("Player1CPCount")->second.SetInt(curCheckpoint.CPCount);

		gameWrapper->GetLocalCar().SetLocation(curCheckpoint.SpawnLocation.Location);
		gameWrapper->GetLocalCar().SetRotation(curCheckpoint.SpawnLocation.Rotation);
		}, .05f);
}

void AirRaceTrainingPlugin::RandomizeSpawn()
{
	int pos = rand() % loadedMap->spawnLocations.size();

	SetSpawn(loadedMap->spawnLocations[pos]);
}

void AirRaceTrainingPlugin::SetSpawn(const CarPosition& spawnPos)
{
	auto car = gameWrapper->GetLocalCar();
	if (car.IsNull()) { return; }

	car.SetLocation(spawnPos.Location);
	car.SetRotation(spawnPos.Rotation);
}

void AirRaceTrainingPlugin::Log(std::string message, bool sendToChat)
{
	cvarManager->log(message);
	if (sendToChat && loadedMap != nullptr && *chatboxEnabled) {
		gameWrapper->LogToChatbox(message, "Air Race");
	}
}