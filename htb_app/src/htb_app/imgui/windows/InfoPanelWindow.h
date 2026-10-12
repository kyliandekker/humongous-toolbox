#pragma once

#include "htb_app/imgui/windows/HEBaseWindow.h"

namespace htb::imgui
{
	//---------------------------------------------------------------------
	// InfoPanelWindow
	//---------------------------------------------------------------------
	/// <summary>
	/// A window for inspecting specific archives.
	/// </summary>
	class InfoPanelWindow : public LoggerDependentWindow
	{
	public:
		/// <summary>
		/// Constructs an explorer window.
		/// </summary>
		InfoPanelWindow();
	private:
		/// <summary>
		/// Renders the explorer window.
		/// </summary>
		void OnUpdate() override;

		/// <summary>
		/// Initializes all behaviours and values for the window.
		/// </summary>
		/// <returns>True if initialization is successful, otherwise false.</returns>
		bool OnInitialized() override;
	};
}