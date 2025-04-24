#include "pch.h"
#include "RLCustomHud.h"
#include <cmath>

BAKKESMOD_PLUGIN(RLCustomHud, "RL Custom Hud", plugin_version, 0)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void RLCustomHud::onLoad()
{
	_globalCvarManager = cvarManager;
	
	gameWrapper->SetTimeout([this](GameWrapper* gameWrapper) {
		cvarManager->executeCommand("togglemenu " + GetMenuName());
		}, 1);

	gameWrapper->HookEvent("Function Engine.GameViewportClient.Tick", bind(&RLCustomHud::OnTick, this, std::placeholders::_1));
	gameWrapper->HookEventWithCaller<CarWrapper>("Function TAGame.Car_TA.SetVehicleInput", 
		std::bind(&RLCustomHud::OnSetInput, this, std::placeholders::_1, std::placeholders::_2));

	colormap = getBMPYColorMap();
}

void RLCustomHud::RenderWindow()
{
	//if (gameWrapper->IsInGame())
	//{
	//	//auto server = gameWrapper->GetCurrentGameState();
	//	//if (!server.IsNull()) {
	//	//	if (!server.GetbMatchEnded()) {
	//	//		//inputs = gameWrapper->GetPlayerController().GetVehicleInput();
	//	//		this->RenderImGui();
	//	//	}
	//	//}

	//	//this->RenderImGui();
	//}

	if (gameWrapper->IsInGame() || gameWrapper->IsInCustomTraining() || gameWrapper->IsInOnlineGame() || gameWrapper->IsInOnlineGame())
	{
		this->RenderImGui();
	}

}

void RLCustomHud::OnSetInput(CarWrapper cw, void* params)
{
	//if (!gameWrapper->IsInGame()) 
	//{
	//	return;
	//}

	if (params == nullptr)
	{
		return;
	}

	ControllerInput* ci = (ControllerInput*)params;
	inputs = (ControllerInput)*ci;
}



void RLCustomHud::OnTick(std::string eventName)
{
	//std::ofstream outfile;
	//outfile.open("C:\\Users\\camille\\Desktop\\perso\\rlLogs.txt", std::ios_base::app); // append instead of overwrite
	//outfile << "OnTick" << std::endl;
	//if (gameWrapper.)

	CarWrapper car = gameWrapper->GetLocalCar();

	if (!car.IsNull()) {
		inputs = car.GetInput();
	}


}

