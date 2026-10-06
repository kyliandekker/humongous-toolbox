#pragma once

// external
#include <imgui/imgui.h>
#include <wtypes.h>
#include <memory>
#include <string>

#include <htb_lib/file/FILEPCH.h>

namespace ImGui
{
	class BaseWindow;
}
namespace htb::patch
{
	enum class PatchState;
}
namespace htb::dx11
{
	class DX11System;
}
namespace htb::imgui
{
	class ImGuiSystem;
	extern ImGuiSystem& GetImGuiSystem();

	enum ImGuiExtraCol_
	{
		ImGuiExtraCol_AccentButton,
		ImGuiExtraCol_AccentButtonHovered,
		ImGuiExtraCol_AccentButtonActive,
		ImGuiExtraCol_CancelButton,
		ImGuiExtraCol_CancelButtonHovered,
		ImGuiExtraCol_CancelButtonActive,
		ImGuiExtraCol_TabInactive,
		ImGuiExtraCol_COUNT
	};
	inline ImVec4 ExtraColors[ImGuiExtraCol_COUNT];

	class BottomToolbar;

	//======================================================================================
	// ImGuiSystem
	//======================================================================================
	class ImGuiSystem
	{
	public:
		/// <summary>
		/// Constructs the ImGui system.
		/// </summary>
		ImGuiSystem();

		/// <summary>
		/// Initializes the imgui system.
		/// </summary>
		bool Initialize();

		/// <summary>
		/// Initializes the windows.
		/// </summary>
		bool InitializeWindows();

		/// <summary>
		/// Destroys all imgui assets.
		/// </summary>
		bool Destroy();

		/// <summary>
		/// Updates ImGui display size to match the new window dimensions.
		/// </summary>
		/// <param name="a_iWidth">The new client area width.</param>
		/// <param name="a_iHeight">The new client area height.</param>
		void Resize(uint32_t a_iWidth, uint32_t a_iHeight);

		/// <summary>
		/// Renders the ImGui frame.
		/// </summary>
		void Render();
	private:
		/// <summary>
		/// Handles Windows messages for the editor's window.
		/// </summary>
		/// <param name="a_hWnd">Handle to the window.</param>
		/// <param name="a_iMsg">Message identifier.</param>
		/// <param name="a_wParam">Additional message information (WPARAM).</param>
		/// <param name="a_lParam">Additional message information (LPARAM).</param>
		/// <returns>The result of the message processing.</returns>
		LRESULT CALLBACK WndProcHandler(HWND a_hWnd, UINT a_iMsg, WPARAM a_wParam, LPARAM a_lParam);

		/// <summary>
		/// Creates the ImGui context for Win32.
		/// </summary>
		/// <returns>True if the context creation succeeds, otherwise false.</returns>
		bool CreateContextWin32();

		/// <summary>
		/// Creates the ImGui context for DirectX 11.
		/// </summary>
		/// <returns>True if the context creation succeeds, otherwise false.</returns>
		bool CreateContextDX11();

		/// <summary>
		/// Initializes the ImGui UI components.
		/// </summary>
		void CreateImGui();
	public:
		/// <summary>
		/// Callback for when the patcher state has changed.
		/// </summary>
		void OnPatchStateChanged(patch::PatchState a_ePatchState);

		/// <summary>
		/// Updates the mouse cursor when hovering, clicking, etc.
		/// </summary>
		void UpdateMouseCursor();

		/// <summary>
		/// Retrieves the default font.
		/// </summary>
		/// <returns>A pointer to the ImFont.</returns>
		ImFont* GetDefaultFont();

		/// <summary>
		/// Retrieves the bold font.
		/// </summary>
		/// <returns>A pointer to the ImFont.</returns>
		ImFont* GetBoldFont();

		/// <summary>
		/// Sets the path for the ImGui INI file.
		/// </summary>
		/// <param name="a_sPath">The directory path where the INI file will be stored.</param>
		void SetIniPath(const fs::path& a_sPath);
	private:
		ImFont* m_pDefaultFont = nullptr;
		ImFont* m_pBoldFont = nullptr;
		ImFont* m_pIconFont = nullptr;

		float m_fFontSize = 0.0f; /// Default font size for ImGui.

		std::unique_ptr<ImGui::BaseWindow> m_pMainWindow;
		std::unique_ptr<ImGui::BaseWindow> m_pPatchingWindow;
		std::unique_ptr<ImGui::BaseWindow> m_pFinishedWindow;

		std::string m_sIniPath;

		friend htb::dx11::DX11System;
	};
}
