#pragma once

#include "imgui/windows/BaseWindow.h"

#include <d3d11.h>
#include <string>
#include <vector>

namespace htb::imgui
{
	//======================================================================================
	// MainWindow
	//======================================================================================
	/// <summary>
	/// The window that handles the patching state.
	/// </summary>
	class MainWindow : public ImGui::BaseWindow
	{
	public:
		/// <summary>
		/// Constructs the main window.
		/// </summary>
		MainWindow();

		/// <summary>
		/// Renders the preview window content.
		/// </summary>
		void Update() override;

		/// <summary>
		/// Shows the patch failure popup on next frame.
		/// </summary>
		void ShowFailedPopup();
	private:
		/// <summary>
		/// Initializes all behaviours and values for the window.
		/// </summary>
		/// <returns>True if initialization is successful, otherwise false.</returns>
		bool OnInitialized() override;

		ID3D11ShaderResourceView* m_pLogoTexture = nullptr;
		int m_iLogoWidth = 0;
		int m_iLogoHeight = 0;

		ID3D11ShaderResourceView* m_pBackground = nullptr;
		size_t m_iBackground = 0;

		bool m_bShowMissingFoldersPopup = false;
		bool m_bShowFailedPopup = false;

		static constexpr size_t PATH_MAX = 256;
		char m_sSpyFox1Path[PATH_MAX] = {};
		char m_sSpyFox2Path[PATH_MAX] = {};
		char m_sSpyFox3Path[PATH_MAX] = {};
	};
}