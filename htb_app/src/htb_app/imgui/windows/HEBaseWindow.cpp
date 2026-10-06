#include "HEBaseWindow.h"

#include <imgui.h>

#include "htb_app/imgui/ImGuiSystem.h"
#include "htb_app/editor/Workspace.h"

namespace htb::imgui
{
	//---------------------------------------------------------------------
	void HEBaseWindow::Render()
	{
		ImGui::PushFont(GetImGuiSystem().GetDefaultFont());
		ImGui::BaseWindow::Render();
		ImGui::PopFont();
	}

	//---------------------------------------------------------------------
	void LoggerDependentWindow::Render()
	{
		bool isOpen = GetWorkspace().IsLogHistoryPanelOpen();
		if (isOpen)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		}
		HEBaseWindow::Render();
		if (isOpen)
		{
			ImGui::PopItemFlag();
		}
	}
}