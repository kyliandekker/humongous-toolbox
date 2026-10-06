#include "Workspace.h"

#include <htb_lib/core/Memory.h>
#include <htb_lib/file/file.h>
#include <htb_lib/parsing/ChunkIDs.h>
#include <htb_lib/parsing/ChunkParser.h>

#include <htb_lib_win32/win32/winfile.h>

#include "htb_app/imgui/views/FileEntryView.h"
#include "htb_app/resources/Resource.h"

namespace htb
{
	//---------------------------------------------------------------------
	editor::Workspace& GetWorkspace()
	{
		static htb::editor::Workspace workspace;

		return workspace;
	}
}

namespace htb::editor
{
	//---------------------------------------------------------------------
	Workspace::Workspace() : core::ISystem("Workspace")
	{
		m_sAppDataPath = file::GetAppDataPath().generic_string() + "/humongous_explorer";
		file::CreateFolder(m_sAppDataPath);
	}

	//---------------------------------------------------------------------
	resources::ResourceType Workspace::GetResourceTypeFilter() const
	{
		return m_ResourceTypeFilter;
	}

	//---------------------------------------------------------------------
	void Workspace::SetResourceTypeFilter(resources::ResourceType a_ResourceTypeFilter)
	{
		m_ResourceTypeFilter = a_ResourceTypeFilter;
	}

	//---------------------------------------------------------------------
	const std::string& Workspace::GetAppDataPath() const
	{
		return m_sAppDataPath;
	}

	//---------------------------------------------------------------------
	const core::Event<>& Workspace::GetArchivesChanged() const
	{
		return m_onArchivesChanged;
	}

	//---------------------------------------------------------------------
	void Workspace::SetSelectedFileEntryView(imgui::TreeFileEntryView* a_pSelectedView)
	{
		if (m_pSelectedFileEntryView == a_pSelectedView)
		{
			return;
		}

		m_pSelectedFileEntryView = a_pSelectedView;
	}

	//---------------------------------------------------------------------
	imgui::TreeFileEntryView* Workspace::GetSelectedView()
	{
		return m_pSelectedFileEntryView.get();
	}

	//---------------------------------------------------------------------
	const core::Observable<imgui::TreeFileEntryView*>& Workspace::GetSelectedViewObs() const
	{
		return m_pSelectedFileEntryView;
	}

	//---------------------------------------------------------------------
	void Workspace::SetSelectedResource(resources::Resource* a_pSelectedResource)
	{
		m_pSelectedResource = a_pSelectedResource;
	}

	//---------------------------------------------------------------------
	resources::Resource* Workspace::GetSelectedResource()
	{
		return m_pSelectedResource.get();
	}

	//---------------------------------------------------------------------
	const core::Observable<resources::Resource*>& Workspace::GetSelectedResourceObs() const
	{
		return m_pSelectedResource;
	}

	//---------------------------------------------------------------------
	const archive::ArchiveSet& Workspace::GetArchiveSet() const
	{
		return m_ArchiveSet;
	}

	//---------------------------------------------------------------------
	archive::ArchiveSet& Workspace::GetArchiveSet()
	{
		return m_ArchiveSet;
	}
}
