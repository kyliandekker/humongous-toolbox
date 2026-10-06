#pragma once

#include <imgui/windows/MainWindowDock.h>

namespace htb::imgui
{
	class EditorWindowDock : public ImGui::MainWindowDock
	{
	public:
		void Render() override;
	private:
		void Update() override;
		bool OnInitialized() override;
	};
}