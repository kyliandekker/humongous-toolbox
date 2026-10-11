#include "./MainWindow.h"

#include <imgui_helpers.h>
#include <array>

#include <htb_lib/core/LibInfo.h>

#include <htb_lib_win32/win32/winfile.h>

#include "htb_ozonlaag/imgui/font_icon_patch.h"
#include "htb_ozonlaag/imgui/ImGuiSystem.h"
#include <htb_ozonlaag/ozonlaag/LocText.h>
#include "htb_ozonlaag/ozonlaag/OzonlaagPatcher.h"

using namespace htb::ozonlaag;

namespace htb::imgui
{
	//======================================================================================
	// MainWindow
	//======================================================================================
	MainWindow::MainWindow() : ImGui::BaseWindow(
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar, 
		"PREVIEW", 
		"MainWindow", 
		true)
	{}

	//======================================================================================
    void MainWindow::ShowFailedPopup(const std::string & a_sFailureMessage)
    {
		m_sFailureMessage = a_sFailureMessage;
		m_bShowFailedPopup = true;
	}

	//======================================================================================
	bool MainWindow::OnInitialized()
	{
		const htb::ozonlaag::OzonlaagPatcher* patcher = htb::ozonlaag::GetOzonlaagPatcher();
		if (!patcher)
		{
			return false;
		}

		const OzonlaagPatcherSettings& settings = patcher->GetSettings();
		strcpy(m_sSpyFox1Path, settings.GetSpyFox1Path().string().c_str());
		strcpy(m_sSpyFox2Path, settings.GetSpyFox2Path().string().c_str());
		strcpy(m_sSpyFox3Path, settings.GetSpyFox3Path().string().c_str());

		return true;
	}
	
	//======================================================================================
	void MainWindow::OnUpdate()
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

		bool needsSaving = false;

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

			ImGui::PushFont(GetImGuiSystem().GetBoldFont());
			ImGui::TextWrapped("%s", MAIN_WINDOW_WARNING);
			ImGui::PopFont();

			ImGui::Spacing();

			const std::string buttonText = std::string(icon::ICON_FOLDER) + MAIN_WINDOW_BROWSE;

			const float contentWidth = ImGui::GetContentRegionAvail().x;

