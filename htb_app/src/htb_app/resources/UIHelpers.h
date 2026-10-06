#pragma once

#include <string>
#include <unordered_map>

namespace htb::archive
{
	enum class EArchiveType;
}
namespace htb::resources
{
	enum class ResourceType;

	//---------------------------------------------------------------------
	// DisplayableChunk
	//---------------------------------------------------------------------
	struct DisplayableChunk
	{
		resources::ResourceType m_eResourceType;
		bool m_bVisible = true;
	};

	//---------------------------------------------------------------------
	std::string GetIconFromArchiveType(archive::EArchiveType a_eArchiveType);

	//---------------------------------------------------------------------
	const std::unordered_map<std::string_view, DisplayableChunk>& GetDisplayableChunks(archive::EArchiveType a_eArchiveType);
}
