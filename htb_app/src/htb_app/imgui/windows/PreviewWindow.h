#pragma once

#include "htb_app/imgui/windows/HEBaseWindow.h"

#include <htb_lib_win32/audio/AudioPlayer.h>

struct ID3D11ShaderResourceView;

namespace htb::resources
{
	class Resource;
}
namespace htb::imgui
{
	//---------------------------------------------------------------------
	// PreviewWindow
	//---------------------------------------------------------------------
	/// <summary>
	/// A window for previewing image resources with zoom controls and metadata.
	/// </summary>
	class PreviewWindow : public LoggerDependentWindow
	{
	public:
		/// <summary>
		/// Constructs a preview window.
		/// </summary>
		PreviewWindow();

		/// <summary>
		/// Renders the preview window content.
		/// </summary>
		void Update() override;
	private:
		/// <summary>
		/// Initializes all behaviours and values for the window.
		/// </summary>
		/// <returns>True if initialization is successful, otherwise false.</returns>
		bool OnInitialized() override;

		void OnSelectedResourceChanged(resources::Resource* oldResource, resources::Resource* newResource);

		float m_fZoom = 1.0f;
		ImVec2 m_vPan = ImVec2(0.0f, 0.0f);

		static const int s_iPresetCount = 6;
		static const float s_aPresets[s_iPresetCount];
		static const char* s_aPresetLabels[s_iPresetCount];

		bool m_bShowCheckerboard = true;
		bool m_bShowBackground = false;

		ID3D11ShaderResourceView* m_pBackgroundSRV = nullptr;
		uint16_t m_iBackgroundWidth = 0;
		uint16_t m_iBackgroundHeight = 0;

		float m_fAudioVolume = 0.75f;

		audio::AudioPlayer m_AudioPlayer;
		uint16_t m_iAudioSampleRate = 0;

		void RenderCheckerboard(const ImVec2& a_vMin, const ImVec2& a_vMax);
		void RenderImageControlsBar();
		void RenderImage();

		void RenderAudio();
		void RenderSoundControlsBar();
		void RenderInfo();
	};
}
