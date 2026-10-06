#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <htb_lib/core/Data.h>
#include <htb_lib/script/ScriptArgType.h>

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
	struct RoomImage
	{
		parsing::Chunk* m_pChunk = nullptr;

		void ToData(core::DataStream& a_Data) const;
	};
}