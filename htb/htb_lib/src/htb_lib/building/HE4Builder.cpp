#include "./HE4Builder.h"

#include <cassert>

#include <htb_lib/archive/Archive.h>
#include <htb_lib/archive/ArchiveSet.h>
#include <htb_lib/archive/ArchiveType.h>
#include <htb_lib/core/Log.h>
#include <htb_lib/core/Memory.h>
#include <htb_lib/parsing/ChunkIDs.h>
#include <htb_lib/parsing/chunks/sound/SGEN_Chunk.h>

namespace htb::building
{
	//======================================================================================
	// HE4Builder
	//======================================================================================
	bool HE4Builder::Bind(archive::ArchiveSet& a_ArchiveSet)
	{
		m_pHE4 = nullptr;
		m_aSongs.clear();

		for (std::unique_ptr<archive::Archive>& archive : a_ArchiveSet.GetArchives())
		{
			if (archive->GetType() == archive::EArchiveType::HE4)
			{
				m_pHE4 = archive.get();
				break;
			}
		}

		assert(m_pHE4);
		if (!m_pHE4)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not bind HE4: Could not find HE4.");
			return false;
		}

		parsing::Chunk* songChunk = m_pHE4->GetRoot().TryFindChild(parsing::SONG_CHUNK_ID);
		assert(songChunk);
		if (!songChunk)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not bind HE4: Could not find SONG chunk.");
			return false;
		}

		std::vector<parsing::Chunk*> sgenChunks;
		if (!songChunk->TryFindChildren(parsing::SGEN_CHUNK_ID, sgenChunks))
		{
			core::Log(core::ELogLevel::_ERROR, "Could not bind HE4: Could not find SGEN chunks.");
			return false;
		}

		assert(!sgenChunks.empty());
		if (sgenChunks.empty())
		{
			core::Log(core::ELogLevel::_ERROR, "Could not bind HE4: No SGEN chunks were found.");
			return false;
		}

		for (parsing::Chunk* chunk : sgenChunks)
		{
			assert(chunk);
			if (!chunk)
			{
				core::Log(core::ELogLevel::_ERROR, "Could not bind HE4: SGEN chunk was null.");
				return false;
			}

			m_aSongs.emplace_back(*chunk);
		}

		return true;
	}

	//======================================================================================
	bool HE4Builder::Build()
	{
		assert(m_pHE4);
		if (!m_pHE4)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not build HE4: HE4 archive was null.");
			return false;
		}

		for (Song& song : m_aSongs)
		{
			song.Update();
		}


		core::Log(core::ELogLevel::SUCCESS, "Successfully rebuilt HE4.");
		return true;
	}
}