#pragma once

#include "imgui/windows/BaseWindow.h"

namespace htb::imgui
{
	//======================================================================================
	// PatchingWindow
	//======================================================================================
	/// <summary>
	/// The window that shows the patching progress.
	/// </summary>
	class PatchingWindow : public ImGui::BaseWindow
	{
	public:
		/// <summary>
		/// Constructs the main window.
		/// </summary>
		PatchingWindow();

		/// <summary>
		/// Opens the window.
		/// </summary>
		void Open() override;
	private:
		/// <summary>
		/// Initializes all behaviours and values for the window.
		/// </summary>
		/// <returns>True if initialization is successful, otherwise false.</returns>
		bool OnInitialized() override;

		/// <summary>
		/// Renders the preview window content.
		/// </summary>
		void OnUpdate() override;

		/// <summary>
		/// Callback for the patching progress.
		/// </summary>
		/// <param name="a_fOldValue">The old value of the progress.</param>
		/// <param name="a_fNewValue">The current progress.</param>
		void OnPatcherProgressChanged(float a_fOldValue, float a_fNewValue);

        /// <summary>
        /// Callback for the patching status message.
        /// </summary>
        /// <param name="a_sOoldMessage">The old status message.</param>
        /// <param name="a_sNewMessage">The new status message.</param>
        void OnStatusMessageChanged(const std::string& a_sOoldMessage, const std::string& a_sNewMessage);

		float m_fDisplayProgress = 0;
		float m_fProgress = 0;

		std::string m_sStatusMessage;
	};
}