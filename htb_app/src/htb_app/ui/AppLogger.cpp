#include "./AppLogger.h"

namespace htb::logger
{
	core::SimpleEvent<core::ELogLevel, const std::string&>& GetLogEvent()
	{
		static core::SimpleEvent<core::ELogLevel, const std::string&> logEvent;
		return logEvent;
	}
}