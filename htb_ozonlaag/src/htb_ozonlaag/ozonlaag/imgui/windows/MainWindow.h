#pragma once

#include "imgui/windows/BaseWindow.h"

namespace htb::imgui
{
	//======================================================================================
	// MainWindow
	//======================================================================================
	/// <summary>
	/// The starting window. Fallback when patching fails.
	/// </summary>
	class MainWindow : public ImGui::BaseWindow
	{
	public:
		/// <summary>
		/// Constructs the main window.
		/// </summary>
		MainWindow();

		/// <summary>
		/// Shows the patch failure popup on next frame.
		/// </summary>
		void ShowFailedPopup(const std::string& a_sFailureMessage);
	private:
		/// <summary>
		/// Initializes all behaviours and values for the window.
		/// </summary>
		/// <returns>True if initialization is successful, otherwise false.</returns>
		bool OnInitialized() override;

		/// <summary>
		/// Renders the preview window content.
		/// </summary>
		void OnUpdate() override;

		/// <summary>
		/// Processes the settings.
		/// </summary>
		void ProcessSettings();

		/// <summary>
		/// Shows the browse button.
		/// </summary>
		/// <returns>True if the path was changed.</returns>
		bool ShowBrowseButton(char* a_sPath, const std::string& a_sName, const std::string& a_sId);

		/// <summary>
		/// Searches the installation path for a given game.
		/// </summary>
		/// <returns>True if the path was changed.</returns>
		bool SearchInstallation(char* a_sPath, const std::string& a_sName);

		bool m_bShowFailedPopup = false;
		std::string m_sFailureMessage = "";

		static constexpr size_t PATH_MAX = 256;
		char m_sSpyFox1Path[PATH_MAX] = {};
		char m_sSpyFox2Path[PATH_MAX] = {};
		char m_sSpyFox3Path[PATH_MAX] = {};
	};
}