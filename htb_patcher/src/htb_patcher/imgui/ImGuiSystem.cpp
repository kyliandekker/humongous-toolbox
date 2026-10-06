#include "ImGuiSystem.h"

// external
#include <imgui/backends/imgui_impl_win32.h>
#include <imgui/backends/imgui_impl_dx11.h>
#include <imgui/imgui_internal.h>
#include <imgui/font_arial.h>
#include <imgui/windows/BaseWindow.h>
#include <implot.h>

#include <htb_lib_win32/dx11/DX11System.h>
#include <htb_lib_win32/win32/Win32Window.h>

#include "htb_patcher/imgui/Theme.h"
#include "htb_patcher/imgui/windows/MainWindow.h"
#include "htb_patcher/imgui/windows/PatchingWindow.h"
#include "htb_patcher/imgui/windows/FinishedWindow.h"
#include "htb_patcher/imgui/font_icon_patch.h"
#include "htb_patcher/Patcher.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace htb::imgui
{
	//---------------------------------------------------------------------
	// ImGuiSystem
	//---------------------------------------------------------------------
	ImGuiSystem& GetImGuiSystem()
	{
		static ImGuiSystem system;
		return system;
	}

	//---------------------------------------------------------------------
	ImGuiSystem::ImGuiSystem()
	{}

	//---------------------------------------------------------------------
	bool ImGuiSystem::Initialize()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = m_sIniPath.c_str();

		if (!CreateContextWin32() || !CreateContextDX11())
		{
			return false;
		}
		ImPlot::CreateContext();

		CreateImGui();

		win32::Win32Window& window = win32::GetWin32Window();
		RECT rc;
		GetClientRect(window.GetHandle(), &rc);
		Resize(rc.right - rc.left, rc.bottom - rc.top);

		m_pMainWindow = std::make_unique<MainWindow>();
		m_pPatchingWindow = std::make_unique<PatchingWindow>();
		m_pPatchingWindow->Close();
		m_pFinishedWindow = std::make_unique<FinishedWindow>();
		m_pFinishedWindow->Close();

		patch::GetPatcher().GetOnPatchStateChanged() += std::bind(&ImGuiSystem::OnPatchStateChanged, this, std::placeholders::_1);
		SetIniPath(patch::GetPatcher().GetSavePath());

		InitializeWindows();

		return true;
	}

	//---------------------------------------------------------------------
	bool ImGuiSystem::InitializeWindows()
	{
		m_pMainWindow->Initialize();
		m_pPatchingWindow->Initialize();
		m_pFinishedWindow->Initialize();

		return true;
	}

	//---------------------------------------------------------------------
	bool ImGuiSystem::Destroy()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		return true;
	}

	//---------------------------------------------------------------------
	bool ImGuiSystem::CreateContextWin32()
	{
		win32::Win32Window& window = win32::GetWin32Window();
		if (!ImGui_ImplWin32_Init(window.GetHandle()))
		{
			return false;
		}

		return true;
	}

	//---------------------------------------------------------------------
	bool ImGuiSystem::CreateContextDX11()
	{
		dx11::DX11System& dx11System = dx11::GetDX11System();

		if (!ImGui_ImplDX11_Init(dx11System.GetDevice(), dx11System.GetDeviceContext()))
		{
			return true;
		}

		return true;
	}

	//---------------------------------------------------------------------
	void ImGuiSystem::CreateImGui()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls

		(void) io;

		win32::Win32Window& window = win32::GetWin32Window();

		ImGui_ImplWin32_EnableDpiAwareness();
		float mainScale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

		m_fFontSize = 15.0f;

		// setup Dear ImGui style
		ImGui::StyleColorsDark();

		ImFontConfig font_config_default;
		font_config_default.FontDataOwnedByAtlas = false;
		m_pDefaultFont = io.Fonts->AddFontFromMemoryTTF(&font::arial, sizeof(font::arial), m_fFontSize, &font_config_default);

		constexpr ImWchar icons_ranges_b[] = { icon::FONT_START, icon::FONT_END, 0 };
		ImFontConfig icons_config_m;
		icons_config_m.MergeMode = true;
		icons_config_m.PixelSnapH = true;
		icons_config_m.FontDataOwnedByAtlas = false;
		m_pIconFont = io.Fonts->AddFontFromMemoryTTF(&icon::ICON, sizeof(icon::ICON), m_fFontSize, &icons_config_m, icons_ranges_b);

		ImFontConfig font_config_bold;
		font_config_bold.FontDataOwnedByAtlas = false;
		m_pBoldFont = io.Fonts->AddFontFromMemoryTTF(&font::arialBold, sizeof(font::arialBold), m_fFontSize, &font_config_bold);

		io.Fonts->Build();

		ApplyTheme();

		ImGuiStyle& style = ImGui::GetStyle();
		style.ScaleAllSizes(mainScale);
		style.FontScaleDpi = mainScale;
	}

	//---------------------------------------------------------------------
	void ImGuiSystem::OnPatchStateChanged(patch::PatchState a_ePatchState)
	{
		switch (a_ePatchState)
		{
			case patch::PatchState::NONE:
			{
				m_pMainWindow->Open();
				m_pPatchingWindow->Close();
				m_pFinishedWindow->Close();
				break;
			}
			case patch::PatchState::FAILED:
			{
				static_cast<MainWindow*>(m_pMainWindow.get())->ShowFailedPopup();
				m_pMainWindow->Open();
				m_pPatchingWindow->Close();
				m_pFinishedWindow->Close();
				break;
			}
			case patch::PatchState::PATCHING:
			{
				m_pMainWindow->Close();
				m_pPatchingWindow->Open();
				m_pFinishedWindow->Close();
				break;
			}
			case patch::PatchState::FINALIZED:
			{
				m_pMainWindow->Close();
				m_pPatchingWindow->Close();
				m_pFinishedWindow->Open();
				break;
			}
		}
	}

	//---------------------------------------------------------------------
	void ImGuiSystem::Resize(uint32_t a_iWidth, uint32_t a_iHeight)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize.x = static_cast<float>(a_iWidth);
		io.DisplaySize.y = static_cast<float>(a_iHeight);
	}

	//---------------------------------------------------------------------
	void ImGuiSystem::Render()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		const ImGuiIO& io = ImGui::GetIO();

		m_pMainWindow->Render();
		m_pPatchingWindow->Render();
		m_pFinishedWindow->Render();

		ImGui::EndFrame();
		ImGui::Render();

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();

			// Pump Win32 messages for ImGui secondary viewport windows that were just
			// created/updated by UpdatePlatformWindows(), so that window state (size,
			// position, visibility) is synchronized before the first render.
			MSG msg = {};
			while (::PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessageW(&msg);
			}

			ImGui::RenderPlatformWindowsDefault();
		}
	}

	//---------------------------------------------------------------------
	void ImGuiSystem::UpdateMouseCursor()
	{
		// Let Win32 own cursor on resize borders (same bw as Window::WM_NCHITTEST)
		{
			HWND hwnd = nullptr;
			if (ImGui::GetMainViewport())
			{
				hwnd = (HWND)ImGui::GetMainViewport()->PlatformHandleRaw;
			}
			if (!hwnd)
			{
				hwnd = ::GetForegroundWindow();
			}
			if (hwnd && !::IsZoomed(hwnd))
			{
				POINT p; ::GetCursorPos(&p);
				RECT rc; ::GetWindowRect(hwnd, &rc);
				const int bx = ::GetSystemMetrics(SM_CXFRAME) + ::GetSystemMetrics(SM_CXPADDEDBORDER);
				const int by = ::GetSystemMetrics(SM_CYFRAME) + ::GetSystemMetrics(SM_CXPADDEDBORDER);
				const int bw = max(max(bx, by) + 4, 12);
				const bool onBorder = (p.x < rc.left + bw) || (p.x >= rc.right - bw) || (p.y < rc.top + bw) || (p.y >= rc.bottom - bw);
				if (onBorder)
				{
					return;
				}
			}
		}

		bool s_IsHovering = ImGui::IsAnyItemHovered();
		if (!s_IsHovering)
		{
			return;
		}

		if (s_IsHovering && ImGui::GetMouseCursor() == ImGuiMouseCursor_Arrow)
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		}

		LPTSTR win32_cursor = IDC_ARROW;
		ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
		switch (imgui_cursor)
		{
			case ImGuiMouseCursor_TextInput:
			{
				win32_cursor = IDC_IBEAM;
				break;
			}
			case ImGuiMouseCursor_ResizeAll:
			{
				win32_cursor = IDC_SIZEALL;
				break;
			}
			case ImGuiMouseCursor_ResizeNS:
			{
				win32_cursor = IDC_SIZENS;
				break;
			}
			case ImGuiMouseCursor_ResizeEW:
			{
				win32_cursor = IDC_SIZEWE;
				break;
			}
			case ImGuiMouseCursor_ResizeNESW:
			{
				win32_cursor = IDC_SIZENESW;
				break;
			}
			case ImGuiMouseCursor_ResizeNWSE:
			{
				win32_cursor = IDC_SIZENWSE;
				break;
			}
			case ImGuiMouseCursor_Hand:
			{
				win32_cursor = IDC_HAND;
				break;
			}
			case ImGuiMouseCursor_NotAllowed:
			{
				win32_cursor = IDC_NO;
				break;
			}
			default:
			{
				win32_cursor = IDC_ARROW;
				break;
			}
		}

		// Set the system cursor using Win32 API
		::SetCursor(LoadCursor(NULL, win32_cursor));
	}

	//---------------------------------------------------------------------
	void ImGuiSystem::SetIniPath(const fs::path& a_sPath)
	{
		m_sIniPath = a_sPath.string() + "/imgui.ini";
	}

	//---------------------------------------------------------------------
	LRESULT ImGuiSystem::WndProcHandler(HWND a_hWnd, UINT a_iMsg, WPARAM a_wParam, LPARAM a_lParam)
	{
		return ImGui_ImplWin32_WndProcHandler(a_hWnd, a_iMsg, a_wParam, a_lParam);
	}

	//---------------------------------------------------------------------
	ImFont* ImGuiSystem::GetBoldFont()
	{
		return m_pBoldFont;
	}

	//---------------------------------------------------------------------
	ImFont* ImGuiSystem::GetDefaultFont()
	{
		return m_pDefaultFont;
	}
}