#pragma once

#include "htb_app/imgui/windows/HEBaseWindow.h"

#include <string>
#include <vector>

namespace htb::core
{
	enum class ELogLevel;
}
namespace htb::imgui
{
	static constexpr float BOTTOM_TOOLBAR_HEIGHT = 75;
	static constexpr size_t LOG_MAX_HISTORY = 100;

	class BottomToolbarWindow : public HEBaseWindow
	{
	public:
		BottomToolbarWindow();
	private:
		void Update() override;

		// Inherited via HEBaseWindow
		bool OnInitialized() override;

		void OnLogEvent(core::ELogLevel a_Level, const std::string& a_sMessage);

		bool m_bLogWindowOpened = false;

		struct LogEntry
		{
			core::ELogLevel level;
			std::string icon;
			std::string message;
		};
		std::vector<LogEntry> m_aLogHistory;
		static bool s_bPanelOpen;
	};
}