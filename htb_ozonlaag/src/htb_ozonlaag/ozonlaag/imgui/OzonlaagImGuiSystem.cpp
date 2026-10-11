#include "./OzonlaagImGuiSystem.h"

#include <htb_lib_patcher/patch/Patcher.h>
#include <htb_lib_patcher/patch/PatcherState.h>

#include "htb_ozonlaag/ozonlaag/imgui/windows/FinishedWindow.h"
#include "htb_ozonlaag/ozonlaag/imgui/windows/MainWindow.h"
#include "htb_ozonlaag/ozonlaag/imgui/windows/PatchingWindow.h"

namespace htb::imgui
{
	//======================================================================================
	// OzonlaagImGuiSystem
	//======================================================================================
	OzonlaagImGuiSystem::OzonlaagImGuiSystem() : ImGuiSystem()
	{}

	//======================================================================================
	OzonlaagImGuiSystem::~OzonlaagImGuiSystem() = default;

	//======================================================================================
	void OzonlaagImGuiSystem::OnInitialized()
	{
		patch::GetPatcher().GetPatcherState().OnChanged() += std::bind(&OzonlaagImGuiSystem::OnPatchStateChanged, this, std::placeholders::_1, std::placeholders::_2);

		m_pFinishedWindow = std::make_unique<FinishedWindow>();
		m_pFinishedWindow->Initialize();

		m_pMainWindow = std::make_unique<MainWindow>();
		m_pMainWindow->Initialize();

		m_pPatchingWindow = std::make_unique<PatchingWindow>();
		m_pPatchingWindow->Initialize();
	}

	//======================================================================================
	void OzonlaagImGuiSystem::OnRender()
	{
		m_pFinishedWindow->Render();
		m_pMainWindow->Render();
		m_pPatchingWindow->Render();
	}

	//======================================================================================
	void OzonlaagImGuiSystem::OnPatchStateChanged(patch::EPatcherState a_ePrevState, patch::EPatcherState a_eNewState)
	{
		switch (a_eNewState)
		{
			case patch::EPatcherState::INACTIVE:
			case patch::EPatcherState::FAILED:
			{
				if (a_eNewState == patch::EPatcherState::FAILED)
				{
					m_pMainWindow->ShowFailedPopup(patch::GetPatcher().GetStatusMessage());
				}
				m_pMainWindow->Open();
				m_pPatchingWindow->Close();
				m_pFinishedWindow->Close();
				break;
			}
			case patch::EPatcherState::RUNNING:
			{
				m_pMainWindow->Close();
				m_pPatchingWindow->Open();
				m_pFinishedWindow->Close();
				break;
			}
			case patch::EPatcherState::FINISHED:
			{
				m_pMainWindow->Close();
				m_pPatchingWindow->Close();
				m_pFinishedWindow->Open();
				break;
			}
		}
	}
}