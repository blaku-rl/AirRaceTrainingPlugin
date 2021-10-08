#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"
#include "AirRaceMaps/BaseAirRaceMap.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

class AirRaceTrainingPlugin: public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginSettingsWindow
{
public:
	void onLoad();
	void onUnload();

private:
	std::shared_ptr<bool> isEnabled;
	std::shared_ptr<bool> chatboxEnabled;
	std::shared_ptr<bool> teleportsEnabled;
	std::shared_ptr<bool> setSpawnEnabled;
	std::shared_ptr<bool> randomizeSpawn;
	bool isFirstSpawn;

	std::vector<std::shared_ptr<BaseAirRaceMap>> allMapInfo;
	std::shared_ptr<BaseAirRaceMap> loadedMap;

	void ToggleCvar(const bool& cvar, const std::string& cvarStr, const std::string& cvarName);
	void ShowEnabledStatus();
	bool ValidPluginState();
	bool IsAnAirRaceMap();
	void OnMapUnload(std::string eventName);
	void MoveCheckUp();
	void MoveCheckDown();
	void OnCarSpawns(std::string eventName);
	void MoveToSelectedCheckpoint();
	void RandomizeSpawn();
	void SetSpawn(const CarPosition& spawnPos);
	void Log(std::string message, bool sendToChat);

	virtual void SetImGuiContext(uintptr_t ctx) override;
	virtual std::string GetPluginName() override;
	virtual void RenderSettings() override;
	void RenderBoolCvar(const char* cvarName, const char* cvarLabel)
};

