#include "./Patcher.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/utils.h>

#include <htb_lib/archive/ArchiveSet.h>
#include <htb_lib/building/HE0Builder.h>
#include <htb_lib/building/resources/Talkie.h>
#include <htb_lib/building/ScriptBuilder.h>
#include <htb_lib/core/Data.h>
#include <htb_lib/core/DataStream.h>
#include <htb_lib/file/file.h>
#include <htb_lib/parsing/Chunk.h>
#include <htb_lib/parsing/ChunkIDs.h>

#include "htb_patcher/PatchInfo.h"
#include "htb_patcher/LocText.h"

constexpr const char* SAVE_FILE = "/patcher.settings";
constexpr const char* USE_SPYFOX_1 = "useSpyFox1";
constexpr const char* USE_SPYFOX_2 = "useSpyFox2";
constexpr const char* SPYFOX_1_PATH = "spyFox1Path";
constexpr const char* SPYFOX_2_PATH = "spyFox2Path";
constexpr const char* SPYFOX_3_PATH = "spyFox3Path";

namespace htb::patch
{
	Patcher& GetPatcher()
	{
		static Patcher patcher;
		return patcher;
	}

	//======================================================================================
	// Patcher
	//======================================================================================
	void Patcher::ApplyPatch()
	{
		Join();

		m_fProgress.store(0.0f);
		m_bPatchFinished.store(false);
		m_bPatchFailed.store(false);
		m_sFailReason.clear();

		SetPatchState(PatchState::PATCHING);

		m_PatchThread = std::thread([this]() { RunPatch(); });
	}

	//======================================================================================
	void Patcher::Join()
	{
		if (m_PatchThread.joinable())
		{
			m_PatchThread.join();
		}
	}

