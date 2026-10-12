#include "./PatchingWindow.h"

#include <imgui_helpers.h>

#include <htb_lib/core/LibInfo.h>

#include "htb_ozonlaag/imgui/ImGuiSystem.h"
#include "htb_ozonlaag/ozonlaag/LocText.h"
#include "htb_ozonlaag/ozonlaag/OzonlaagPatcher.h"

using namespace htb::ozonlaag;

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
	bool PatchingWindow::OnInitialized()
	{
		m_fDisplayProgress = 0.0f;
		m_fProgress = 0.0f;

		const htb::ozonlaag::OzonlaagPatcher* patcher = htb::ozonlaag::GetOzonlaagPatcher();
		if (!patcher)
		{
			return false;
		}

		patcher->GetPatcherProgress().OnChanged() += std::bind(&PatchingWindow::OnPatcherProgressChanged, this, std::placeholders::_1, std::placeholders::_2);
		patcher->GetStatusMessage().OnChanged() += std::bind(&PatchingWindow::OnStatusMessageChanged, this, std::placeholders::_1, std::placeholders::_2);

		return true;
	}

	//======================================================================================
	void PatchingWindow::Open()
	{
		BaseWindow::Open();
		m_fDisplayProgress = 0.0f;
		m_fProgress = 0.0f;
	}
	
	//======================================================================================
	void PatchingWindow::OnUpdate()
	{
		htb::ozonlaag::OzonlaagPatcher* patcher = htb::ozonlaag::GetOzonlaagPatcher();
		if (!patcher)
		{
			return;
		}

		const std::string patcherVersion = patcher->GetVersion() + " using " + LIB_VERSION;
		const ImVec2 patcherTextSize = ImGui::CalcTextSize(patcherVersion.c_str());
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - patcherTextSize.x) * 0.5f);
		ImGui::Text("%s", patcherVersion.c_str());

		if (ImGui::BeginChild(
			FormatId("", CHILD_ID, "PATCHING_PROGRESS").c_str(),
			ImVec2(
			ImGui::GetContentRegionAvail().x,
			ImGui::GetContentRegionAvail().y
			),
			ImGuiChildFlags_Borders
		))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 8));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 10));

			ImGui::TextWrapped(PATCHING_WINDOW_WAITING_TEXT);

			ImGui::Spacing();
			ImGui::Spacing();

			float targetProgress = m_fProgress;
			if (m_fDisplayProgress < targetProgress)
			{
				m_fDisplayProgress += (targetProgress - m_fDisplayProgress) * 0.05f;
				if (targetProgress - m_fDisplayProgress < 0.001f)
				{
					m_fDisplayProgress = targetProgress;
				}
			}
			ImGui::ProgressBar(m_fDisplayProgress);

			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::PushFont(GetImGuiSystem().GetBoldFont());
			ImGui::TextWrapped("%s", m_sStatusMessage.c_str());
			ImGui::PopFont();

			ImGui::PopStyleVar();
			ImGui::PopStyleVar();
		}
		ImGui::EndChild();
	}

	//======================================================================================
	void PatchingWindow::OnPatcherProgressChanged(float a_fOldValue, float a_fNewValue)
	{
		m_fProgress = a_fNewValue;
	}

	//======================================================================================
	void PatchingWindow::OnStatusMessageChanged(const std::string& a_sOoldMessage, const std::string& a_sNewMessage)
	{
		m_sStatusMessage = a_sNewMessage;
	}
}