#pragma once

#include "imgui/windows/BaseWindow.h"

#include <d3d11.h>
#include <string>
#include <vector>

#include <htb_lib_win32/audio/AudioPlayer.h>

namespace htb::imgui
{
	//---------------------------------------------------------------------
	// FinishedWindow
	//---------------------------------------------------------------------
	/// <summary>
	/// The window that shows the patching result.
	/// </summary>
	class FinishedWindow : public ImGui::BaseWindow
	{
	public:
		/// <summary>
		/// Constructs the finished window.
		/// </summary>
		FinishedWindow();

		/// <summary>
		/// Renders the finished window content.
		/// </summary>
		void Update() override;

		/// <summary>
		/// Opens the window.
		/// </summary>
		void Open() override;

		/// <summary>
		/// Closes the window.
		/// </summary>
		void Close() override;
	private:
		/// <summary>
		/// Initializes all behaviours and values for the window.
		/// </summary>
		/// <returns>True if initialization is successful, otherwise false.</returns>
		bool OnInitialized() override;

		ID3D11ShaderResourceView* m_pLogoTexture = nullptr;
		int m_iLogoWidth = 0;
		int m_iLogoHeight = 0;

		ID3D11ShaderResourceView* m_pDefaultBackground = nullptr;

		audio::AudioPlayer m_AudioPlayer;
	};
}
