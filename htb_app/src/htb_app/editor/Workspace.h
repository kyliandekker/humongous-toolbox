#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

#include <htb_lib/archive/ArchiveSet.h>
#include <htb_lib/core/Data.h>
#include <htb_lib/file/FILEPCH.h>
#include <htb_lib/parsing/Chunk.h>

#include <htb_lib_gui/core/ISystem.h>

#include "htb_app/core/Event.h"
#include "htb_app/core/Observable.h"
#include "htb_app/resources/ResourceType.h"

namespace htb
{
	namespace editor
	{
		class Workspace;
	}

	extern editor::Workspace& GetWorkspace();
}
namespace htb::imgui
{
	class TreeFileEntryView;
}
namespace htb::resources
{
	class Resource;
}
namespace htb::editor
{
	//---------------------------------------------------------------------
	class Workspace : public core::ISystem
	{
	public:
		Workspace();

		resources::ResourceType GetResourceTypeFilter() const;
		void SetResourceTypeFilter(resources::ResourceType a_ResourceTypeFilter);
		const std::string& GetAppDataPath() const;

		const core::Event<>& GetArchivesChanged() const;

		void SetSelectedFileEntryView(imgui::TreeFileEntryView* a_pSelectedView);
		imgui::TreeFileEntryView* GetSelectedView();
		const core::Observable<imgui::TreeFileEntryView*>& GetSelectedViewObs() const;

		void SetSelectedResource(resources::Resource* a_pSelectedResource);
		resources::Resource* GetSelectedResource();
		const core::Observable<resources::Resource*>& GetSelectedResourceObs() const;

		const archive::ArchiveSet& GetArchiveSet() const;
		archive::ArchiveSet& GetArchiveSet();

		const std::string& GetAppVersion() const
		{
			return m_sAppVersion;
		}

		const std::string& GetAppName() const
		{
			return m_sAppName;
		}

		void ToggleLogHistoryPanelOpen()
		{
			m_bIsLogHistoryPanelOpened = !m_bIsLogHistoryPanelOpened;
		}

		bool IsLogHistoryPanelOpen() const
		{
			return m_bIsLogHistoryPanelOpened;
		}
	private:
		std::string m_sAppName = "Humongous Explorer";
		std::string m_sAppVersion = "v1.0.0";

		bool m_bIsLogHistoryPanelOpened = false;

		resources::ResourceType m_ResourceTypeFilter = resources::ResourceType::Unknown;
		std::string m_sAppDataPath;

		archive::ArchiveSet m_ArchiveSet;

		core::Event<> m_onArchivesChanged;

		core::Observable<imgui::TreeFileEntryView*> m_pSelectedFileEntryView{nullptr};
		core::Observable<resources::Resource*> m_pSelectedResource{nullptr};
	};
}