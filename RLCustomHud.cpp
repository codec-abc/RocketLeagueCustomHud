#include "pch.h"
#include "RLCustomHud.h"

BAKKESMOD_PLUGIN(RLCustomHud, "RL Custom Hud", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void RLCustomHud::onLoad()
{
	_globalCvarManager = cvarManager;
	
	gameWrapper->SetTimeout([this](GameWrapper* gameWrapper) {
		cvarManager->executeCommand("togglemenu " + GetMenuName());
		}, 1);

	gameWrapper->HookEvent("Function Engine.GameViewportClient.Tick", bind(&RLCustomHud::OnTick, this, std::placeholders::_1));
}

void RLCustomHud::RenderWindow()
{
	if (gameWrapper->IsInGame())
	{
		auto server = gameWrapper->GetCurrentGameState();
		if (!server.IsNull()) {
			if (!server.GetbMatchEnded()) {
				this->RenderImGui();
			}
		}
	}
}


void RLCustomHud::OnTick(std::string eventName)
{
	//std::ofstream outfile;
	//outfile.open("C:\\Users\\camille\\Desktop\\perso\\rlLogs.txt", std::ios_base::app); // append instead of overwrite
	//outfile << "OnTick" << std::endl;
}

void RLCustomHud::RenderImGui()
{
	//const auto inputs = gameWrapper->GetPlayerController().GetVehicleInput();
	//const auto directionalAirRoll = gameWrapper->IsKeyPressed(airRollKeyIndex);

	// Compute time since registering started
	//const auto time = to_string((system_clock::now() - startingPoint).count());

	float scale = (size == 0 ? 2.0f : 1.0f);

	ImGui::SetNextWindowBgAlpha(transparency);

	ImGui::SetNextWindowPos(ImVec2(128, 128), ImGuiCond_FirstUseEver);

	float windowHeight = 156 * scale;

	//if (!titleBar) {
		windowHeight -= ImGui::GetFrameHeight();
	//}

	ImVec2 windowSize = ImVec2(216 * scale, windowHeight);

	if (size == 1) 
	{
		windowSize.x -= 16;
		windowSize.y -= 32;
	}

	ImGui::SetNextWindowSize(windowSize);

	ImGuiWindowFlags windowFlags = 
		ImGuiWindowFlags_NoCollapse | 
		ImGuiWindowFlags_NoResize | 
		ImGuiWindowFlags_NoTitleBar;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::Begin(GetMenuTitle().c_str(), &isWindowOpen_, windowFlags);

	ImVec2 cursorPosition = ImGui::GetCursorPos();

	if (size == 1) {
		ImGuiIO io = ImGui::GetIO();

		if (io.Fonts->Fonts.size() >= 1) {
			ImFont* font = io.Fonts->Fonts[1];
			ImGui::PushFont(font);
		}
	}

	ImDrawList* drawList = ImGui::GetWindowDrawList();

	ImVec2 p = ImGui::GetCursorScreenPos();

	p.x += 12 * scale;

	float buttonWidth = 48 * scale, buttonHeight = 16 * scale;

	ImVec2 buttonLBPosition = ImVec2(p.x, p.y);
	ImVec2 buttonRBPosition = ImVec2(p.x + 128 * scale, p.y);

	float leftStickRadius = 32 * scale;
	float buttonRadius = 12 * scale;
	ImVec2 leftStickCenter = ImVec2(p.x + leftStickRadius, p.y + leftStickRadius);
	ImVec2 buttonsCenter = ImVec2(leftStickCenter.x + 1 * scale, leftStickCenter.y);
	ImVec2 buttonPosition = ImVec2(buttonsCenter.x, buttonsCenter.y + buttonRadius * 2);
	drawList->AddCircle(buttonPosition, buttonRadius, WHITE, 32, 2 * scale);
	drawList->AddText(buttonPosition, 0xffffffff, "Pouet");
	ImGui::SetCursorPos(ImVec2((size == 0 ? cursorPosition.x + 73 : cursorPosition.x + 156), cursorPosition.y + 8 * scale));

	if (size == 1) {
		ImGui::PopFont();
	}

	ImGui::PopStyleVar();

	ImGui::End();
}


std::string RLCustomHud::GetMenuName()
{
	return "rlcustomhud";
}

std::string RLCustomHud::GetMenuTitle()
{
	return menuTitle_;
}

void RLCustomHud::SetImGuiContext(uintptr_t ctx)
{
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

bool RLCustomHud::ShouldBlockInput()
{
	//return ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
	return false;
}

void RLCustomHud::onUnload() 
{
}

bool RLCustomHud::IsActiveOverlay()
{
	return false;
}

void RLCustomHud::OnOpen()
{
	isWindowOpen_ = true;
}

void RLCustomHud::OnClose()
{
	isWindowOpen_ = false;
}

void RLCustomHud::Render()
{
	//if (!ImGui::Begin(menuTitle_.c_str(), &isWindowOpen_, ImGuiWindowFlags_None))
	//{
	//	// Early out if the window is collapsed, as an optimization.
	//	ImGui::End();
	//	return;
	//}

	this->RenderWindow();
}

//	ImGui::End();
//
//	if (!isWindowOpen_)
//	{
//		_globalCvarManager->executeCommand("togglemenu " + GetMenuName());
//	}
//}