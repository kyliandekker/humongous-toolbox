#include "./Patcher.h"

#include <htb_lib/file/File.h>

namespace htb::patch
{
	//======================================================================================
	// Patcher
	//======================================================================================
	Patcher::Patcher(const std::string& a_sName) : m_sName(a_sName)
	{}

	//======================================================================================
	const std::string& Patcher::GetVersion() const
	{
		return m_sVersion;
	}

	//======================================================================================
	const fs::path& Patcher::GetWorkingPath() const
	{
		return m_WorkingPath;
	}

	//======================================================================================
	void Patcher::SetWorkingPath(const fs::path& a_WorkingPath)
	{
		m_WorkingPath = a_WorkingPath / m_sName;
		htb::file::CreateFolder(m_WorkingPath);
	}

	//======================================================================================
	const core::Observable<EPatcherState>& Patcher::GetPatcherState() const
	{
		return m_ePatcherState;
	}

	//======================================================================================
	const core::Observable<std::string>& Patcher::GetStatusMessage() const
	{
		return m_sStatusMessage;
	}
}