	//======================================================================================
	void Patcher::RunPatch()
	{
		try
		{
			{
				fs::path sf1he4path = m_SpyFox1Path;
				sf1he4path.append("SPYFox.HE4");

				std::unique_ptr<archive::Archive> he4 = std::make_unique<archive::Archive>();
				if (!he4->Load(sf1he4path))
				{
					throw std::runtime_error(PATCHER_ERROR_LOAD_SPY_FOX_1_HE4);
				}

				std::vector<parsing::Chunk*> songs;
				if (!he4->GetRoot().TryFindChildren(parsing::SDAT_CHUNK_ID, songs))
				{
					throw std::runtime_error(PATCHER_ERROR_FIND_SONGS_SPY_FOX_1);
				}

				constexpr int SPYFOX_SONG = 38;
				if (songs.size() <= SPYFOX_SONG)
				{
					throw std::runtime_error(PATCHER_ERROR_FIND_SF_SONG_SPY_FOX_1);
				}

				m_Song = songs[SPYFOX_SONG]->GetData();

				fs::path sf1apath = m_SpyFox1Path;
				sf1apath.append("SPYFox.(A)");

				std::unique_ptr<archive::Archive> a = std::make_unique<archive::Archive>();
				if (!he4->Load(sf1apath))
				{
					throw std::runtime_error(PATCHER_ERROR_LOAD_SPY_FOX_1_A);
				}

				std::vector<parsing::Chunk*> rooms;
				if (!he4->GetRoot().TryFindChildren(parsing::LFLF_CHUNK_ID, rooms))
				{
					throw std::runtime_error(PATCHER_ERROR_FIND_ROOMS_SPY_FOX_1);
				}

				constexpr int END_SOUND_LFLF_ROOM = 8;
				if (rooms.size() <= END_SOUND_LFLF_ROOM)
				{
					throw std::runtime_error(PATCHER_ERROR_FIND_LFLF_SPY_FOX_1);
				}

				std::vector<parsing::Chunk*> sfx;
				if (!rooms[END_SOUND_LFLF_ROOM]->TryFindChildren(parsing::SDAT_CHUNK_ID, sfx))
				{
					throw std::runtime_error(PATCHER_ERROR_FIND_END_SOUND_SPY_FOX_1);
				}

				m_EndSound = sfx[0]->GetData();
			}

			fs::path sf3archivepath = m_SpyFox3Path;
			sf3archivepath.append("SPYOZON.(A)");

			{
				std::lock_guard lock(m_BusyWithMutex);
				m_sBusyWith = PATCHER_BUSY_WITH_LOAD_SPY_FOX_3;
			}

			archive::ArchiveSet spyfox3;
			if (!spyfox3.LoadArchives(sf3archivepath))
			{
				throw std::runtime_error(PATCHER_ERROR_LOAD_ARCHIVES_SPY_FOX_3);
			}

			archive::Archive* spyfox3HE2 = nullptr;
			archive::Archive* spyfox3HE0 = nullptr;
			archive::Archive* spyfox3A = nullptr;
			for (std::unique_ptr<archive::Archive>& archive : spyfox3.GetArchives())
			{
				if (archive->GetType() == archive::EArchiveType::HE2)
				{
					spyfox3HE2 = archive.get();
				}
				else if (archive->GetType() == archive::EArchiveType::HE0)
				{
					spyfox3HE0 = archive.get();
				}
				else if (archive->GetType() == archive::EArchiveType::A)
				{
					spyfox3A = archive.get();
				}
			}

			m_fProgress.store(0.10f);

			SetBusyWith(PATCHER_BUSY_WITH_LOADING_BACKGROUND_IMAGES);

			m_aBackgroundImages.clear();
			m_aBackgroundImages.reserve(PATCHER_BACKGROUND.size());

			std::vector<parsing::Chunk*> backgrounds;
			if (!spyfox3A->GetRoot().TryFindChildren(parsing::IM00_CHUNK_ID, backgrounds))
			{
				throw std::runtime_error(PATCHER_ERROR_FIND_IMAGES_SPY_FOX_3);
			}

			size_t backgroundsSize = backgrounds.size();
			for (size_t i = 0; i < backgroundsSize; i++)
			{
				parsing::Chunk* chunk = backgrounds[i];
				if (std::find(PATCHER_BACKGROUND.begin(), PATCHER_BACKGROUND.end(), i) != PATCHER_BACKGROUND.end())
				{
					m_aBackgroundImages.emplace_back(chunk);
				}
			}

			for (Image& image : m_aBackgroundImages)
			{
				if (!image.Load())
				{
					throw std::runtime_error(PATCHER_ERROR_LOAD_IMAGES_SPY_FOX_3);
				}
			}

			m_fProgress.store(0.20f);

			if (!spyfox3HE2)
			{
				throw std::runtime_error(PATCHER_ERROR_FIND_HE2_SPY_FOX_3);
			}
			
			if (!spyfox3HE0)
			{
				throw std::runtime_error(PATCHER_ERROR_FIND_HE0_SPY_FOX_3);
			}
			
			if (!spyfox3A)
			{
				throw std::runtime_error(PATCHER_ERROR_FIND_A_SPY_FOX_3);
			}

			SetBusyWith(PATCHER_BUSY_WITH_BINDING_SCRIPTS);

			building::ScriptBuilder scriptBuilder;
			if (!scriptBuilder.Bind(spyfox3))
			{
				throw std::runtime_error(PATCHER_ERROR_BIND_SCRIPTS_SPY_FOX_3);
			}

			m_fProgress.store(0.30f);

			SetBusyWith(PATCHER_BUSY_WITH_BINDING_INDEX);

			building::HE0Builder he0Builder;
			if (!he0Builder.Bind(spyfox3))
			{
				throw std::runtime_error(PATCHER_ERROR_BIND_HE0_SPY_FOX_3);
			}

			m_fProgress.store(0.40f);

			SetBusyWith(PATCHER_BUSY_WITH_SEARCHING_TALKIES_SPY_FOX_3);

			std::vector<building::Talkie> sf3Talkies;
			spyfox3HE2->GetRoot().TryFindChildren(parsing::TALK_CHUNK_ID, [&sf3Talkies](parsing::Chunk* chunk)
			{
				sf3Talkies.emplace_back(*chunk);
			});

			if (sf3Talkies.empty())
			{
				throw std::runtime_error(PATCHER_ERROR_FIND_TALKS_SPY_FOX_3);
			}

			m_fProgress.store(0.45f);

			SetBusyWith(PATCHER_BUSY_WITH_REPLACING_WITH_SPY_FOX_2_TALKIES);

			// Replace with TALKs from SF2.
			{
				fs::path sf2archivepath = m_SpyFox2Path;
				sf2archivepath.append("Spyfox2.(A)");

				archive::ArchiveSet spyfox2;
				if (!spyfox2.LoadArchives(sf2archivepath))
				{
					throw std::runtime_error(PATCHER_ERROR_LOAD_ARCHIVES_SPY_FOX_2);
				}

				archive::Archive* spyfox2HE2 = nullptr;
				for (std::unique_ptr<archive::Archive>& archive : spyfox2.GetArchives())
				{
					if (archive->GetType() == archive::EArchiveType::HE2)
					{
						spyfox2HE2 = archive.get();
					}
				}

				if (!spyfox2HE2)
				{
					throw std::runtime_error(PATCHER_ERROR_FIND_HE2_SPY_FOX_2);
				}

				std::vector<building::Talkie> sf2Talkies;
				spyfox2HE2->GetRoot().TryFindChildren(parsing::TALK_CHUNK_ID, [&sf2Talkies](parsing::Chunk* chunk)
				{
					sf2Talkies.emplace_back(*chunk);
				});

				if (sf2Talkies.empty())
				{
					throw std::runtime_error(PATCHER_ERROR_FIND_TALKS_SPY_FOX_2);
				}

				for (const auto& patches : SF2_VO_INDEX)
				{
					building::Talkie& sf2Talkie = sf2Talkies[patches.first];
					building::Talkie& sf3Talkie = sf3Talkies[patches.second];

					sf3Talkie.SetAudioData(sf2Talkie.GetAudioData());
					if (sf2Talkie.HasSBNGData() && sf3Talkie.HasSBNGData())
					{
						sf3Talkie.SetSBNGData(sf2Talkie.GetSBNGData());
					}
				}
			}

			m_fProgress.store(0.60f);

			SetBusyWith(PATCHER_BUSY_WITH_REPLACING_WITH_SPY_FOX_1_TALKIES);

			// Replace with TALKs from SF1.
			{
				fs::path sf1archivepath = m_SpyFox1Path;
				sf1archivepath.append("SPYFox.(A)");

				archive::ArchiveSet spyfox1;
				if (!spyfox1.LoadArchives(sf1archivepath))
				{
					throw std::runtime_error(PATCHER_ERROR_LOAD_ARCHIVES_SPY_FOX_1);
				}

				archive::Archive* spyfox1HE2 = nullptr;
				for (std::unique_ptr<archive::Archive>& archive : spyfox1.GetArchives())
				{
					if (archive->GetType() == archive::EArchiveType::HE2)
					{
						spyfox1HE2 = archive.get();
					}
				}

				if (!spyfox1HE2)
				{
					throw std::runtime_error(PATCHER_ERROR_FIND_HE2_SPY_FOX_1);
				}

				std::vector<building::Talkie> sf1Talkies;
				spyfox1HE2->GetRoot().TryFindChildren(parsing::TALK_CHUNK_ID, [&sf1Talkies](parsing::Chunk* chunk)
					{
						sf1Talkies.emplace_back(*chunk);
					});

				if (sf1Talkies.empty())
				{
					throw std::runtime_error(PATCHER_ERROR_FIND_TALKS_SPY_FOX_1);
				}

				for (const auto& patches : SF1_VO_INDEX)
				{
					building::Talkie& sf1Talkie = sf1Talkies[patches.first];
					building::Talkie& sf3Talkie = sf3Talkies[patches.second];

					sf3Talkie.SetAudioData(sf1Talkie.GetAudioData());
					if (sf1Talkie.HasSBNGData() && sf3Talkie.HasSBNGData())
					{
						sf3Talkie.SetSBNGData(sf1Talkie.GetSBNGData());
					}
				}
			}

			m_fProgress.store(0.75f);

			SetBusyWith(PATCHER_BUSY_WITH_BUILDING_SCRIPTS);

			if (!scriptBuilder.Build())
			{
				throw std::runtime_error(PATCHER_ERROR_BUILD_SCRIPTS_SPY_FOX_3);
			}

			m_fProgress.store(0.82f);

			SetBusyWith(PATCHER_BUSY_WITH_BUILDING_INDEX);

			if (!he0Builder.Build())
			{
				throw std::runtime_error(PATCHER_ERROR_BUILD_HE0_SPY_FOX_3);
			}

			m_fProgress.store(0.90f);

			SetBusyWith(PATCHER_BUSY_WITH_SAVING);

			fs::path newArchiveFolderPath = m_SpyFox3Path;
			file::CreateFolder(newArchiveFolderPath);
			for (std::unique_ptr<archive::Archive>& archive : spyfox3.GetArchives())
			{
				core::DataStream data;
				archive->Build(data);
				fs::path newArchivePath = newArchiveFolderPath.string() + "/" + archive->GetName();
				file::SaveFile(newArchivePath, data);
			}

			m_fProgress.store(1.0f);

			m_bPatchFinished.store(true);
		}
		catch (const std::exception& e)
		{
			m_sFailReason = e.what();
			m_bPatchFailed.store(true);
		}
	}

