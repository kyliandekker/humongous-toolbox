#pragma once

#include "htb_app/imgui/windows/HEBaseWindow.h"

#include <vector>
#include <memory>

#include "htb_app/imgui/views/SearchBar.h"
#include "htb_app/imgui/views/FileEntryView.h"

namespace htb::imgui
{
	class FileEntryView;

	//---------------------------------------------------------------------
	// ArchiveContentsWindow
	//---------------------------------------------------------------------
	/// <summary>
	/// A window for inspecting specific archives.
	/// </summary>
	class ArchiveContentsWindow : public LoggerDependentWindow
	{
	public:
		/// <summary>
		/// Constructs an explorer window.
		/// </summary>
		ArchiveContentsWindow();

		/// <summary>
		/// Renders the explorer window.
		/// </summary>
		void Update() override;
	private:
		/// <summary>
		/// Initializes all behaviours and values for the window.
		/// </summary>
		/// <returns>True if initialization is successful, otherwise false.</returns>
		bool OnInitialized() override;

		void RenderDropZone();
		void RebuildArchiveViews();

		void OnArchivesChanged();

		std::vector<std::unique_ptr<TreeFileEntryView>> m_aArchiveViews;
		SearchBar m_SearchBar;
	};
}