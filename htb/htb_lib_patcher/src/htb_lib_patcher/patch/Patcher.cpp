#include "./Patcher.h"

#include <htb_lib/file/File.h>
#include <htb_lib_patcher/patch/PatcherState.h>

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
	void Patcher::Initialize()
	{
		m_ePatcherState = EPatcherState::INACTIVE;

		OnInitialized();
	}

	//======================================================================================
	void Patcher::ApplyPatch()
	{
		m_ePatcherState = EPatcherState::RUNNING;

		m_PatchThread = std::thread([this]()
		{
			try
			{
				OnPatchApplied();
			}
			catch (const std::exception& e)
			{
				m_sStatusMessage = e.what();
				m_ePatcherState = EPatcherState::FAILED;
			}
		});
	}

	//======================================================================================
	void Patcher::Join()
	{
		if (m_PatchThread.joinable())
		{
			m_PatchThread.join();
		}
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
	const core::Observable<float>& Patcher::GetPatcherProgress() const
	{
		return m_fPatcherProgress;
	}

	//======================================================================================
	const core::Observable<std::string>& Patcher::GetStatusMessage() const
	{
		return m_sStatusMessage;
	}
}