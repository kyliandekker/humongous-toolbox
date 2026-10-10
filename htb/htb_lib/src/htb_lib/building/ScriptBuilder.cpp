#include "./ScriptBuilder.h"

#include <cassert>
#include <map>

#include <htb_lib/archive/Archive.h>
#include <htb_lib/archive/ArchiveSet.h>
#include <htb_lib/core/DataStream.h>
#include <htb_lib/core/Log.h>
#include <htb_lib/core/Memory.h>
#include <htb_lib/parsing/ChunkIDs.h>
#include <htb_lib/script/ScriptArgType.h>

namespace htb::building
{
	//======================================================================================
	// ScriptBuilder
	//======================================================================================
	bool ScriptBuilder::Bind(archive::ArchiveSet& a_ArchiveSet)
	{
		m_pA = nullptr;
		m_pHE2 = nullptr;
		m_aTalkScripts.clear();
		m_mOPCodeMap.clear();

		for (std::unique_ptr<archive::Archive>& archive : a_ArchiveSet.GetArchives())
		{
			if (archive->GetType() == archive::EArchiveType::HE2)
			{
				m_pHE2 = archive.get();
			}
			else if (archive->GetType() == archive::EArchiveType::A)
			{
				m_pA = archive.get();
			}
		}

		assert(m_pHE2);
		if (!m_pHE2)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not bind scripts: Could not find HE2 archive.");
			return false;
		}

		assert(m_pA);
		if (!m_pA)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not bind scripts: Could not find (A) archive.");
			return false;
		}

		script::GetOPCodeTable(m_mOPCodeMap, a_ArchiveSet.GetScriptVersion(), a_ArchiveSet.GetHEVersion());
		assert(!m_mOPCodeMap.empty());
		if (m_mOPCodeMap.empty())
		{
			core::Log(core::ELogLevel::_ERROR, "Could not bind scripts: Could not find OP codes map.");
			return false;
		}

		// Cache TALK chunks in a map beforehand for faster lookup.
		std::unordered_map<std::size_t, parsing::Chunk*> talkChunkTable;
		m_pHE2->GetRoot().TryFindChildren(parsing::TALK_CHUNK_ID, [&talkChunkTable](parsing::Chunk* chunk)
		{
			talkChunkTable[chunk->GetOffsetFromRoot()] = chunk;
		});

		// Find all scripts.
		std::vector<parsing::Chunk*> scripts;
		m_pA->GetRoot().TryFindChildren({
				parsing::SCRP_CHUNK_ID,
				parsing::LSCR_CHUNK_ID,
				parsing::LSC2_CHUNK_ID,
				parsing::ENCD_CHUNK_ID,
				parsing::EXCD_CHUNK_ID,
				parsing::VERB_CHUNK_ID,
			}, [this, &talkChunkTable](parsing::Chunk* chunk)
			{
				assert(chunk);
				if (!chunk)
				{
					return;
				}

				size_t offset = chunk->GetOffsetFromRoot();

				Script script(*chunk, m_mOPCodeMap);
				if (!script.IsValid())
				{
					return;
				}

				if (script.BindTALKies(talkChunkTable))
				{
					m_aTalkScripts.push_back(std::move(script));
				}
			}
		);

		return true;
	}

	//======================================================================================
	bool ScriptBuilder::Build()
	{
		assert(m_pHE2);
		if (!m_pHE2)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not build scripts: HE2 archive was null.");
			return false;
		}

		assert(m_pA);
		if (!m_pA)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not build scripts: (A) archive was null.");
			return false;
		}

		for (Script& script : m_aTalkScripts)
		{
			script.Update();
		}


		core::Log(core::ELogLevel::SUCCESS, "Successfully rebuilt scripts in (A).");
		return true;
	}
}