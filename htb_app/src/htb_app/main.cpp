#include <imgui/imgui.h>
#include <imgui/implot.h>
#include <imgui/backends/imgui_impl_win32.h>
#include <imgui/backends/imgui_impl_dx11.h>

#include <htb_lib/core/Log.h>
#include <htb_lib/file/file.h>

#include <htb_lib_win32/dx11/DX11System.h>
#include <htb_lib_win32/win32/winfile.h>
#include <htb_lib_win32/win32/Win32Window.h>
#include <htb_lib_win32/dx11/SVGTextureCache.h>

#include "htb_app/editor/Workspace.h"
#include "htb_app/imgui/ImGuiSystem.h"
#include "htb_app/ui/AppLogger.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static void HelibLogCallback(htb::core::ELogLevel a_Level, const std::string& a_sMessage)
{
	htb::logger::GetLogEvent().invoke(a_Level, a_sMessage);

	const char* prefix = "";
	switch (a_Level)
	{
		case htb::core::ELogLevel::SUCCESS: prefix = "[OK]     "; break;
		case htb::core::ELogLevel::_ERROR:   prefix = "[FAIL]   "; break;
		case htb::core::ELogLevel::WARNING: prefix = "[WARN]   "; break;
		case htb::core::ELogLevel::INFO:    prefix = "[INFO]   "; break;
	}
	printf("%s%s\n", prefix, a_sMessage.c_str());
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	htb::core::InitializeLog();
	htb::core::SetLogCallback(HelibLogCallback);

	ImGui_ImplWin32_EnableDpiAwareness();
	float mainScale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

	htb::win32::Win32Window& window = htb::win32::GetWin32Window();
	std::string appName = htb::GetWorkspace().GetAppName();
	std::wstring wAppName(appName.begin(), appName.end());
	window.SetData(hInstance, (int) (1280 * mainScale), (int) (800 * mainScale), wAppName.c_str());
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

	ImColor clearColor = IM_COL32(21, 26, 36, 255);

	htb::GetWorkspace().Initialize();

	fs::path appDataPath = htb::file::GetAppDataPath().string() + "/htb";
	htb::file::CreateFolder(appDataPath);

	htb::imgui::GetImGuiSystem().SetIniPath(appDataPath.string());
	htb::imgui::GetImGuiSystem().Initialize();

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
			htb::dx11::GetDX11System().Resize(width, height);
		}

		//if (window.HasDroppedFile())
		//{
		//	std::string path = window.ConsumeDroppedFile();
		//	POINT pt = window.GetDroppedFilePosition();
		//	htb::imgui::GetImGuiSystem().SetDroppedFile(path, ImVec2(static_cast<float>(pt.x), static_cast<float>(pt.y)));
		//}
		const float clear_color_with_alpha[4] = { clearColor.Value.x * clearColor.Value.w, clearColor.Value.y * clearColor.Value.w, clearColor.Value.z * clearColor.Value.w, clearColor.Value.w };
		htb::dx11::GetDX11System().BeginFrame(clear_color_with_alpha);

		htb::imgui::GetImGuiSystem().Render();

		htb::imgui::GetImGuiSystem().UpdateMouseCursor();

		htb::dx11::GetDX11System().EndFrame(1);
	}

	htb::imgui::GetImGuiSystem().Destroy();

	htb::dx11::SVGTextureCache::Shutdown();
	htb::dx11::GetDX11System().Destroy();
	window.Destroy();

	htb::core::DestroyLog();

	return 0;
}