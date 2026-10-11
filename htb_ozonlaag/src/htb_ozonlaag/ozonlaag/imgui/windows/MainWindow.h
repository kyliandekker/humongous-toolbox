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
		/// Renders the preview window content.
		/// </summary>
		void OnUpdate() override;

		/// <summary>
		/// Initializes all behaviours and values for the window.
		/// </summary>
		/// <returns>True if initialization is successful, otherwise false.</returns>
		bool OnInitialized() override;

		bool m_bShowFailedPopup = false;
		std::string m_sFailureMessage = "";
	};
}