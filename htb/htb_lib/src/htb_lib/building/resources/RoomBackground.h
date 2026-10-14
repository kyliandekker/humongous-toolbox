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
	// RoomBackground
	//======================================================================================
	/// <summary>
	/// Contains a room background and all the related chunks. Can be converted to png.
	/// </summary>
	struct RoomBackground : public GameResource
	{
	public:
		/// <summary>
		/// Constructs an empty background. Not valid.
		/// </summary>
		RoomBackground();

		/// <summary>
		/// Constructs a background. Provided chunk MUST be RMIM.
		/// </summary>
		/// <param name="a_RMIMChunk">The RMIM chunk.</param>
		RoomBackground(parsing::Chunk& a_RMIMChunk);

		/// <summary>
		/// Replaces an image.
		/// </summary>
		/// <param name="a_RoomBackground">Replaces all the images.</param>
		void ReplaceBackground(RoomBackground& a_RoomBackground);
	private:
		parsing::Chunk* m_pChunk = nullptr;

		parsing::Chunk* m_pRMIHChunk = nullptr; // Contains the x, y, width, height.
		parsing::Chunk* m_pBSMAPChunk = nullptr; // Contains the data.

		parsing::Chunk* m_pAPALChunk = nullptr; // Contains the color palette.
		parsing::Chunk* m_pTRNSChunk = nullptr; // Contains the transparency value.
	};
}