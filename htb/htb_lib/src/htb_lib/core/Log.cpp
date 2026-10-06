#include "./Log.h"

#include <condition_variable>
#include <cstdio>
#include <mutex>
#include <queue>
#include <thread>

namespace htb::core
{
	struct LogMessage
	{
		ELogLevel level;
		std::string message;
	};

	static LogCallback s_fnCallback = nullptr;
	static std::queue<LogMessage> s_Messages;
	static std::mutex s_Mutex;
	static std::condition_variable s_CondVar;
	static std::thread s_Thread;
	static bool s_bRunning = false;

	//======================================================================================
	static void LogThread()
	{
		while (true)
		{
			std::unique_lock lock(s_Mutex);
			s_CondVar.wait(lock, []
			{
				return !s_Messages.empty() || !s_bRunning;
			});

			while (!s_Messages.empty())
			{
				LogMessage msg = std::move(s_Messages.front());
				s_Messages.pop();

				lock.unlock();

				if (s_fnCallback)
				{
					s_fnCallback(msg.level, msg.message);
				}
				else
				{
					const char* prefix = "";
					switch (msg.level)
					{
						case ELogLevel::SUCCESS: prefix = "[OK]     "; break;
						case ELogLevel::_ERROR:   prefix = "[FAIL]   "; break;
						case ELogLevel::WARNING: prefix = "[WARN]   "; break;
						case ELogLevel::INFO:    prefix = "[INFO]   "; break;
					}
					printf("%s%s\n", prefix, msg.message.c_str());
				}

				lock.lock();
			}

			if (s_bRunning)
			{
				continue;
			}

			break;
		}
	}

	//======================================================================================
	void InitializeLog()
	{
		s_bRunning = true;
		s_Thread = std::thread(LogThread);
	}

	//======================================================================================
	void DestroyLog()
	{
		{
			std::scoped_lock lock(s_Mutex);
			s_bRunning = false;
		}
		s_CondVar.notify_all();
		if (s_Thread.joinable())
		{
			s_Thread.join();
		}
		fflush(stdout);
	}

	//======================================================================================
	void SetLogCallback(LogCallback a_fnCallback)
	{
		s_fnCallback = std::move(a_fnCallback);
	}

	//======================================================================================
	void Log(ELogLevel a_Level, const std::string& a_sMessage)
	{
		{
			std::scoped_lock lock(s_Mutex);
			s_Messages.push({ a_Level, a_sMessage });
		}
		s_CondVar.notify_all();
	}
}