			ImGui::PushStyleColor(ImGuiCol_Button, ExtraColors[ImGuiExtraCol_AccentButton]);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ExtraColors[ImGuiExtraCol_AccentButtonHovered]);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ExtraColors[ImGuiExtraCol_AccentButtonActive]);

			if (ImGui::Button(MAIN_WINDOW_FIND_INSTALLATION))
			{
				needsSaving |= SearchInstallation(m_sSpyFox1Path, "Spy Fox 1");
				needsSaving |= SearchInstallation(m_sSpyFox2Path, "Spy Fox 2");
				needsSaving |= SearchInstallation(m_sSpyFox3Path, "Spy Fox 3");
			}

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			ImGui::Spacing();

			needsSaving |= ShowBrowseButton(m_sSpyFox1Path, MAIN_WINDOW_INSTALL_PATH_SPYFOX_1, "SPYFOX_1");
			needsSaving |= ShowBrowseButton(m_sSpyFox2Path, MAIN_WINDOW_INSTALL_PATH_SPYFOX_2, "SPYFOX_2");
			needsSaving |= ShowBrowseButton(m_sSpyFox3Path, MAIN_WINDOW_INSTALL_PATH_SPYFOX_3, "SPYFOX_3");

			if (needsSaving)
			{
				ProcessSettings();
			}

			ImGui::Spacing();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(16, 16));

			ImGui::PushStyleColor(ImGuiCol_Button, ExtraColors[ImGuiExtraCol_AccentButton]);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ExtraColors[ImGuiExtraCol_AccentButtonHovered]);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ExtraColors[ImGuiExtraCol_AccentButtonActive]);

			const std::string patchButtonText = std::string(icon::ICON_PATCH) + MAIN_WINDOW_PATCH_BUTTON_TEXT;
			const std::string annulerenButtonText = std::string(icon::ICON_CANCEL) + MAIN_WINDOW_PATCH_BUTTON_CANCEL;
			const float buttonWidth = ImGui::CalcTextSize(patchButtonText.c_str()).x + ImGui::GetStyle().FramePadding.x * 2.0f;
			const float cancelWidth = ImGui::CalcTextSize(annulerenButtonText.c_str()).x + ImGui::GetStyle().FramePadding.x * 2.0f;
			const float totalButtonsWidth = buttonWidth + cancelWidth + ImGui::GetStyle().ItemSpacing.x;

			ImGui::SetCursorPosX((contentWidth - totalButtonsWidth) * 0.5f);

			if (ImGui::Button(FormatId(patchButtonText, BUTTON_ID, "PATCH").c_str()))
			{
				OzonlaagPatcherSettings& settings = patcher->GetSettings();
				if (settings.GetSpyFox1Path().empty() || settings.GetSpyFox2Path().empty() || settings.GetSpyFox3Path().empty())
				{
					ShowFailedPopup(MAIN_WINDOW_MISSING_FOLDERS_MESSAGE);
				}
				else
				{
					patcher->ApplyPatch();
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

			ImGui::PopStyleVar();
			ImGui::PopStyleVar();
		}
		ImGui::EndChild();

		const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
		const ImVec2 popupSize(displaySize.x * 0.75f, displaySize.y * 0.5f);

		if (m_bShowFailedPopup)
		{
			ImGui::OpenPopup(FormatId("", "FailedPatch", POPUP_WINDOW_ID).c_str());
			m_bShowFailedPopup = false;
		}

		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(popupSize, ImGuiCond_Appearing);

		if (ImGui::BeginPopupModal(FormatId(MAIN_WINDOW_POPUP_FAILED, "FailedPatch", POPUP_WINDOW_ID).c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
		{
			ImGui::TextWrapped("\"%s\"", m_sFailureMessage.c_str());

			ImGui::Spacing();

			const float buttonWidth = 120.0f;
			ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) * 0.5f);
			if (ImGui::Button("OK", ImVec2(buttonWidth, 0)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	//======================================================================================
	void MainWindow::ProcessSettings()
	{
		htb::ozonlaag::OzonlaagPatcher* patcher = htb::ozonlaag::GetOzonlaagPatcher();
		if (!patcher)
		{
			return;
		}

		OzonlaagPatcherSettings& settings = patcher->GetSettings();
		settings.SetSpyFox1Path(m_sSpyFox1Path);
		settings.SetSpyFox2Path(m_sSpyFox2Path);
		settings.SetSpyFox3Path(m_sSpyFox3Path);

		patcher->SaveSettings();
	}

	//======================================================================================
	bool MainWindow::ShowBrowseButton(char* a_sPath, const std::string& a_sName, const std::string& a_sId)
	{
		bool changed = false;

		ImGui::PushFont(GetImGuiSystem().GetBoldFont());
		ImGui::TextWrapped("%s", a_sName.c_str());
		ImGui::PopFont();

		const std::string buttonText = std::string(icon::ICON_FOLDER) + MAIN_WINDOW_BROWSE;
		const float browseButtonWidth = ImGui::CalcTextSize(buttonText.c_str()).x + ImGui::GetStyle().FramePadding.x * 2.0f;
		const float contentWidth = ImGui::GetContentRegionAvail().x;

		const float labelWidth = contentWidth - browseButtonWidth - ImGui::GetStyle().ItemSpacing.x;

		ImGui::SetNextItemWidth(labelWidth);
		if (ImGui::InputText(FormatId("", INPUT_ID, "FOLDER_PATH", a_sId).c_str(), a_sPath, PATH_MAX))
		{
			changed = true;
		}

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ExtraColors[ImGuiExtraCol_BrowseButton]);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ExtraColors[ImGuiExtraCol_BrowseButtonHovered]);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ExtraColors[ImGuiExtraCol_BrowseButtonActive]);

		if (ImGui::Button(FormatId(buttonText, BUTTON_ID, "FOLDER_BROWSE_BUTTON", a_sId).c_str()))
		{
			fs::path selectedPath;
			if (file::PickContainer(selectedPath))
			{
				strncpy(a_sPath, selectedPath.string().c_str(), PATH_MAX - 1);
				a_sPath[PATH_MAX - 1] = '\0';

				changed = true;
			}
		}

		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();

		return changed;
	}

	//======================================================================================
	constexpr std::array<const char*, 5> SEARCH_DRIVES =
	{
		"C",
		"D",
		"E",
		"F",
		"G"
	};

	//======================================================================================
	bool MainWindow::SearchInstallation(char* a_sPath, const std::string& a_sName)
	{
		bool found = false;

		fs::path
			foundPath = "";

		for (size_t driveNum = 0; driveNum < SEARCH_DRIVES.size() && !found; driveNum++)
		{
			fs::path path = fs::path(std::string(SEARCH_DRIVES[driveNum]) + ":/Program Files (x86)/Steam/steamapps/common/") / a_sName;

			if (fs::exists(path))
			{
				foundPath = path;
				break;
			}

			path = fs::path(std::string(SEARCH_DRIVES[driveNum]) + ":/Program Files/Steam/steamapps/common/") / a_sName;

			if (fs::exists(path))
			{
				foundPath = path;
				break;
			}
		}

		if (!foundPath.empty())
		{
			strcpy(a_sPath, foundPath.string().c_str());
			found = true;
		}

		return found;
	}
}