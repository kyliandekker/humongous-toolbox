#pragma once

#include "imgui/windows/BaseWindow.h"

namespace htb::imgui
{
	//======================================================================================
	// PatchingWindow
	//======================================================================================
	/// <summary>
	/// The window that shows the patching progress.
	/// </summary>
	class PatchingWindow : public ImGui::BaseWindow
	{
	public:
		/// <summary>
		/// Constructs the main window.
		/// </summary>
		PatchingWindow();
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