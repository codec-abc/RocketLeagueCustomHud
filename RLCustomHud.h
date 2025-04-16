#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);


#define RED ImColor(255, 0, 0, 255)
#define BLUE ImColor(0, 0, 255, 255)
#define GREEN ImColor(0, 255, 0, 255)
#define DARKGREEN ImColor(0, 128, 0, 255)
#define BLACK ImColor(0, 0, 0, 255)
#define WHITE ImColor(255, 255, 255, 255)
#define GREY ImColor(170, 170, 170, 255)
#define DARKGREY ImColor(85, 85, 85, 255)
#define YELLOW ImColor(255, 255, 0, 255)
#define PURPLE ImColor(128, 0, 128, 255)


class RLCustomHud: public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginWindow // , SettingsWindowBase
	//,public SettingsWindowBase // Uncomment if you wanna render your own tab in the settings menu
	//,public PluginWindowBase // Uncomment if you want to render your own plugin window
{

	std::shared_ptr<bool> enabled;

	//Boilerplate
	//void onUnload() override; // Uncomment and implement if you need a unload method
	void OnTick(std::string eventName);

	bool isWindowOpen_ = false;
	std::string menuTitle_ = "RLCustomHud";
	std::vector<ImVec2> pointsLeftAirRoll;
	std::vector<ImVec2> pointsRightAirRoll;
	ControllerInput inputs;

public:
	//void RenderSettings() override; // Uncomment if you wanna render your own tab in the settings menu
	void RenderWindow(); // Uncomment if you want to render your own plugin window
	void onLoad() override;

	void onUnload() override;

	void onTick(std::string eventName);

	void OnSetInput(CarWrapper cw, void* params);

	void Render() override;

	virtual std::string GetMenuName() override;

	virtual std::string GetMenuTitle() override;

	virtual void SetImGuiContext(uintptr_t ctx) override;

	virtual bool ShouldBlockInput() override;

	virtual bool IsActiveOverlay() override;

	virtual void OnOpen() override;

	virtual void OnClose() override;
	void RenderImGui();

	float transparency = 0.2f;
	int type = 0;
	int size = 1;
};
