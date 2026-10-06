#pragma once

#include <string>
#include <memory>

namespace htb::parsing
{
	class Chunk;
}
namespace htb::resources
{
	class Resource;

	class ResourceFactory
	{
	public:
		static std::unique_ptr<Resource> GetResource(parsing::Chunk* a_pChunk, const std::string& a_sFallbackName);
	};
}