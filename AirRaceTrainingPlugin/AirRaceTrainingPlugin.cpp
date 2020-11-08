#include "AirRaceTrainingPlugin.h"

BAKKESMOD_PLUGIN(AirRaceTrainingPlugin, "Air Race Training Plugin", "1.0", PLUGINTYPE_FREEPLAY)

std::vector<CarPosition> recordPositions;

void AirRaceTrainingPlugin::onLoad()
{
	loadSpawnToCheckLocations();

	cvarManager->registerNotifier("airracetraining_panicsairrace_firstcheck", [this](std::vector<std::string> params) {
		this->setupCarMove(SpawnToFirstCheckLocations);
		}, "", PERMISSION_FREEPLAY);
	cvarManager->registerNotifier("airracetraining_panicsairrace_secondcheck", [this](std::vector<std::string> params) {
		this->setupCarMove(SpawnToSecondCheckLocations);
		}, "", PERMISSION_FREEPLAY);
	cvarManager->registerNotifier("airracetraining_panicsairrace_thirdcheck", [this](std::vector<std::string> params) {
		this->setupCarMove(SpawnToThirdCheckLocations);
		}, "", PERMISSION_FREEPLAY);
	cvarManager->registerNotifier("airracetraining_panicsairrace_fourthcheck", [this](std::vector<std::string> params) {
		this->setupCarMove(SpawnToFourthCheckLocations);
		}, "", PERMISSION_FREEPLAY);
	cvarManager->registerNotifier("airracetraining_panicsairrace_fifthcheck", [this](std::vector<std::string> params) {
		this->setupCarMove(SpawnToFifthCheckLocations);
		}, "", PERMISSION_FREEPLAY);
	cvarManager->registerNotifier("airracetraining_panicsairrace_sixthcheck", [this](std::vector<std::string> params) {
		this->setupCarMove(SpawnToSixthCheckLocations);
		}, "", PERMISSION_FREEPLAY);
	cvarManager->registerNotifier("airracetraining_panicsairrace_seventhcheck", [this](std::vector<std::string> params) {
		this->setupCarMove(SpawnToSeventhCheckLocations);
		}, "", PERMISSION_FREEPLAY);
	cvarManager->registerNotifier("airracetraining_panicsairrace_eighthcheck", [this](std::vector<std::string> params) {
		this->setupCarMove(SpawnToEighthCheckLocations);
		}, "", PERMISSION_FREEPLAY);
	cvarManager->registerNotifier("airracetraining_panicsairrace_ninthcheck", [this](std::vector<std::string> params) {
		this->setupCarMove(SpawnToNinthCheckLocations);
		}, "", PERMISSION_FREEPLAY);
	cvarManager->registerNotifier("airracetraining_panicsairrace_tenthcheck", [this](std::vector<std::string> params) {
		this->setupCarMove(SpawnToTenthCheckLocations);
		}, "", PERMISSION_FREEPLAY);
	cvarManager->registerNotifier("airracetraining_panicsairrace_lastcheck", [this](std::vector<std::string> params) {
		this->setupCarMove(SpawnToLastCheckLocations);
		}, "", PERMISSION_FREEPLAY);
}

void AirRaceTrainingPlugin::onUnload()
{

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
	if (gameWrapper->IsInFreeplay() && gameWrapper->GetCurrentMap() == "panicsairrace" && !isMoving)
	{
		isMoving = true;
		counter = 0;
		currentMovingLocations = positions;

		moveCarAlongPositions();
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