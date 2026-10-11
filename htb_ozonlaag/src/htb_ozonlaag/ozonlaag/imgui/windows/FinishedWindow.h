#pragma once

#include "imgui/windows/BaseWindow.h"

namespace htb::imgui
{
	//======================================================================================
	// FinishedWindow
	//======================================================================================
	/// <summary>
	/// The window that shows up after successfully patching.
	/// </summary>
	class FinishedWindow : public ImGui::BaseWindow
	{
	public:
		/// <summary>
		/// Constructs the main window.
		/// </summary>
		FinishedWindow();
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
	};
}