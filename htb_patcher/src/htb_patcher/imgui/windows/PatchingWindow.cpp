#include "./PatchingWindow.h"

#include <cmath>
#include <imgui/imgui.h>
#include <imgui/implot.h>
#include <imgui/imgui_helpers.h>
#include <stb_image/stb_image.h>

#include <htb_lib/core/LibInfo.h>

#include <htb_lib_win32/dx11/DX11System.h>

#include "htb_patcher/imgui/ImGuiSystem.h"
#include "htb_patcher/LocText.h"
#include "htb_patcher/Patcher.h"

namespace htb::imgui
{
	//======================================================================================
	// PatchingWindow
	//======================================================================================
	PatchingWindow::PatchingWindow() : ImGui::BaseWindow(ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar, "PREVIEW", "PatchingWindow", true)
	{
	}

	//======================================================================================
	bool PatchingWindow::OnInitialized()
	{
		int channels = 0;
		unsigned char* pixels = stbi_load("../resources/images/op_weg_naar.png", &m_iLogoWidth, &m_iLogoHeight, &channels, 4);
		if (pixels)
		{
			m_pLogoTexture = dx11::GetDX11System().CreateTexture(pixels, m_iLogoWidth, m_iLogoHeight);
			stbi_image_free(pixels);
		}

		int bgW = 0, bgH = 0;
		unsigned char* bgPixels = stbi_load("../resources/images/background.png", &bgW, &bgH, &channels, 4);
		if (bgPixels)
		{
			m_pDefaultBackground = dx11::GetDX11System().CreateTexture(bgPixels, bgW, bgH);
			stbi_image_free(bgPixels);
		}

		m_fLastSwitchTime = ImGui::GetTime();
		return true;
	}

	//======================================================================================
	void PatchingWindow::Update()
	{
		patch::Patcher& patcher = patch::GetPatcher();
		const core::Data& pcmData = patcher.GetSong();

		if (!m_AudioPlayer.IsPlaying() && !pcmData.empty())
		{
			if (!m_AudioPlayer.IsOpen())
			{
				m_AudioPlayer.Open(
					HE_SAMPLE_RATE,
					HE_BITS_PER_SAMPLE,
					HE_CHANNELS
				);
			}

			m_AudioPlayer.Play(
				pcmData.data(),
				pcmData.size()
			);
		}

		ImVec2 padding = ImGui::GetStyle().ItemSpacing;
		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();
		float headerHeight = ImGui::GetContentRegionAvail().y * 0.5f;

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		std::vector<patch::Image>& backgrounds = patcher.GetBackgroundImages();
		if (!backgrounds.empty())
		{
			double now = ImGui::GetTime();
			if (now - m_fLastSwitchTime >= m_fSwitchInterval)
			{
				m_iCurrentBackground = (m_iCurrentBackground + 1) % backgrounds.size();
				m_fLastSwitchTime = now;

				m_pCurrentBackground = backgrounds[m_iCurrentBackground].GetSRV();
			}

			if (m_pCurrentBackground)
			{
				ImVec2 bgMin = windowPos;
				ImVec2 bgMax(windowPos.x + windowSize.x, windowPos.y + headerHeight);

				drawList->AddImage(
					(ImTextureID) m_pCurrentBackground,
					bgMin,
					bgMax,
					ImVec2(0, 0),
					ImVec2(1, 1)
				);
			}
		}

		if (m_pDefaultBackground && (backgrounds.empty() || !m_pCurrentBackground))
		{
			ImVec2 bgMin = windowPos;
			ImVec2 bgMax(windowPos.x + windowSize.x, windowPos.y + headerHeight);

			drawList->AddImage(
				(ImTextureID) m_pDefaultBackground,
				bgMin,
				bgMax,
				ImVec2(0, 0),
				ImVec2(1, 1)
			);
		}

		if (m_pLogoTexture)
		{
			float logoAspect = static_cast<float>(m_iLogoWidth) / static_cast<float>(m_iLogoHeight);
			float logoHeight = headerHeight;
			float logoWidth = logoHeight * logoAspect;

			ImVec2 logoMin;
			ImVec2 logoMax;

			if (m_pCurrentBackground)
			{
				logoHeight = headerHeight * 0.3f;
				logoWidth = logoHeight * logoAspect;
				if (logoWidth > windowSize.x)
				{
					logoWidth = windowSize.x;
					logoHeight = logoWidth / logoAspect;
				}

				logoMin = ImVec2(
					windowPos.x + padding.x,
					windowPos.y + padding.y
				);
			}
			else
			{
				if (logoWidth > windowSize.x)
				{
					logoWidth = windowSize.x;
					logoHeight = logoWidth / logoAspect;
				}

				logoMin = ImVec2(
					windowPos.x + (windowSize.x - logoWidth) * 0.5f,
					windowPos.y + (headerHeight - logoHeight) * 0.5f
				);
			}

			logoMax = ImVec2(logoMin.x + logoWidth, logoMin.y + logoHeight);

			drawList->AddImage(
				(ImTextureID) m_pLogoTexture,
				logoMin,
				logoMax,
				ImVec2(0, 0),
				ImVec2(1, 1)
			);
		}

		ImGui::Dummy(ImVec2(windowSize.x, headerHeight - padding.y));

		std::string patcherVersion = patcher.GetVersion() + " using " + LIB_VERSION;
		ImVec2 patcherTextSize = ImGui::CalcTextSize(patcherVersion.c_str());
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - patcherTextSize.x) * 0.5f);
		ImGui::Text("%s", patcherVersion.c_str());

		if (ImGui::BeginChild(
			FormatId("", CHILD_ID, "PREVIEW").c_str(),
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

			float targetProgress = patcher.GetProgress();
			if (m_fDisplayProgress < targetProgress)
			{
				m_fDisplayProgress += (targetProgress - m_fDisplayProgress) * 0.05f;
				if (targetProgress - m_fDisplayProgress < 0.001f)
				{
					m_fDisplayProgress = targetProgress;
				}
			}
			ImGui::ProgressBar(m_fDisplayProgress);

			if (m_fDisplayProgress >= 1)
			{
				patcher.SetPatchState(patch::PatchState::FINALIZED);
			}

			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::PushFont(GetImGuiSystem().GetBoldFont());
			ImGui::TextWrapped("%s", patcher.GetBusyWith().c_str());
			ImGui::PopFont();

			ImGui::PopStyleVar(2);
		}
		ImGui::EndChild();

		if (patcher.HasFailed())
		{
			patcher.Join();
			patcher.SetPatchState(patch::PatchState::FAILED);
		}
		else if (patcher.IsFinished())
		{
			patcher.Join();
			patcher.SetPatchState(patch::PatchState::FINISHED);
		}
	}
	//======================================================================================
	void PatchingWindow::Close()
	{
		m_AudioPlayer.Stop();
		BaseWindow::Close();
	}
}
