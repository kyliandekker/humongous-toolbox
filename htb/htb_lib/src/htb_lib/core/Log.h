#pragma once

#include <functional>
#include <string>
#include <utility>

namespace htb::core
{
	enum class ELogLevel
	{
		SUCCESS,
		_ERROR,
		WARNING,
		INFO,
	};

	using LogCallback = std::function<void(ELogLevel, const std::string&)>;

	/// <summary>
	/// Initializes the threaded log system. Must be called before any Log calls.
	/// </summary>
	void InitializeLog();

	/// <summary>
	/// Destroys the threaded log system, flushing remaining messages.
	/// </summary>
	void DestroyLog();

	/// <summary>
	/// Sets the global log callback used by the lib to report messages.
	/// </summary>
	/// <param name="a_fnCallback">The callback to invoke on each log message.</param>
	void SetLogCallback(LogCallback a_fnCallback);

	/// <summary>
	/// Logs a message. The message is queued and processed on a background thread.
	/// </summary>
	/// <param name="a_Level">The severity level of the message.</param>
	/// <param name="a_sMessage">The message to log.</param>
	void Log(ELogLevel a_Level, const std::string& a_sMessage);
	
	template<typename... TArgs>
	void Log(ELogLevel a_Level, const char* a_sFormat, TArgs&&... a_Args)
	{
		int iSize = std::snprintf(nullptr, 0, a_sFormat, std::forward<TArgs>(a_Args)...);

		if (iSize <= 0)
		{
			Log(a_Level, std::string(a_sFormat));
			return;
		}

		std::string sMessage(static_cast<size_t>(iSize), '\0');
		std::snprintf(sMessage.data(), sMessage.size() + 1, a_sFormat, std::forward<TArgs>(a_Args)...);

		Log(a_Level, sMessage);
	}
}
