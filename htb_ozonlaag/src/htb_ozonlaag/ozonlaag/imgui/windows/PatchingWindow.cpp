#include "./PatchingWindow.h"

namespace htb::imgui
{
	//======================================================================================
	// PatchingWindow
	//======================================================================================
	PatchingWindow::PatchingWindow() : ImGui::BaseWindow(
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar, 
		"PREVIEW", 
		"PatchingWindow", 
		true)
	{}
	
	//======================================================================================
	void PatchingWindow::OnUpdate()
	{
		ImGui::Text(" am patching lookat me go");
	}

	//======================================================================================
	bool PatchingWindow::OnInitialized()
	{
		return true;
	}
}