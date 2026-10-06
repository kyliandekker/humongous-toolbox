#pragma once

#include <imgui/windows/BaseWindow.h>

#include <d3d11.h>
#include <string>
#include <vector>

#include <htb_lib_win32/audio/AudioPlayer.h>

namespace htb::imgui
{
	//======================================================================================
	// PatchingWindow
	//======================================================================================
	/// <summary>
	/// The window that handles the patching state.
	/// </summary>
	class PatchingWindow : public ImGui::BaseWindow
	{
	public:
		/// <summary>
		/// Constructs the patching window.
		/// </summary>
		PatchingWindow();

		/// <summary>
		/// Renders the patching window content.
		/// </summary>
		void Update() override;

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

		ID3D11ShaderResourceView* m_pCurrentBackground = nullptr;
		size_t m_iCurrentBackground = 0;
		double m_fLastSwitchTime = 0.0;
		double m_fSwitchInterval = 5.0;
		float m_fDisplayProgress = 0.0f;

		audio::AudioPlayer m_AudioPlayer;
	};
}
