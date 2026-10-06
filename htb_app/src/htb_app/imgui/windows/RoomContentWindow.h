#pragma once

#include "htb_app/imgui/windows/HEBaseWindow.h"

#include "htb_app/imgui/views/SearchBar.h"
#include "htb_app/resources/ResourceType.h"

namespace htb::imgui
{
	class TreeFileEntryView;

	//---------------------------------------------------------------------
	// RoomContentWindow
	//---------------------------------------------------------------------
	/// <summary>
	/// A window for inspecting resources in a room.
	/// </summary>
	class RoomContentWindow : public LoggerDependentWindow
	{
	public:
		/// <summary>
		/// Constructs a room content window.
		/// </summary>
		RoomContentWindow();

		/// <summary>
		/// Renders the room content window.
		/// </summary>
		void Update() override;
	private:
		/// <summary>
		/// Initializes all behaviours and values for the window.
		/// </summary>
		/// <returns>True if initialization is successful, otherwise false.</returns>
		bool OnInitialized() override;

		void OnSelectedViewChanged(const imgui::TreeFileEntryView* oldView, const imgui::TreeFileEntryView* newView);

		SearchBar m_SearchBar;
		int m_iSortColumn = -1;
		bool m_bSortAscending = true;
		int m_iSelectedTab = 0;

		bool MatchesTabFilter(resources::ResourceType a_eType, int a_iTab) const;
		int CountResourcesForTab(int a_iTab) const;

		std::string m_sRoomName;
	};
}