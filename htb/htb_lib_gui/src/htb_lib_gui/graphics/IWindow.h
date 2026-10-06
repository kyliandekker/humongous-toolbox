#pragma once

#include <htb_lib_gui/core/ISystem.h>

#include <cstdint>
#include <string>

namespace htb::graphics
{
	//======================================================================================
	// IWindow
	//======================================================================================
	/// <summary>
	/// Represents an application window. Abstract, needs to be implemented on platforms.
	/// </summary>
	class IWindow : public core::ISystem
	{
	public:
		/// <summary>
		/// Constructs a system with a given name.
		/// </summary>
		/// <param name="a_sName">The name of the system.</param>
		IWindow(const std::string& a_sName);

		IWindow(const IWindow&) = delete;
		IWindow& operator=(const IWindow&) = delete;

		/// <summary>
		/// Processes pending window messages.
		/// </summary>
		/// <returns>False once the application should quit, otherwise true.</returns>
		virtual bool ProcessMessages() = 0;

		/// <summary>
		/// Makes the window visible and updates it.
		/// </summary>
		virtual void Show(int a_iShowCmd) = 0;

		/// <summary>
		/// Minimizes the window.
		/// </summary>
		virtual void Minimize() = 0;

		/// <summary>
		/// Maximizes or restores the window.
		/// </summary>
		virtual void Maximize() = 0;

		/// <summary>
		/// Closes the window.
		/// </summary>
		virtual void Close() = 0;

		/// <summary>
		/// Checks whether the window is maximized.
		/// </summary>
		/// <returns>True if the window is maximized, otherwise false.</returns>
		virtual bool IsMaximized() const = 0;

		/// <summary>
		/// Checks whether a resize was queued and not yet consumed.
		/// </summary>
		bool HasPendingResize() const;

		/// <summary>
		/// Retrieves and clears the queued client area size.
		/// </summary>
		void ConsumeResize(uint32_t& a_iOutWidth, uint32_t& a_iOutHeight);

		/// <summary>
		/// Checks whether the window gained focus and this was not yet consumed.
		/// </summary>
		/// <returns>True if a focus-gain event is pending, otherwise false.</returns>
		bool HasPendingFocusGained() const;

		/// <summary>
		/// Retrieves and clears the pending focus-gain event.
		/// </summary>
		void ConsumeFocusGained();

		/// <summary>
		/// Retrieves the current window width.
		/// </summary>
		uint32_t GetWidth() const;

		/// <summary>
		/// Retrieves the current window height.
		/// </summary>
		uint32_t GetHeight() const;
	protected:
		/// <summary>
		/// Queues a resize event.
		/// </summary>
		void QueueResize(uint32_t a_iWidth, uint32_t a_iHeight);

		/// <summary>
		/// Queues a focus-gain event.
		/// </summary>
		void QueueFocusGained();

		uint32_t m_iWidth = 1920;
		uint32_t m_iHeight = 1080;
	private:
		uint32_t m_iResizeWidth = 0;
		uint32_t m_iResizeHeight = 0;

		bool m_bFocusGained = false;
	};
}