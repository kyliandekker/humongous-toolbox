#include "./MainWindow.h"

namespace htb::imgui
{
	//======================================================================================
	// MainWindow
	//======================================================================================
	MainWindow::MainWindow() : ImGui::BaseWindow(
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar, 
		"PREVIEW", 
		"MainWindow", 
		true)
	{}

	//======================================================================================
    void MainWindow::ShowFailedPopup(const std::string & a_sFailureMessage)
    {
		m_sFailureMessage = a_sFailureMessage;
	}
	
	//======================================================================================
	void MainWindow::OnUpdate()
	{
		ImGui::Text("Test");
	}

	//======================================================================================
	bool MainWindow::OnInitialized()
	{
		return true;
	}
}