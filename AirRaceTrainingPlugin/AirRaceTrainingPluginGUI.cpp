#include "pch.h"
#include "AirRaceTrainingPlugin.h"

// Don't call this yourself, BM will call this function with a pointer to the current ImGui context
void AirRaceTrainingPlugin::SetImGuiContext(uintptr_t ctx)
{
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

std::string AirRaceTrainingPlugin::GetPluginName()
{
	return "Air Race Training Plugin 2.0";
}

void AirRaceTrainingPlugin::RenderSettings()
{
	
}

void AirRaceTrainingPlugin::RenderBoolCvar(const char* cvarName, const char* cvarLabel)
{
	CVarWrapper boolCvar = cvarManager->getCvar(cvarName);
	if (!boolCvar) return;
	bool enabled = boolCvar.getBoolValue();
	if (ImGui::Checkbox(cvarLabel, &enabled)) {
		gameWrapper->Execute([this, boolCvar, enabled](...) mutable {
			boolCvar.setValue(enabled);
			});
	}
}