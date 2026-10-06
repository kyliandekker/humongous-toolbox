#pragma once

#include "htb_app/imgui/windows/HEBaseWindow.h"

#include <string>

namespace htb::imgui
{
	inline float TOP_TOOLBAR_HEIGHT = 120;
	inline float BUTTONS_TOOLBAR_WIDTH = 120;

	class TopToolbarWindow : public HEBaseWindow
	{
	public:
		TopToolbarWindow();
	private:
		void Update() override;

		bool OnInitialized() override;
	};
}
