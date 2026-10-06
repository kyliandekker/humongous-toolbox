#include "./FinishedWindow.h"

#include <cmath>
#include <imgui/imgui.h>
#include <imgui/implot.h>
#include <imgui/imgui_helpers.h>
#include <shellapi.h>
#include <stb_image/stb_image.h>

#include <htb_lib/core/LibInfo.h>

#include <htb_lib_win32/dx11/DX11System.h>
#include <htb_lib_win32/dx11/SVGTextureCache.h>

#include "htb_patcher/imgui/ImGuiSystem.h"
#include "htb_patcher/LocText.h"
#include "htb_patcher/Patcher.h"

namespace htb::imgui
{
	//======================================================================================
	// FinishedWindow
	//======================================================================================
	FinishedWindow::FinishedWindow() : ImGui::BaseWindow(ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar, "PREVIEW", "FinishedWindow", true)
	{}

	//======================================================================================
	bool FinishedWindow::OnInitialized()
	{
		int channels = 0;
		unsigned char* pixels = stbi_load("../resources/images/logo.png", &m_iLogoWidth, &m_iLogoHeight, &channels, 4);
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

		return true;
	}

	//======================================================================================
	void FinishedWindow::Update()
	{
		ImVec2 padding = ImGui::GetStyle().ItemSpacing;
		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();
		float headerHeight = ImGui::GetContentRegionAvail().y * 0.5f;

		patch::Patcher& patcher = patch::GetPatcher();

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		if (m_pDefaultBackground)
		{
			ImVec2 bgMin = windowPos;
			ImVec2 bgMax(windowPos.x + windowSize.x, windowPos.y + headerHeight);

			drawList->AddImage(
				(ImTextureID)m_pDefaultBackground,
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

			if (logoWidth > windowSize.x)
			{
				logoWidth = windowSize.x;
				logoHeight = logoWidth / logoAspect;
			}

			logoMin = ImVec2(
				windowPos.x + (windowSize.x - logoWidth) * 0.5f,
				windowPos.y + (headerHeight - logoHeight) * 0.5f
			);

			logoMax = ImVec2(logoMin.x + logoWidth, logoMin.y + logoHeight);

			drawList->AddImage(
				(ImTextureID)m_pLogoTexture,
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

			ImGui::TextWrapped(FINISHED_WINDOW_TEXT);
			ImGui::PushFont(GetImGuiSystem().GetBoldFont());
			ImGui::TextWrapped(FINISHED_WINDOW_FOOTER);
			ImGui::PopFont();

			ID3D11ShaderResourceView* ppIcon = dx11::SVGTextureCache::Get("icon_pp.svg");
			if (ppIcon)
			{
				float iconSize = ImGui::GetFontSize() * 2.0f;
				int svgW = dx11::SVGTextureCache::GetWidth("icon_pp.svg");
				int svgH = dx11::SVGTextureCache::GetHeight("icon_pp.svg");
				float iconW = iconSize;
				float iconH = iconSize;
				if (svgW > 0 && svgH > 0)
				{
					float aspect = static_cast<float>(svgW) / static_cast<float>(svgH);
					if (aspect >= 1.0f)
					{
						iconH = iconW / aspect;
					}
					else
					{
						iconW = iconH * aspect;
					}
				}

				ImVec2 padding = ImGui::GetStyle().FramePadding;
				float supportTextH = ImGui::GetTextLineHeight() + ImGui::GetStyle().ItemSpacing.y;
				float buttonH = iconH + padding.y * 2.0f;
				float paypalBlockH = supportTextH + buttonH;

				ImGui::SetCursorPosY(ImGui::GetWindowContentRegionMax().y - paypalBlockH);

				const char* supportText = FINISHED_WINDOW_SUPPORT;
				ImVec2 supportTextSize = ImGui::CalcTextSize(supportText);
				ImGui::SetCursorPosX((ImGui::GetWindowWidth() - supportTextSize.x) * 0.5f);
				ImGui::Text("%s", supportText);

				ImGui::SetCursorPosX((ImGui::GetWindowWidth() - (iconW + padding.x * 2.0f)) * 0.5f);
				if (ImGui::ImageButton("##paypal", (ImTextureID)ppIcon, ImVec2(iconW, iconH)))
				{
					ShellExecuteA(nullptr, "open", "https://www.paypal.com/paypalme/KylianDekkerNL", nullptr, nullptr, SW_SHOWNORMAL);
				}
			}

			ImGui::PopStyleVar(2);
		}
		ImGui::EndChild();
	}

	//======================================================================================
	void FinishedWindow::Open()
	{
		patch::Patcher& patcher = patch::GetPatcher();
		const core::Data& pcmData = patcher.GetEndSound();

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
		BaseWindow::Open();
	}

	//======================================================================================
	void FinishedWindow::Close()
	{
		m_AudioPlayer.Stop();
		BaseWindow::Close();
	}
}
