#include "./FinishedWindow.h"

namespace htb::imgui
{
	//======================================================================================
	// FinishedWindow
	//======================================================================================
	FinishedWindow::FinishedWindow() : ImGui::BaseWindow(
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar, 
		"PREVIEW", 
		"FinishedWindow", 
		true)
	{}
	
	//======================================================================================
	void FinishedWindow::OnUpdate()
	{
		ImGui::Text("Test");
	}

	//======================================================================================
	bool FinishedWindow::OnInitialized()
	{
		return true;
	}
}