	//======================================================================================
	float Patcher::GetProgress() const
	{
		return m_fProgress.load();
	}

	//======================================================================================
	bool Patcher::IsFinished() const
	{
		return m_bPatchFinished.load();
	}

	//======================================================================================
	bool Patcher::HasFailed() const
	{
		return m_bPatchFailed.load();
	}

	//======================================================================================
	const std::string& Patcher::GetFailReason() const
	{
		return m_sFailReason;
	}

	//======================================================================================
	void Patcher::SetPatchState(PatchState a_ePatchState)
	{
		m_ePatchState = a_ePatchState;
		m_fnOnPatchStateChanged(m_ePatchState);
	}

	//======================================================================================
	const core::Event<PatchState>& Patcher::GetOnPatchStateChanged() const
	{
		return m_fnOnPatchStateChanged;
	}

	//======================================================================================
	void Patcher::SetSavePath(const fs::path& a_SavePath)
	{
		m_SavePath = a_SavePath;
	}

	//======================================================================================
	const fs::path& Patcher::GetSavePath() const
	{
		return m_SavePath;
	}

	//======================================================================================
	const fs::path& Patcher::GetSpyFox1Path() const
	{
		return m_SpyFox1Path;
	}

	//======================================================================================
	void Patcher::SetSpyFox1Path(const fs::path& a_SpyFox1Path)
	{
		m_SpyFox1Path = a_SpyFox1Path;

		SaveSettings();
	}

