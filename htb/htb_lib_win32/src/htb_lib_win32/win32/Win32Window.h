#ifdef WIN32

#pragma once

#include <htb_lib_gui/graphics/IWindow.h>

#include <string>

#include <htb_lib_win32/win32/WINPCH.h>

namespace htb::win32
{
	class Win32Window;
	extern Win32Window& GetWin32Window();

	//======================================================================================
	// Win32Window
	//======================================================================================
	/// <summary>
	/// Represents a Win32 application window, handling class registration,
	/// creation, message pumping and resize tracking.
	/// </summary>
	class Win32Window : public graphics::IWindow
	{
	public:
		/// <summary>
		/// Signature for an optional message hook, called before default handling.
		/// </summary>
		/// <returns>Non-zero if the message was handled and default processing should be skipped.</returns>
		using MessageHook = LRESULT(WINAPI*)(HWND, UINT, WPARAM, LPARAM);

		/// <summary>
		/// Constructs a Win32 window.
		/// </summary>
		Win32Window();

		Win32Window(const Win32Window&) = delete;
		Win32Window& operator=(const Win32Window&) = delete;

		/// <summary>
		/// Registers the window class and creates the window.
		/// </summary>
		/// <param name="a_HInstance">The application instance handle.</param>
		/// <param name="a_iWidth">The desired window width in pixels.</param>
		/// <param name="a_iHeight">The desired window height in pixels.</param>
		/// <param name="a_sTitle">The window title.</param>
		/// <returns>True if the window was created successfully, otherwise false.</returns>
		bool SetData(HINSTANCE a_HInstance, int a_iWidth, int a_iHeight, const wchar_t* a_sTitle);

		bool ProcessMessages() override;

		void Show(int a_iShowCmd) override;
		void Minimize() override;
		void Maximize() override;
		void Close() override;

		bool IsMaximized() const override;

		/// <summary>
		/// Installs an optional hook invoked for every window message before default handling.
		/// </summary>
		/// <param name="a_Hook">The hook to invoke, or nullptr to clear it.</param>
		void SetMessageHook(MessageHook a_Hook);

		/// <summary>
		/// Retrieves the native window handle.
		/// </summary>
		HWND GetHandle() const;
	private:
		/// <summary>
		/// Called once on the thread to perform initialization steps.
		/// Must be implemented by subclasses.
		/// </summary>
		/// <returns>True if the initialization was successful, otherwise false.</returns>
		bool OnInitialized() override;

		/// <summary>
		/// Called once the system is stopping, to release resources.
		/// Must be implemented by subclasses.
		/// </summary>
		bool OnDestroyed() override;

		static LRESULT CALLBACK WndProc(HWND a_HWnd, UINT a_iMsg, WPARAM a_WParam, LPARAM a_LParam);
		LRESULT HandleMessage(UINT a_iMsg, WPARAM a_WParam, LPARAM a_LParam);

		HWND m_HWnd = nullptr;
		HINSTANCE m_HInstance = nullptr;
		std::wstring m_wsClassName;
		std::wstring m_wsTitle;
		MessageHook m_MessageHook = nullptr;
	};
}

#endif // WIN32