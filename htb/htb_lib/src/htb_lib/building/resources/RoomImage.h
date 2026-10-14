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
	// RoomImage
	//======================================================================================
	/// <summary>
	/// Contains a room image and all related chunks. Can be converted into png.
	/// </summary>
	struct RoomImage : public GameResource
	{
	public:
		/// <summary>
		/// Constructs an empty room image. Not valid.
		/// </summary>
		RoomImage();

		/// <summary>
		/// Constructs a room image. Provided chunk MUST be OBIM.
		/// </summary>
		/// <param name="a_OBIMChunk">The OBIM chunk.</param>
		RoomImage(parsing::Chunk& a_OBIMChunk);

		/// <summary>
		/// Replaces an image by index.
		/// </summary>
		/// <param name="a_iIndex">Index of the room data.</param>
		/// <param name="a_ChunkData">The data to replace it with.</param>
		void ReplaceImage(size_t a_iIndex, const core::Data& a_ChunkData);

		/// <summary>
		/// Replaces an image.
		/// </summary>
		/// <param name="a_RoomImage">Replaces all the images.</param>
		void ReplaceImage(RoomImage& a_RoomImage);
	private:
		parsing::Chunk* m_pChunk = nullptr;

		parsing::Chunk* m_pIMHDChunk = nullptr; // Contains the x, y, width, height.
		parsing::Chunk* m_pAPALChunk = nullptr; // Contains the color palette.
		parsing::Chunk* m_pTRNSChunk = nullptr; // Contains the transparency value.

		// BMAP or SMAP.
		// BMAP is a HE flat bitmap container.
		// SMAP is a strip-map container.
		std::vector<parsing::Chunk*> m_aImages;
	};
}