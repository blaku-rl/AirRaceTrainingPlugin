#include "AirRaceTrainingPlugin.h"

BAKKESMOD_PLUGIN(AirRaceTrainingPlugin, "Air Race Training Plugin", "2.0", PLUGINTYPE_FREEPLAY)

void AirRaceTrainingPlugin::onLoad()
{
	loadSpawnToCheckLocations();
	
	cvarManager->registerCvar("airracetraining_enabled", "0", "Determines if teleporting is desired", true, true, 0, true, 1, true)
		.addOnValueChanged(bind(&AirRaceTrainingPlugin::toggleEnabled, this, std::placeholders::_1, std::placeholders::_2));
	cvarManager->registerCvar("airracetraining_chatbox_messages", "1", "Should show messages to the chatbox", true, true, 0, true, 1, true);
	cvarManager->registerNotifier("airracetraining_toggle_enabled", [this](std::vector<std::string> params) {
		this->toggleEnabledEasy();
		}, "Switch the plugin on and off", PERMISSION_FREEPLAY);

	selectedCheckpoint = std::make_shared<int>(0);
	cvarManager->registerCvar("airracetraining_panicsairrace_selectedcheck", "10", "The checkpoint that will you will be teleported to", true, true, 0, true, 10).bindTo(selectedCheckpoint);
	cvarManager->registerNotifier("airracetraining_movecheckdown", [this](std::vector<std::string> params) {
		this->moveCheckDown();
		}, "Move the selected checkpoint to the previous one", PERMISSION_FREEPLAY);
	cvarManager->registerNotifier("airracetraining_movecheckup", [this](std::vector<std::string> params) {
		this->moveCheckUp();
		}, "Move the selected checkpoint to the next one", PERMISSION_FREEPLAY);

	gameWrapper->HookEvent("Function TAGame.Mutator_Freeplay_TA.Init", bind(&AirRaceTrainingPlugin::onMapLoad, this, std::placeholders::_1));
}


void AirRaceTrainingPlugin::onUnload()
{
	
}

/*
* This provides a searchable way in the binding tab of bakkesmod to toggle the enabled status
* This has the added benefit of only running in the map when not paused
*/
void AirRaceTrainingPlugin::toggleEnabledEasy()
{
	if (isPanicsAirRace() && !gameWrapper->IsPaused())
	{
		std::string enabled = !cvarManager->getCvar("airracetraining_enabled").getBoolValue() ? "1" : "0";
		cvarManager->executeCommand("airracetraining_enabled " + enabled);
	}
}

void AirRaceTrainingPlugin::toggleEnabled(std::string oldValue, CVarWrapper newValue)
{
	isEnabled = newValue.getBoolValue();
	if (isPanicsAirRace() && !gameWrapper->IsPaused())
	{
		showEnabledStatus();
	}
}

void AirRaceTrainingPlugin::showEnabledStatus()
{
	if (isEnabled)
	{
		log("Air Race Training Enabled", true);
	}
	else
	{
		log("Air Race Training Disabled", true);
	}
}

bool AirRaceTrainingPlugin::validPluginState()
{
	return isEnabled && gameWrapper->IsInFreeplay() && isPanicsAirRace();
}

bool AirRaceTrainingPlugin::isPanicsAirRace()
{
	std::string mapName = gameWrapper->GetCurrentMap();
	std::transform(mapName.begin(), mapName.end(), mapName.begin(), std::tolower);
	return mapName == "airracetest" || mapName == "panicsairrace";
}

void AirRaceTrainingPlugin::loadSpawnToCheckLocations()
{
	SpawnToFirstCheckLocations = combineLocations(FirstCheckLocations, {});
	SpawnToSecondCheckLocations = combineLocations(SpawnToFirstCheckLocations, FirstCheckToSecondCheckLocations);
	SpawnToThirdCheckLocations = combineLocations(SpawnToSecondCheckLocations, SecondCheckToThirdCheckLocations);
	SpawnToFourthCheckLocations = combineLocations(SpawnToThirdCheckLocations, ThirdCheckToFourthCheckLocations);
	SpawnToFifthCheckLocations = combineLocations(SpawnToFourthCheckLocations, FourthCheckToFifthCheckLocations);
	SpawnToSixthCheckLocations = combineLocations(SpawnToFifthCheckLocations, FifthCheckToSixthCheckLocations);
	SpawnToSeventhCheckLocations = combineLocations(SpawnToSixthCheckLocations, SixthCheckToSeventhCheckLocations);
	SpawnToEighthCheckLocations = combineLocations(SpawnToSeventhCheckLocations, SeventhCheckToEighthCheckLocations);
	SpawnToNinthCheckLocations = combineLocations(SpawnToEighthCheckLocations, EighthCheckToNinthCheckLocations);
	SpawnToTenthCheckLocations = combineLocations(SpawnToNinthCheckLocations, NinthCheckToTenthCheckLocations);
	SpawnToLastCheckLocations = combineLocations(SpawnToTenthCheckLocations, TenthCheckToLastCheckLocations);
}

