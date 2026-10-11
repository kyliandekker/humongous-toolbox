#pragma once

namespace htb::patch
{
	enum class EPatcherState
	{
		INACTIVE,
		RUNNING,
		FINISHED,
		FAILED,
	};
}