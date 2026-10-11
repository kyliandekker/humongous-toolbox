#pragma once

#include "htb_ozonlaag/imgui/ImGuiSystem.h"

namespace htb::imgui
{
	class OzonlaagImGuiSystem : public ImGuiSystem
	{
	private:
		/// <summary>
		/// Called when the window is successfully initialized. Meant to initialize windows, modals, etc.
		/// </summary>
		void OnInitialized() override;

		/// <summary>
		/// Renders the windows.
		/// </summary>
		void OnRender() override;


	};
}