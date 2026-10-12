#include "./PatchingWindow.h"

#include <imgui_helpers.h>
#include <imgui_internal.h>

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


	//---------------------------------------------------------------------
	void Spinner(ImDrawList* draw, ImVec2 center, float radius = 10.0f, float thickness = 3.0f)
	{
		const int segments = 30;
		const float speed = 5.0f;

		const float time = ImGui::GetTime();
		const float start = std::fmod(time * speed, IM_PI * 2.0f);

		for (int i = 0; i < segments; ++i)
		{
			const float a =
				start + (static_cast<float>(i) / segments) * IM_PI * 2.0f;

			// Fade around the circle
			const float alpha =
				static_cast<float>(i) / segments;

			ImVec2 p1(
				center.x + std::cos(a) * radius * 0.5f,
				center.y + std::sin(a) * radius * 0.5f
			);

			ImVec2 p2(
				center.x + std::cos(a) * radius,
				center.y + std::sin(a) * radius
			);

			draw->AddLine(
				p1,
				p2,
				ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, alpha)),
				thickness
			);
		}
	}
	
	//======================================================================================
	void PatchingWindow::OnUpdate()
	{
		htb::ozonlaag::OzonlaagPatcher* patcher = htb::ozonlaag::GetOzonlaagPatcher();
		if (!patcher)
		{
			return;
		}

		const float deltaTime = ImGui::GetIO().DeltaTime;

		m_fTextTimer += deltaTime;

		if (m_fTextTimer >= m_fTextTimerInterval)
		{
			m_fTextTimer = 0.0f;

			static int dotCount = 0;
			dotCount = (dotCount + 1) % 3;

			m_sStatusMessageInWindow = m_sStatusMessage
				+ std::string(dotCount, '.');
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

			const float spinnerRadius = 7.0f;
			const float spinnerSpacing = 8.0f;
			const ImVec2 spinnerCenter(
				ImGui::GetCursorScreenPos().x + spinnerRadius,
				ImGui::GetCursorScreenPos().y + ImGui::GetTextLineHeight() * 0.5f
			);
			Spinner(ImGui::GetWindowDrawList(), spinnerCenter, spinnerRadius);

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + spinnerRadius * 2.0f + spinnerSpacing);
			ImGui::TextWrapped("%s", m_sStatusMessageInWindow.c_str());
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