std::vector<CarPosition> AirRaceTrainingPlugin::combineLocations(std::vector<CarPosition> firstLocations, std::vector<CarPosition> secondLocations)
{
	std::vector<CarPosition> combinedLocations;
	combinedLocations.reserve(firstLocations.size() + secondLocations.size());
	for (const CarPosition &position : firstLocations)
	{
		combinedLocations.push_back(position);
	}
	for (const CarPosition &position : secondLocations)
	{
		combinedLocations.push_back(position);
	}
	return combinedLocations;
}

void AirRaceTrainingPlugin::setupCarMove(std::vector<CarPosition> positions) 
{
	if ( validPluginState() && !isMoving)
	{
		isMoving = true;
		counter = 0;
		currentMovingLocations = positions;	

		gameWrapper->SetTimeout([this](GameWrapper* gw) {
			this->moveCarAlongPositions();
			}, .1f);
	}
}

void AirRaceTrainingPlugin::moveCarAlongPositions()
{
	if (counter >= currentMovingLocations.size())
	{
		counter = 0;
		isMoving = false;
		currentMovingLocations = {};

		return;
	}

	gameWrapper->GetLocalCar().SetLocation(currentMovingLocations[counter].Location);
	gameWrapper->GetLocalCar().SetRotation(currentMovingLocations[counter].Rotation);
	counter++;

	gameWrapper->SetTimeout([this](GameWrapper* gw) {
		this->moveCarAlongPositions();
		}, .01f);
}

void AirRaceTrainingPlugin::onMapLoad(std::string eventName)
{
	gameWrapper->HookEvent("Function Engine.PrimitiveComponent.SetRBPosition", std::bind(&AirRaceTrainingPlugin::onInitialCarSpawn, this, std::placeholders::_1));
}

void AirRaceTrainingPlugin::onMapUnload(std::string eventName)
{
	gameWrapper->UnhookEvent("Function GameEvent_Soccar_TA.Active.StartRound");
	gameWrapper->UnhookEvent("Function TAGame.GameEvent_Soccar_TA.Destroyed");
}

void AirRaceTrainingPlugin::onInitialCarSpawn(std::string eventName)
{
	if (isPanicsAirRace())
	{
		gameWrapper->SetTimeout([this](GameWrapper* gw) {
			this->showEnabledStatus();
			this->gameWrapper->HookEvent("Function GameEvent_Soccar_TA.Active.StartRound", std::bind(&AirRaceTrainingPlugin::moveToSelectedCheckpoint, this, std::placeholders::_1));
			this->gameWrapper->HookEvent("Function TAGame.GameEvent_Soccar_TA.Destroyed", std::bind(&AirRaceTrainingPlugin::onMapUnload, this, std::placeholders::_1));
			}, 5.0f);
	}
	gameWrapper->UnhookEvent("Function Engine.PrimitiveComponent.SetRBPosition");
	
}

void AirRaceTrainingPlugin::moveCheckUp()
{
	if (!validPluginState() || gameWrapper->IsPaused()) { return; }

	(*selectedCheckpoint)++;
	if ((*selectedCheckpoint) > 10)
		*selectedCheckpoint = 0;

	cvarManager->executeCommand("airracetraining_panicsairrace_selectedcheck " + std::to_string(*selectedCheckpoint));
	log("#" + std::to_string((*selectedCheckpoint) + 1) + " Checkpoint Selected", true);
}

void AirRaceTrainingPlugin::moveCheckDown()
{
	if (!validPluginState() || gameWrapper->IsPaused()) { return; }

	(*selectedCheckpoint)--;
	if ((*selectedCheckpoint) < 0)
		*selectedCheckpoint = 10;

	cvarManager->executeCommand("airracetraining_panicsairrace_selectedcheck " + std::to_string(*selectedCheckpoint));
	log("#" + std::to_string((*selectedCheckpoint) + 1) + " Checkpoint Selected", true);
}

void AirRaceTrainingPlugin::moveToSelectedCheckpoint(std::string eventName)
{
	if (!validPluginState()) { return; }

	switch (*selectedCheckpoint) 
	{
	case 0:
		setupCarMove(SpawnToFirstCheckLocations);
		break;
	case 1:
		setupCarMove(SpawnToSecondCheckLocations);
		break;
	case 2:
		setupCarMove(SpawnToThirdCheckLocations);
		break;
	case 3:
		setupCarMove(SpawnToFourthCheckLocations);
		break;
	case 4:
		setupCarMove(SpawnToFifthCheckLocations);
		break;
	case 5:
		setupCarMove(SpawnToSixthCheckLocations);
		break;
	case 6:
		setupCarMove(SpawnToSeventhCheckLocations);
		break;
	case 7:
		setupCarMove(SpawnToEighthCheckLocations);
		break;
	case 8:
		setupCarMove(SpawnToNinthCheckLocations);
		break;
	case 9:
		setupCarMove(SpawnToTenthCheckLocations);
		break;
	case 10:
		setupCarMove(SpawnToLastCheckLocations);
		break;
	}
}

void AirRaceTrainingPlugin::log(std::string message, bool sendToChat)
{
	cvarManager->log(message);
	if (sendToChat && isPanicsAirRace() && cvarManager->getCvar("airracetraining_chatbox_messages").getBoolValue())
	{
		gameWrapper->LogToChatbox(message, "Air Race");
	}
}