void RLCustomHud::RenderImGui()
{

	//const auto directionalAirRoll = gameWrapper->IsKeyPressed(airRollKeyIndex);

	// Compute time since registering started
	//const auto time = to_string((system_clock::now() - startingPoint).count());

	float scale = (size == 0 ? 2.0f : 1.0f);

	ImGui::SetNextWindowBgAlpha(transparency);

	ImGui::SetNextWindowPos(ImVec2(128, 128), ImGuiCond_FirstUseEver);

	//float windowHeight = 156 * scale;

	//if (!titleBar) {
		//windowHeight -= ImGui::GetFrameHeight();
	//}

	//ImVec2 windowSize = ImVec2(216 * scale, windowHeight);
	int windowWith = 800;
	int windowHeight = 800;
	ImVec2 windowSize = ImVec2(windowWith, windowHeight);

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
	auto font = ImGui::GetFont();
	auto oldSize = font->FontSize;
	//font->FontSize *= 4;

	//if (inputs != nullptr)
	{
		float thickness = 10;
		float gap = windowWith * 0.05f;
		float arcSize = windowWith * 0.2f;
		pointsLeftAirRoll.clear();
		pointsRightAirRoll.clear();
		ImVec2 middleCross = ImVec2(p.x + 2 * gap + arcSize, p.y + windowHeight / 2);
		ImVec2 startLeftAirRoll = ImVec2(middleCross.x, middleCross.y);
		ImVec2 startRightAirRoll = ImVec2(middleCross.x, middleCross.y);

		float airRollLeftValue = inputs.Roll > 0 ? (inputs.Roll) : 0;
		float airRollRightValue = inputs.Roll < 0 ? (-inputs.Roll) : 0;
		float leftValue = inputs.Yaw < 0 ? (-inputs.Yaw) : 0;
		float rightValue = inputs.Yaw > 0 ? (inputs.Yaw) : 0;
		float downValue = inputs.Pitch > 0 ? (inputs.Pitch) : 0;
		float upValue = inputs.Pitch < 0 ? (-inputs.Pitch) : 0;
		float throttleValue = inputs.Throttle < 0 ? (-inputs.Throttle) : inputs.Throttle;
		float throttleSign = inputs.Throttle < 0 ? 1 : -1;

		float nbPointsArc = 90;
		for (float i = 0; i < nbPointsArc; i++)
		{
			float percent = i / nbPointsArc;
			float x = cos(i * 3.14159f / 180.0f);
			float alternateX = 1.0 - x;
			float y = sin(i * 3.14159f / 180.0f);

			if (airRollLeftValue >= percent)
			{
				pointsLeftAirRoll.push_back(ImVec2(
					startLeftAirRoll.x + x * arcSize + gap,
					startLeftAirRoll.y - y * arcSize - gap));
			}

			if (airRollRightValue >= percent)
			{
				pointsRightAirRoll.push_back(ImVec2(
					startRightAirRoll.x + alternateX * arcSize - arcSize - gap,
					startRightAirRoll.y - y * arcSize - gap));
			}
		}

		// air roll left
		drawList->AddPolyline(
			&pointsLeftAirRoll.front(),
			pointsLeftAirRoll.size(),
			sampleColorMap(airRollLeftValue, colormap),
			false,
			thickness);

		// air roll right
		drawList->AddPolyline(
			&pointsRightAirRoll.front(),
			pointsRightAirRoll.size(),
			sampleColorMap(airRollRightValue, colormap),
			false,
			thickness);

		// right
		drawList->AddLine(
			ImVec2(middleCross.x + gap, middleCross.y), 
			ImVec2(middleCross.x + gap + (arcSize * rightValue), middleCross.y),
			sampleColorMap(rightValue, colormap),
			thickness);

		// left
		drawList->AddLine(
			ImVec2(middleCross.x - gap, middleCross.y), 
			ImVec2(middleCross.x - gap - (arcSize * leftValue), middleCross.y),
			sampleColorMap(leftValue, colormap),
			thickness);

		// down
		drawList->AddLine(
			ImVec2(middleCross.x, middleCross.y + gap), 
			ImVec2(middleCross.x, middleCross.y + gap + (arcSize * downValue)),
			sampleColorMap(downValue, colormap),
			thickness);

		// up
		drawList->AddLine(
			ImVec2(middleCross.x, middleCross.y - gap), 
			ImVec2(middleCross.x, middleCross.y - gap - (arcSize * upValue)),
			sampleColorMap(upValue, colormap),
			thickness);

		// front / backward
		drawList->AddLine(
			ImVec2(middleCross.x + 2 * gap + arcSize, middleCross.y),
			ImVec2(middleCross.x + 2 * gap + arcSize, middleCross.y + (arcSize + 2 * gap) * throttleValue * throttleSign), 
			sampleColorMap(throttleValue, colormap),
			thickness);

		ImGui::SetCursorPos(ImVec2((size == 0 ? cursorPosition.x + 73 : cursorPosition.x + 156), cursorPosition.y + 8 * scale));

		float labelPosition = middleCross.x + 3 * gap + arcSize;
		int backgroundColor = 0x33;

		ImColor jumpColor = ImColor(backgroundColor, 0xff, backgroundColor, 0xff);
		if (inputs.Jump == 0)
		{
			jumpColor = ImColor(backgroundColor, backgroundColor, backgroundColor, 0xff);
		}
		drawList->AddText(ImVec2(labelPosition, p.y + 2.0f * windowHeight / 5.0f), jumpColor, "JUMP ");
		//drawList->AddText(ImVec2(labelPosition, p.y + 2.5f * windowHeight / 5.0f), 0xffffffff, std::to_string(inputs.Jump).c_str());

		ImColor boostColor = ImColor(backgroundColor, backgroundColor, 0xff, 0xff);
		if (inputs.ActivateBoost == 0)
		{
			boostColor = ImColor(backgroundColor, backgroundColor, backgroundColor, 0xff);
		}
		drawList->AddText(ImVec2(labelPosition, p.y + 3.0f * windowHeight / 5.0f), boostColor, "BOOST");
		//drawList->AddText(ImVec2(labelPosition, p.y + 3.5f * windowHeight / 5.0f), 0xffffffff, std::to_string(inputs.ActivateBoost).c_str());

		ImColor powerSlideColor = ImColor(0xff, backgroundColor, backgroundColor, 0xff);
		if (inputs.Handbrake == 0)
		{
			powerSlideColor = ImColor(backgroundColor, backgroundColor, backgroundColor, 0xff);
		}
		drawList->AddText(ImVec2(labelPosition, p.y + 4.0f * windowHeight / 5.0f), powerSlideColor, "SLIDE");
		//drawList->AddText(ImVec2(labelPosition, p.y + 4.5f * windowHeight / 5.0f), 0xffffffff, std::to_string(inputs.Handbrake).c_str());
	}

	font->FontSize = oldSize;

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