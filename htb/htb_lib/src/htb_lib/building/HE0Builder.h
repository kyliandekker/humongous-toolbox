#pragma once

// external
#include <cstdint>
#include <string_view>
#include <vector>

#include <htb_lib/building/BuilderBase.h>

namespace htb::archive
{
	class Archive;
}
namespace htb::parsing
{
	class Chunk;
}
namespace htb::building
{
	using IdToChunk = std::vector<parsing::Chunk*>;

	//======================================================================================
	// HE0Builder
	//======================================================================================
	/// <summary>
	/// Rebuilds a HE0 with the associated data.
	/// </summary>
	class HE0Builder : public BuilderBase
	{
	public:
		/// <summary>
		/// Associates the chunks before rebuilding other archive this archive is dependent on.
		/// </summary>
		bool Bind(archive::ArchiveSet& a_ArchiveSet) override;

		/// <summary>
		/// Builds the chunks in the associated archive.
		/// </summary>
		bool Build() override;
	protected:
		/// <summary>
		/// Parses the DLFL chunk.
		/// </summary>
		/// <param name="a_pChunk">The DLFL chunk.</param>
		/// <param name="a_aOut">Offsets of all LFLFs.</param>
		/// <returns>True if parsing was successful, false otherwise.</returns>
		bool ParseDLFL(parsing::Chunk* a_pChunk, std::vector<uint32_t>& a_aOut);

		/// <summary>
		/// Reads a DIR chunk.
		/// </summary>
		/// <param name="a_pChunk">Any DIR chunk.</param>
		/// <param name="a_sChunkID">The DIR chunk ID.</param>
		/// <param name="a_iNumEntries">The number of entries that were found.</param>
		/// <param name="a_aIds">The ids that were found.</param>
		/// <param name="a_aOffsets">The offsets that were found.</param>
		/// <param name="a_aSizes">The sizes that were found.</param>
		/// <returns>True if parsing was successful, false otherwise.</returns>
		bool ReadDIRChunk(parsing::Chunk* a_pChunk, const std::string_view& a_sChunkID, uint16_t& a_iNumEntries, std::vector<uint8_t>& a_aIds, std::vector<uint32_t>& a_aOffsets, std::vector<uint32_t>& a_aSizes);

		/// <summary>
		/// Binds chunks to the DIR chunk.
		/// </summary>
		/// <param name="a_pDirChunk">The DIR chunk to check.</param>
		/// <param name="a_OriginalArchive">The archive with the chunks.</param>
		/// <param name="a_aOriginalDLFLData">The DLFL data for each LFLF.</param>
		/// <param name="a_sDirTag">The chunk ID of the DIR.</param>
		/// <param name="a_aOut">Map with mapped chunk ids.</param>
		/// <returns>True if parsing was successful, false otherwise.</returns>
		bool BuildIdToChunk(parsing::Chunk* a_pDirChunk, archive::Archive& a_OriginalArchive, const std::vector<uint32_t>& a_aOriginalDLFLData, std::string_view a_sDirTag, IdToChunk& a_aOut);

		/// <summary>
		/// Rebuilds the DLFL chunk.
		/// </summary>
		/// <param name="a_pChunk">The DLFL chunk to rebuild.</param>
		/// <param name="a_Archive">The archive with the LFLF chunks.</param>
		/// <returns>True if building was successful, false otherwise.</returns>
		bool RebuildDLFL(parsing::Chunk* a_pChunk, archive::Archive& a_Archive);

		/// <summary>
		/// Rebuilds a DIR chunk.
		/// </summary>
		/// <param name="a_pChunk">The DIR chunk to rebuild.</param>
		/// <param name="a_aIdToChunk">The map with the chunks.</param>
		/// <param name="a_aNewDLFLData">The new LFLF data.</param>
		/// <param name="a_sChunkID">The DIR chunk ID.</param>
		/// <returns>True if building was successful, false otherwise.</returns>
		bool RebuildDIRChunk(parsing::Chunk* a_pChunk, const IdToChunk& a_aIdToChunk, const std::vector<uint32_t>& a_aNewDLFLData, std::string_view a_sChunkID);

		archive::Archive* m_pHE0 = nullptr;
		archive::Archive* m_pA = nullptr;

		std::vector<uint32_t> m_aOriginalDLFLData;

		parsing::Chunk* m_pDLFLChunk = nullptr;

		parsing::Chunk* m_pDIRRChunk = nullptr;
		IdToChunk m_aDIRR; // RMDA byte offsets + 16.

		parsing::Chunk* m_pDIRSChunk = nullptr;
		IdToChunk m_aDIRS; // SCRP byte offsets + 16 (LSCR, LSC2, EXCD, ENCD are inside the RMDA).

		parsing::Chunk* m_pDIRNChunk = nullptr;
		IdToChunk m_aDIRN; // SOUN/DIGI/TALK byte offsets + 16 (only TALK inside the (A) numEntries).

		parsing::Chunk* m_pDIRCChunk = nullptr;
		IdToChunk m_aDIRC; // COST/AKOS byte offsets + 16 (Costume/Actor).

		parsing::Chunk* m_pDIRFChunk = nullptr;
		IdToChunk m_aDIRF; // CHAR byte offsets + 16 (Characters).

		parsing::Chunk* m_pDIRMChunk = nullptr;
		IdToChunk m_aDIRM; // (MULT/AWIZ) byte offsets + 16 (Movie/Animation).

		parsing::Chunk* m_pDIRTChunk = nullptr;
		IdToChunk m_aDIRT; // TLKE byte offsets + 16 (old TALKs from non-humongous games (afaik)).
	};
}