#pragma once

#include "htb_ozonlaag/imgui/ImGuiSystem.h"

#include <memory>

namespace htb::patch
{
	enum class EPatcherState;
}
namespace htb::imgui
{
	class FinishedWindow;
	class MainWindow;
	class PatchingWindow;

	class OzonlaagImGuiSystem : public ImGuiSystem
	{
	public:
		OzonlaagImGuiSystem();
		~OzonlaagImGuiSystem() override;
	private:
		/// <summary>
		/// Called when the window is successfully initialized. Meant to initialize windows, modals, etc.
		/// </summary>
		void OnInitialized() override;

		/// <summary>
		/// Renders the windows.
		/// </summary>
		void OnRender() override;

		/// <summary>
		/// Callback for when the patch state has changed.
		/// </summary>
		/// <param name="a_ePrevState">The previous state of the patch state.</param>
		/// <param name="a_eNewState">The new state.</param>
		void OnPatchStateChanged(patch::EPatcherState a_ePrevState, patch::EPatcherState a_eNewState);

		std::unique_ptr<FinishedWindow> m_pFinishedWindow;
		std::unique_ptr<MainWindow> m_pMainWindow;
		std::unique_ptr<PatchingWindow> m_pPatchingWindow;
	};
}