#include "./IWindow.h"

namespace htb::graphics
{
	//======================================================================================
	// IWindow
	//======================================================================================
	IWindow::IWindow(const std::string& a_sName) : core::ISystem(a_sName)
	{}

	//======================================================================================
	bool IWindow::HasPendingResize() const
	{
		return m_iResizeWidth != 0 && m_iResizeHeight != 0;
	}

	//======================================================================================
	void IWindow::ConsumeResize(uint32_t& a_iOutWidth, uint32_t& a_iOutHeight)
	{
		a_iOutWidth = m_iResizeWidth;
		a_iOutHeight = m_iResizeHeight;
		m_iResizeWidth = 0;
		m_iResizeHeight = 0;
	}

	//======================================================================================
	bool IWindow::HasPendingFocusGained() const
	{
		return m_bFocusGained;
	}

	//======================================================================================
	void IWindow::ConsumeFocusGained()
	{
		m_bFocusGained = false;
	}

	//======================================================================================
	uint32_t IWindow::GetWidth() const
	{
		return m_iWidth;
	}

	//======================================================================================
	uint32_t IWindow::GetHeight() const
	{
		return m_iHeight;
	}

	//======================================================================================
	void IWindow::QueueResize(uint32_t a_iWidth, uint32_t a_iHeight)
	{
		m_iResizeWidth = a_iWidth;
		m_iResizeHeight = a_iHeight;
		m_iWidth = a_iWidth;
		m_iHeight = a_iHeight;
	}

	//======================================================================================
	void IWindow::QueueFocusGained()
	{
		m_bFocusGained = true;
	}
}