	//======================================================================================
	const fs::path& Patcher::GetSpyFox2Path() const
	{
		return m_SpyFox2Path;
	}

	//======================================================================================
	void Patcher::SetSpyFox2Path(const fs::path& a_SpyFox2Path)
	{
		m_SpyFox2Path = a_SpyFox2Path;

		SaveSettings();
	}

	//======================================================================================
	const fs::path& Patcher::GetSpyFox3Path() const
	{
		return m_SpyFox3Path;
	}

	//======================================================================================
	void Patcher::SetSpyFox3Path(const fs::path& a_SpyFox3Path)
	{
		m_SpyFox3Path = a_SpyFox3Path;

		SaveSettings();
	}

	//======================================================================================
	void Patcher::LoadSettings()
	{
		core::Data data;
		file::LoadFile(m_SavePath.string() + SAVE_FILE, data);

		rapidjson::Document document;
		document.Parse(reinterpret_cast<char*>(data.data()), data.size());

		if (document.HasParseError())
		{
			return;
		}

		std::string spyfox1Path;
		rapidjson::GetString(document, SPYFOX_1_PATH, spyfox1Path);
		m_SpyFox1Path = spyfox1Path;

		std::string spyfox2Path;
		rapidjson::GetString(document, SPYFOX_2_PATH, spyfox2Path);
		m_SpyFox2Path = spyfox2Path;

		std::string spyfox3Path;
		rapidjson::GetString(document, SPYFOX_3_PATH, spyfox3Path);
		m_SpyFox3Path = spyfox3Path;
	}

	//======================================================================================
	void Patcher::SaveSettings() const
	{
		rapidjson::Document document;
		document.SetObject();
		rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

		document.AddMember(
			rapidjson::Value(SPYFOX_1_PATH, allocator),
			rapidjson::Value(m_SpyFox1Path.string().c_str(), allocator),
			allocator
		);

		document.AddMember(
			rapidjson::Value(SPYFOX_2_PATH, allocator),
			rapidjson::Value(m_SpyFox2Path.string().c_str(), allocator),
			allocator
		);

		document.AddMember(
			rapidjson::Value(SPYFOX_3_PATH, allocator),
			rapidjson::Value(m_SpyFox3Path.string().c_str(), allocator),
			allocator
		);

		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
		document.Accept(writer);

		file::SaveFile(m_SavePath.string() + SAVE_FILE, core::Data(buffer.GetString(), buffer.GetSize()));
	}

	//======================================================================================
	const std::string& Patcher::GetVersion() const
	{
		return m_sVersion;
	}

	//======================================================================================
	std::vector<Image>& Patcher::GetBackgroundImages()
	{
		return m_aBackgroundImages;
	}

	//======================================================================================
	const core::Data& Patcher::GetSong() const
	{
		return m_Song;
	}

	//======================================================================================
	const core::Data& Patcher::GetEndSound() const
	{
		return m_EndSound;
	}

	//======================================================================================
	std::string Patcher::GetBusyWith() const
	{
		std::lock_guard lock(m_BusyWithMutex);
		return m_sBusyWith;
	}

	//======================================================================================
	void Patcher::SetBusyWith(const std::string& a_sMessage)
	{
		std::lock_guard lock(m_BusyWithMutex);
		m_sBusyWith = a_sMessage;
	}
}