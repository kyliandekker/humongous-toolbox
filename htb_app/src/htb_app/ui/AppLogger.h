#pragma once

#include <string>

#include <htb_lib/core/Log.h>

#include "htb_app/core/Event.h"

namespace htb::logger
{
	extern core::SimpleEvent<core::ELogLevel, const std::string&>& GetLogEvent();
}