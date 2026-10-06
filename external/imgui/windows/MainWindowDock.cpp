#include "./MainWindowDock.h"

// external
#include <imgui/imgui.h>

namespace ImGui
{
	//---------------------------------------------------------------------
	// MainWindowDock
	//---------------------------------------------------------------------
	MainWindowDock::MainWindowDock() : BaseWindow(ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar, "", "DockSpace")
	{}

	//---------------------------------------------------------------------
	void MainWindowDock::Update()
	{
		ImGui::DockSpace(ImGui::GetID("DockSpace"));
	}
}