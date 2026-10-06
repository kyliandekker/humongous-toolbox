#include "./MainWindow.h"

#include <cmath>
#include <cstring>
#include <imgui/imgui.h>
#include <imgui/implot.h>
#include <imgui/imgui_helpers.h>
#include <stb_image/stb_image.h>

#include <htb_lib/core/LibInfo.h>
#include <htb_lib/file/File.h>

#include <htb_lib_win32/dx11/DX11System.h>
#include <htb_lib_win32/dx11/SVGTextureCache.h>
#include <htb_lib_win32/win32/winfile.h>

#include "htb_patcher/imgui/ImGuiSystem.h"
#include "htb_patcher/imgui/font_icon_patch.h"
#include "htb_patcher/LocText.h"
#include "htb_patcher/Patcher.h"

namespace htb::imgui
{
	//======================================================================================
	// MainWindow
	//======================================================================================
	MainWindow::MainWindow() : ImGui::BaseWindow(ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar, "PREVIEW", "MainWindow", true)
	{
	}

	//======================================================================================
	void MainWindow::ShowFailedPopup()
	{
		m_bShowFailedPopup = true;
	}

	//======================================================================================
	bool MainWindow::OnInitialized()
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
			m_pBackground = dx11::GetDX11System().CreateTexture(bgPixels, bgW, bgH);
			stbi_image_free(bgPixels);
		}

		strncpy(m_sSpyFox1Path, patch::GetPatcher().GetSpyFox1Path().string().c_str(), PATH_MAX - 1);
		strncpy(m_sSpyFox2Path, patch::GetPatcher().GetSpyFox2Path().string().c_str(), PATH_MAX - 1);
		strncpy(m_sSpyFox3Path, patch::GetPatcher().GetSpyFox3Path().string().c_str(), PATH_MAX - 1);

		return true;
	}

	//======================================================================================
	void MainWindow::Update()
	{
		ImVec2 padding = ImGui::GetStyle().ItemSpacing;
		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();
		float headerHeight = ImGui::GetContentRegionAvail().y * 0.25f;

		if (m_pBackground)
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 bgMin = windowPos;
			ImVec2 bgMax(windowPos.x + windowSize.x, windowPos.y + headerHeight);

			drawList->AddImage(
				(ImTextureID) m_pBackground,
				bgMin,
				bgMax,
				ImVec2(0, 0),
				ImVec2(1, 1)
			);

			if (m_pLogoTexture)
			{
				float logoAspect = static_cast<float>(m_iLogoWidth) / static_cast<float>(m_iLogoHeight);
				float logoHeight = headerHeight;
				float logoWidth = logoHeight * logoAspect;
				if (logoWidth > windowSize.x)
				{
					logoWidth = windowSize.x;
					logoHeight = logoWidth / logoAspect;
				}

				ImVec2 logoMin(
					windowPos.x + (windowSize.x - logoWidth) * 0.5f,
					windowPos.y + (headerHeight - logoHeight) * 0.5f
				);
				ImVec2 logoMax(logoMin.x + logoWidth, logoMin.y + logoHeight);

				drawList->AddImage(
					(ImTextureID) m_pLogoTexture,
					logoMin,
					logoMax,
					ImVec2(0, 0),
					ImVec2(1, 1)
				);
			}

			ImGui::Dummy(ImVec2(windowSize.x, headerHeight - padding.y));
		}

		patch::Patcher& patcher = patch::GetPatcher();

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

			ImGui::TextWrapped(MAIN_WINDOW_SELECT);

			ImGui::Spacing();

			struct FolderRow
			{
				std::string m_sLabel;
				std::string m_sHint;
				std::string m_sIconName;
				char* m_sPath;
			};

			FolderRow rows[] = {
				{ MAIN_WINDOW_INSTALL_PATH_SPYFOX_1, MAIN_WINDOW_PATH_TO_SPYFOX_1, "icon_folder_spyfox.svg", m_sSpyFox1Path },
				{ MAIN_WINDOW_INSTALL_PATH_SPYFOX_2, MAIN_WINDOW_PATH_TO_SPYFOX_2, "icon_folder_spyfox2.svg", m_sSpyFox2Path },
				{ MAIN_WINDOW_INSTALL_PATH_SPYFOX_3, MAIN_WINDOW_PATH_TO_SPYFOX_3, "icon_folder_spyfox3.svg", m_sSpyFox3Path },
			};

			std::string buttonText = std::string(icon::ICON_FOLDER) + MAIN_WINDOW_BROWSE;

			float iconSize = ImGui::GetFontSize() * 3.0f;
			float browseButtonWidth = ImGui::CalcTextSize(buttonText.c_str()).x + ImGui::GetStyle().FramePadding.x * 2.0f;
			float contentWidth = ImGui::GetContentRegionAvail().x;

			ImGui::PushStyleColor(ImGuiCol_Button, ExtraColors[ImGuiExtraCol_AccentButton]);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ExtraColors[ImGuiExtraCol_AccentButtonHovered]);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ExtraColors[ImGuiExtraCol_AccentButtonActive]);

			if (ImGui::Button("Vind Installatie"))
			{
				std::vector<std::string> drives =
				{
					"C",
					"D",
					"E",
					"F",
					"G",
				};

				char* gamePaths[] = { m_sSpyFox1Path, m_sSpyFox2Path, m_sSpyFox3Path };

				for (int game = 1; game <= 3; game++)
				{
					bool found = false;
					std::string gameFolder = "Spy Fox " + std::to_string(game);

					for (size_t j = 0; j < drives.size() && !found; j++)
					{
						std::string path = drives[j] + ":/Program Files (x86)/Steam/steamapps/common/" + gameFolder;

						if (fs::exists(path))
						{
							strncpy(gamePaths[game - 1], path.c_str(), PATH_MAX - 1);
							gamePaths[game - 1][PATH_MAX - 1] = '\0';
							found = true;

							switch (game)
							{
								case 1:
								{
									patcher.SetSpyFox1Path(gamePaths[game - 1]);
									break;
								}
								case 2:
								{
									patcher.SetSpyFox2Path(gamePaths[game - 1]);
									break;
								}
								case 3:
								{
									patcher.SetSpyFox3Path(gamePaths[game - 1]);
									break;
								}
							}
							break;
						}

						path = drives[j] + ":/Program Files/Steam/steamapps/common/" + gameFolder;

						if (fs::exists(path))
						{
							strncpy(gamePaths[game - 1], path.c_str(), PATH_MAX - 1);
							gamePaths[game - 1][PATH_MAX - 1] = '\0';
							found = true;

							switch (game)
							{
								case 1:
								{
									patcher.SetSpyFox1Path(gamePaths[game - 1]);
									break;
								}
								case 2:
								{
									patcher.SetSpyFox2Path(gamePaths[game - 1]);
									break;
								}
								case 3:
								{
									patcher.SetSpyFox3Path(gamePaths[game - 1]);
									break;
								}
							}
						}
					}
				}
			}

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			ImGui::Spacing();

			for (size_t i = 0; i < 3; i++)
			{
				ID3D11ShaderResourceView* icon = dx11::SVGTextureCache::Get(rows[i].m_sIconName);

				if (icon)
				{
					int svgW = dx11::SVGTextureCache::GetWidth(rows[i].m_sIconName);
					int svgH = dx11::SVGTextureCache::GetHeight(rows[i].m_sIconName);
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
					ImGui::Image((ImTextureID)icon, ImVec2(iconW, iconH));
					ImGui::SameLine();
				}

				float labelWidth = contentWidth - browseButtonWidth - ImGui::GetStyle().ItemSpacing.x;

				ImGui::PushFont(GetImGuiSystem().GetBoldFont());
				ImGui::Text("%s", rows[i].m_sLabel.c_str());
				ImGui::PopFont();

				ImGui::SetNextItemWidth(labelWidth);
				if (ImGui::InputTextWithHint(FormatId("", INPUT_ID, "FOLDER_PATH", std::to_string(i)).c_str(), rows[i].m_sHint.c_str(), rows[i].m_sPath, PATH_MAX))
				{
					switch (i)
					{
						case 0:
						{
							patcher.SetSpyFox1Path(rows[i].m_sPath);
							break;
						}
						case 1:
						{
							patcher.SetSpyFox2Path(rows[i].m_sPath);
							break;
						}
						case 2:
						{
							patcher.SetSpyFox3Path(rows[i].m_sPath);
							break;
						}
					}
				}

				ImGui::SameLine();

				if (ImGui::Button(FormatId(buttonText, BUTTON_ID, "FOLDER_BROWSE_BUTTON", std::to_string(i)).c_str()))
				{
					fs::path selectedPath;
					if (file::PickContainer(selectedPath))
					{
						strncpy(rows[i].m_sPath, selectedPath.string().c_str(), PATH_MAX - 1);
						rows[i].m_sPath[PATH_MAX - 1] = '\0';
						switch (i)
						{
							case 0:
							{
								patcher.SetSpyFox1Path(selectedPath);
								break;
							}
							case 1:
							{
								patcher.SetSpyFox2Path(selectedPath);
								break;
							}
							case 2:
							{
								patcher.SetSpyFox3Path(selectedPath);
								break;
							}
						}
					}
				}

				ImGui::Spacing();
			}

			ImGui::Spacing();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(16, 16));

			ImGui::PushStyleColor(ImGuiCol_Button, ExtraColors[ImGuiExtraCol_AccentButton]);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ExtraColors[ImGuiExtraCol_AccentButtonHovered]);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ExtraColors[ImGuiExtraCol_AccentButtonActive]);

			std::string patchButtonText = std::string(icon::ICON_PATCH) + MAIN_WINDOW_PATCH_BUTTON_TEXT;
			std::string annulerenButtonText = std::string(icon::ICON_CANCEL) + MAIN_WINDOW_PATCH_BUTTON_CANCEL;
			float buttonWidth = ImGui::CalcTextSize(patchButtonText.c_str()).x + ImGui::GetStyle().FramePadding.x * 2.0f;
			float cancelWidth = ImGui::CalcTextSize(annulerenButtonText.c_str()).x + ImGui::GetStyle().FramePadding.x * 2.0f;
			float totalButtonsWidth = buttonWidth + cancelWidth + ImGui::GetStyle().ItemSpacing.x;

			ImGui::SetCursorPosX((contentWidth - totalButtonsWidth) * 0.5f);

			if (ImGui::Button(FormatId(patchButtonText, BUTTON_ID, "PATCH").c_str()))
			{
				if (patcher.GetSpyFox1Path().empty() || patcher.GetSpyFox2Path().empty() || patcher.GetSpyFox3Path().empty())
				{
					m_bShowMissingFoldersPopup = true;
				}
				else
				{
					patcher.ApplyPatch();
				}
			}

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ExtraColors[ImGuiExtraCol_CancelButton]);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ExtraColors[ImGuiExtraCol_CancelButtonHovered]);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ExtraColors[ImGuiExtraCol_CancelButtonActive]);

			if (ImGui::Button(FormatId(annulerenButtonText, BUTTON_ID, "CANCEL").c_str()))
			{
				PostQuitMessage(0);
			}

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			ImGui::PopStyleVar();

			ImGui::PopStyleVar(2);
		}
		ImGui::EndChild();

		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImVec2 displaySize = ImGui::GetIO().DisplaySize;
		ImVec2 popupSize(displaySize.x * 0.75f, displaySize.y * 0.5f);

		if (m_bShowMissingFoldersPopup)
		{
			ImGui::OpenPopup(FormatId("MissingFolders", POPUP_WINDOW_ID).c_str());
			m_bShowMissingFoldersPopup = false;
		}

		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(popupSize, ImGuiCond_Appearing);

		if (ImGui::BeginPopupModal(FormatId("MissingFolders", POPUP_WINDOW_ID).c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
		{
			ImGui::TextWrapped(MAIN_WINDOW_MISSING_FOLDERS_MESSAGE);

			ImGui::Spacing();

			float buttonWidth = 120.0f;
			ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) * 0.5f);
			if (ImGui::Button("OK", ImVec2(buttonWidth, 0)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		if (m_bShowFailedPopup)
		{
			ImGui::OpenPopup(FormatId("FailedPatch", POPUP_WINDOW_ID).c_str());
			m_bShowFailedPopup = false;
		}

		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(popupSize, ImGuiCond_Appearing);

		if (ImGui::BeginPopupModal(FormatId("FailedPatch", POPUP_WINDOW_ID).c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
		{
			ImGui::TextWrapped(MAIN_WINDOW_PATCH_FAILED_MESSAGE);

			ImGui::Spacing();

			ImGui::TextWrapped("\"%s\"", patch::GetPatcher().GetFailReason().c_str());

			ImGui::Spacing();

			float buttonWidth = 120.0f;
			ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) * 0.5f);
			if (ImGui::Button("OK", ImVec2(buttonWidth, 0)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}