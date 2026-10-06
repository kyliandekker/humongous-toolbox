#pragma once

#include "htb_app/imgui/views/FileEntryView.h"

namespace htb::resources
{
	enum class ResourceType;
}
namespace htb::imgui
{
	//---------------------------------------------------------------------
	struct ResourceFileEntryView : public FileEntryView
	{
		ResourceFileEntryView(std::vector<std::unique_ptr<RowEntry>> a_aRows);
	};

	//---------------------------------------------------------------------
	struct TreeResourceFileEntryView : public TreeFileEntryView
	{
		TreeResourceFileEntryView(std::vector<std::unique_ptr<RowEntry>> a_aRows);
	};
}