#include "./ISystem.h"

#include <htb_lib/core/Log.h>

namespace htb::core
{
	//======================================================================================
	// ISystem
	//======================================================================================
	ISystem::ISystem(const std::string& a_sName) :
		m_sName(a_sName)
	{}

	//======================================================================================
	ISystem::~ISystem()
	{}

	//======================================================================================
	bool ISystem::Initialize()
	{
		if (m_bInitialized)
		{
			return false;
		}

		if (!OnInitialized())
		{
			return false;
		}

		Log(ELogLevel::SUCCESS, "Initialized %s.", m_sName.c_str());
		m_bDestroyed = false;
		m_bInitialized = true;

		m_bRunning.store(true);
		return true;
	}

	//======================================================================================
	bool ISystem::Destroy(bool a_bLog)
	{
		if (m_bDestroyed)
		{
			return false;
		}

		m_bRunning.store(false);

		if (!OnDestroyed())
		{
			return false;
		}

		//if (a_bLog)
		{
			Log(ELogLevel::SUCCESS, "Destroyed %s.", m_sName.c_str());
		}
		m_bDestroyed = true;
		m_bInitialized = false;

		return true;
	}

	//======================================================================================
	bool ISystem::Running() const
	{
		return m_bRunning.load();
	}

	//======================================================================================
	bool ISystem::OnInitialized()
	{
		return true;
	}

	//======================================================================================
	bool ISystem::OnDestroyed()
	{
		return true;
	}

	//======================================================================================
	// IThreadedSystem
	//======================================================================================
	IThreadedSystem::IThreadedSystem(const std::string& a_sName) :
		m_sName(a_sName)
	{}

	//======================================================================================
	IThreadedSystem::~IThreadedSystem()
	{
		Destroy();
	}

	//======================================================================================
	bool IThreadedSystem::Initialize(bool a_Wait)
	{
		// NOTE: Called from main thread.

		if (m_Thread.joinable())
		{
			// Already running or awaiting cleanup.
			return false;
		}

		m_bRunning.store(false);

		std::promise<bool> initPromise;
		std::future<bool> initFuture = initPromise.get_future();

		m_Thread = std::thread(&IThreadedSystem::ThreadEntry, this, std::move(initPromise));

		if (a_Wait)
		{
			return initFuture.get();
		}

		return true;
	}

	//======================================================================================
	bool IThreadedSystem::Destroy()
	{
		// NOTE: Called from main thread.

		if (!m_Thread.joinable())
		{
			return false;
		}

		{
			std::scoped_lock lock(m_RunningMutex);
			m_bRunning.store(false);
		}
		m_RunningCondVar.notify_one();

		m_Thread.join();

		return OnDestroyed();
	}

	//======================================================================================
	bool IThreadedSystem::OnInitialized()
	{
		Log(ELogLevel::SUCCESS, "Initialized %s.", m_sName.c_str());
		return true;
	}

	//======================================================================================
	bool IThreadedSystem::OnShutdown()
	{
		return true;
	}

	//======================================================================================
	bool IThreadedSystem::OnDestroyed()
	{
		Log(ELogLevel::SUCCESS, "Destroyed %s.", m_sName.c_str());
		return true;
	}

	//======================================================================================
	void IThreadedSystem::WakeUp()
	{
		m_RunningCondVar.notify_one();
	}

	//======================================================================================
	void IThreadedSystem::ThreadEntry(std::promise<bool> a_Promise)
	{
		// NOTE: Called from new thread.

		if (!OnInitialized())
		{
			Log(ELogLevel::_ERROR, "Failed to initialize %s.", m_sName.c_str());
			a_Promise.set_value(false);
			return;
		}

		m_bRunning.store(true);
		a_Promise.set_value(true);

		while (m_bRunning.load())
		{
			{
				std::unique_lock lock(m_RunningMutex);
				m_RunningCondVar.wait(lock, [this]()
					{
						return !Sleep() || !m_bRunning.load();
					});
			}

			if (!m_bRunning.load())
			{
				break;
			}

			Loop();
		}

		if (!OnShutdown())
		{
			Log(ELogLevel::_ERROR, "Failed to shutdown %s.", m_sName.c_str());
		}
	}
}