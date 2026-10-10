#pragma once

#include <htb_lib/building/GameResource.h>

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <htb_lib/core/Data.h>

namespace htb::core
{
	class DataStream;
}
namespace htb::parsing
{
	class Chunk;
}
namespace htb::building
{
	//======================================================================================
	// Script
	//======================================================================================
	/// <summary>
	/// Contains a bunch of instructions with arguments. Can be easily converted to data.
	/// </summary>
	struct RoomImage : public GameResource
	{
	public:
		/// <summary>
		/// Constructs an empty image. Not valid.
		/// </summary>
		RoomImage();

		/// <summary>
		/// Constructs a song. Provided chunk MUST be SGEN.
		/// </summary>
		/// <param name="a_SGENChunk">The SGEN chunk.</param>
		RoomImage(parsing::Chunk& a_Chunk);
	private:
		parsing::Chunk* m_pChunk = nullptr;
	};
}