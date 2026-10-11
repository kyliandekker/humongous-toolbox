#include <imgui/backends/imgui_impl_dx11.h>
#include <imgui/backends/imgui_impl_win32.h>
#include <imgui/imgui.h>
#include <imgui/implot.h>

#include <htb_lib/core/Log.h>
#include <htb_lib/file/file.h>

#include <htb_lib_win32/dx11/DX11System.h>
#include <htb_lib_win32/dx11/SVGTextureCache.h>
#include <htb_lib_win32/win32/Win32Window.h>
#include <htb_lib_win32/win32/winfile.h>

#include <htb_lib_patcher/patch/PatcherState.h>

#include "htb_ozonlaag/imgui/ImGuiSystem.h"
#include "htb_ozonlaag/ozonlaag/OzonlaagPatcher.h"
#include "htb_ozonlaag/ozonlaag/imgui/OzonlaagImGuiSystem.h"

//======================================================================================
namespace htb::patch
{
	Patcher& GetPatcher()
	{
		static ozonlaag::OzonlaagPatcher patcher;
		return patcher;
	}
}
namespace htb::imgui
{
	ImGuiSystem& GetImGuiSystem()
	{
		static OzonlaagImGuiSystem system;
		return system;
	}
}

//======================================================================================
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//======================================================================================
void OnPatchStateChanged(htb::patch::EPatcherState a_ePrevState, htb::patch::EPatcherState a_eNewState)
{
	if (a_ePrevState == htb::patch::EPatcherState::FAILED || a_ePrevState == htb::patch::EPatcherState::FINISHED)
	{
		htb::ozonlaag::OzonlaagPatcher* patcher = htb::ozonlaag::GetOzonlaagPatcher();
		patcher->Join();
	}
}

//======================================================================================
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	htb::core::InitializeLog();

	ImGui_ImplWin32_EnableDpiAwareness();
	const float mainScale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

	htb::win32::Win32Window& window = htb::win32::GetWin32Window();
	const std::string windowName = "Spy Fox 3: Operatie Ozonlaag - Patcher";
	const std::wstring wAppName(windowName.begin(), windowName.end());
	window.SetData(hInstance, (int)(400 * mainScale), (int)(700 * mainScale), wAppName.c_str());
	if (!window.Initialize())
	{
		return 1;
	}
	window.SetMessageHook(&ImGui_ImplWin32_WndProcHandler);
	HWND hwnd = window.GetHandle();

	if (!htb::dx11::GetDX11System().Initialize(hwnd))
	{
		htb::dx11::GetDX11System().Destroy();
		window.Destroy();
		return 1;
	}

	window.Show(SW_SHOWDEFAULT);

	const ImColor clearColor = IM_COL32(26, 20, 40, 255);

	const fs::path appDataPath = htb::file::GetAppDataPath();
	htb::ozonlaag::OzonlaagPatcher* patcher = htb::ozonlaag::GetOzonlaagPatcher();
	patcher->SetWorkingPath(appDataPath);
	patcher->GetPatcherState().OnChanged() += OnPatchStateChanged;
	patcher->Initialize();

	htb::imgui::ImGuiSystem& imguiSystem = htb::imgui::GetImGuiSystem();
	imguiSystem.SetIniPath(patcher->GetWorkingPath());
	imguiSystem.Initialize();

	htb::dx11::DX11System& dx11System = htb::dx11::GetDX11System();

	bool running = true;
	while (running)
	{
		running = window.ProcessMessages();
		if (!running)
		{
			break;
		}

		if (window.HasPendingResize())
		{
			uint32_t width = 0, height = 0;
			window.ConsumeResize(width, height);
			dx11System.Resize(width, height);
		}

		const float clearColorWithAlpha[4] = { clearColor.Value.x * clearColor.Value.w, clearColor.Value.y * clearColor.Value.w, clearColor.Value.z * clearColor.Value.w, clearColor.Value.w };
		dx11System.BeginFrame(clearColorWithAlpha);

		imguiSystem.Render();
		imguiSystem.UpdateMouseCursor();

		dx11System.EndFrame(1);
	}

	imguiSystem.Destroy();

	patcher->Join();

	htb::dx11::SVGTextureCache::Shutdown();
	htb::dx11::GetDX11System().Destroy();
	window.Destroy();

	htb::core::DestroyLog();

	return 0;
}