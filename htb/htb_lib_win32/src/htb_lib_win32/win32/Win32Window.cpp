#ifdef WIN32

#include "./Win32Window.h"

#include <format>

namespace htb::win32
{
	//======================================================================================
	Win32Window& GetWin32Window()
	{
		static Win32Window window;
		return window;
	}

	//======================================================================================
	// Win32Window
	//======================================================================================
	Win32Window::Win32Window() : graphics::IWindow("Window_Win32")
	{}

	//======================================================================================
	bool Win32Window::SetData(HINSTANCE a_HInstance, int a_iWidth, int a_iHeight, const wchar_t* a_sTitle)
	{
		m_HInstance = a_HInstance;

		m_wsClassName = std::format(L"{}_%08X", a_sTitle, GetCurrentProcessId());
		m_wsTitle = a_sTitle;

		m_iWidth = a_iWidth;
		m_iHeight = a_iHeight;

		return true;
	}

	//======================================================================================
	bool Win32Window::ProcessMessages()
	{
		MSG msg;
		bool running = true;

		while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);

			switch (msg.message)
			{
				case WM_QUIT:
				{
					running = false;
					break;
				}
			}
		}

		return running;
	}

	//======================================================================================
	void Win32Window::Show(int a_iShowCmd)
	{
		if (!m_HWnd)
		{
			return;
		}

		::ShowWindow(m_HWnd, a_iShowCmd);
		::UpdateWindow(m_HWnd);
	}

	//======================================================================================
	void Win32Window::Minimize()
	{
		::ShowWindow(m_HWnd, SW_MINIMIZE);
	}

	//======================================================================================
	void Win32Window::Maximize()
	{
		::ShowWindow(m_HWnd, IsZoomed(m_HWnd) ? SW_RESTORE : SW_MAXIMIZE);
	}

	//======================================================================================
	void Win32Window::Close()
	{
		::PostMessage(m_HWnd, WM_CLOSE, 0, 0);
	}

	//======================================================================================
	bool Win32Window::IsMaximized() const
	{
		return ::IsZoomed(m_HWnd) != FALSE;
	}

	//======================================================================================
	void Win32Window::SetMessageHook(MessageHook a_Hook)
	{
		m_MessageHook = a_Hook;
	}

	//======================================================================================
	HWND Win32Window::GetHandle() const
	{
		return m_HWnd;
	}

	//======================================================================================
	bool Win32Window::OnInitialized()
	{
		const DWORD style = WS_OVERLAPPEDWINDOW;

		const WNDCLASSEXW wc =
		{
			sizeof(wc),
			CS_CLASSDC,
			&Win32Window::WndProc,
			0L,
			0L,
			m_HInstance,
			nullptr,
			::LoadCursorW(nullptr, reinterpret_cast<LPCWSTR>(IDC_ARROW)),
			nullptr,
			nullptr,
			m_wsClassName.c_str(),
			nullptr
		};

		if (!::RegisterClassExW(&wc))
		{
			DWORD err = ::GetLastError();
			WNDCLASSEXW existing;

			if (err != ERROR_CLASS_ALREADY_EXISTS || ::GetClassInfoExW(m_HInstance, m_wsClassName.c_str(), &existing) == 0)
			{
				return false;
			}
		}

		m_HWnd = ::CreateWindowW(
			m_wsClassName.c_str(),
			m_wsTitle.c_str(),
			style,
			100,
			100,
			m_iWidth,
			m_iHeight,
			nullptr,
			nullptr,
			m_HInstance,
			this);

		if (!m_HWnd)
		{
			::UnregisterClassW(m_wsClassName.c_str(), m_HInstance);
			return false;
		}

		return graphics::IWindow::OnInitialized();
	}

	//======================================================================================
	bool Win32Window::OnDestroyed()
	{
		if (m_HWnd)
		{
			Show(SW_HIDE);
			::DestroyWindow(m_HWnd);
			m_HWnd = nullptr;
		}

		if (!m_wsClassName.empty() && m_HInstance)
		{
			::UnregisterClassW(m_wsClassName.c_str(), m_HInstance);
		}

		return graphics::IWindow::OnDestroyed();
	}

	//======================================================================================
	LRESULT CALLBACK Win32Window::WndProc(HWND a_HWnd, UINT a_iMsg, WPARAM a_WParam, LPARAM a_LParam)
	{
		Win32Window* window = nullptr;

		if (a_iMsg == WM_NCCREATE)
		{
			window = reinterpret_cast<Win32Window*>(
				reinterpret_cast<CREATESTRUCTW*>(a_LParam)->lpCreateParams);

			::SetWindowLongPtrW(
				a_HWnd,
				GWLP_USERDATA,
				reinterpret_cast<LONG_PTR>(window));
		}
		else
		{
			window = reinterpret_cast<Win32Window*>(
				::GetWindowLongPtrW(a_HWnd, GWLP_USERDATA));
		}

		if (window)
		{
			window->m_HWnd = a_HWnd;
			return window->HandleMessage(a_iMsg, a_WParam, a_LParam);
		}

		return ::DefWindowProcW(a_HWnd, a_iMsg, a_WParam, a_LParam);
	}

	//======================================================================================
	LRESULT Win32Window::HandleMessage(UINT a_iMsg, WPARAM a_WParam, LPARAM a_LParam)
	{
		if (m_MessageHook && m_MessageHook(m_HWnd, a_iMsg, a_WParam, a_LParam))
		{
			return 0;
		}

		switch (a_iMsg)
		{
			case WM_SIZE:
			{
				if (a_WParam == SIZE_MINIMIZED)
				{
					return 0;
				}

				QueueResize(
					static_cast<uint32_t>(LOWORD(a_LParam)),
					static_cast<uint32_t>(HIWORD(a_LParam)));

				return 0;
			}
			case WM_ACTIVATE:
			{
				if (LOWORD(a_WParam) != WA_INACTIVE)
				{
					QueueFocusGained();
				}
				break;
			}
			case WM_DESTROY:
			{
				::PostQuitMessage(0);
				return 0;
			}
		}

		return ::DefWindowProcW(m_HWnd, a_iMsg, a_WParam, a_LParam);
	}
}

#endif // WIN32