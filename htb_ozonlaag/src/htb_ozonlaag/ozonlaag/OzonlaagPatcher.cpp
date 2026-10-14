#include "./OzonlaagPatcher.h"

#include <chrono>

#include <htb_lib/archive/ArchiveSet.h>
#include <htb_lib/building/HE0Builder.h>
#include <htb_lib/building/resources/RoomImage.h>
#include <htb_lib/building/resources/RoomBackground.h>
#include <htb_lib/building/resources/SFX.h>
#include <htb_lib/building/resources/Song.h>
#include <htb_lib/building/resources/Talkie.h>
#include <htb_lib/building/ScriptBuilder.h>
#include <htb_lib/core/Data.h>
#include <htb_lib/core/DataStream.h>
#include <htb_lib/parsing/Chunk.h>
#include <htb_lib/parsing/ChunkIDs.h>

#include <htb_lib_patcher/patch/PatcherState.h>

#include "htb_ozonlaag/ozonlaag/LocText.h"

namespace htb::ozonlaag
{
	constexpr auto OZONLAAG_SETTINGS_NAME = "settings.conf";

	//======================================================================================
	OzonlaagPatcher* GetOzonlaagPatcher()
	{
		patch::Patcher& patcher = patch::GetPatcher();
		return dynamic_cast<OzonlaagPatcher*>(&patcher);
	}

	//======================================================================================
	// OzonlaagPatcher
	//======================================================================================
	OzonlaagPatcher::~OzonlaagPatcher()
	{}

	//======================================================================================
	OzonlaagPatcher::OzonlaagPatcher() : patch::Patcher("OperatieOzonlaag")
	{}

	//======================================================================================
    const std::string& OzonlaagPatcher::GetVersion() const
    {
		return m_sOzonlaagVersion;
    }

	//======================================================================================
	void OzonlaagPatcher::OnInitialized()
	{
		LoadSettings();
	}

	//======================================================================================
	void OzonlaagPatcher::OnPatchApplied()
	{
		m_pSong.reset();

		LoadSong();
		m_fPatcherProgress = 0.025f;
		LoadEndSound();
		m_fPatcherProgress = 0.05f;

		m_sStatusMessage = PATCHER_BUSY_WITH_LOAD_SPY_FOX_3;

		fs::path sf3archivepath = m_Settings.GetSpyFox3Path() / "SPYOZON.(A)";

		archive::ArchiveSet spyfox3;
		if (!spyfox3.LoadArchives(sf3archivepath))
		{
			throw std::runtime_error(PATCHER_ERROR_LOAD_ARCHIVES_SPY_FOX_3);
		}

		archive::Archive* sf3HE2 = nullptr;
		archive::Archive* sf3HE0 = nullptr;
		archive::Archive* sf3A = nullptr;
		for (std::unique_ptr<archive::Archive>& archive : spyfox3.GetArchives())
		{
			if (archive->GetType() == archive::EArchiveType::HE2)
			{
				sf3HE2 = archive.get();
			}
			else if (archive->GetType() == archive::EArchiveType::HE0)
			{
				sf3HE0 = archive.get();
			}
			else if (archive->GetType() == archive::EArchiveType::A)
			{
				sf3A = archive.get();
			}
		}

		if (!sf3HE2)
		{
			throw std::runtime_error(PATCHER_ERROR_LOAD_SPY_FOX_3_HE2);
		}

		if (!sf3HE0)
		{
			throw std::runtime_error(PATCHER_ERROR_LOAD_SPY_FOX_3_HE0);
		}

		if (!sf3A)
		{
			throw std::runtime_error(PATCHER_ERROR_LOAD_SPY_FOX_3_A);
		}

		m_fPatcherProgress = 0.57f;

		LoadWaitingImages(*sf3A);

		m_fPatcherProgress = 0.1f;

		m_sStatusMessage = PATCHER_BUSY_WITH_BINDING_SCRIPTS;

		building::ScriptBuilder scriptBuilder;
		if (!scriptBuilder.Bind(spyfox3))
		{
			throw std::runtime_error(PATCHER_ERROR_BIND_SCRIPTS_SPY_FOX_3);
		}

		m_fPatcherProgress = 0.2f;

		m_sStatusMessage = PATCHER_BUSY_WITH_BINDING_INDEX;

		building::HE0Builder he0Builder;
		if (!he0Builder.Bind(spyfox3))
		{
			throw std::runtime_error(PATCHER_ERROR_BIND_HE0_SPY_FOX_3);
		}

		m_fPatcherProgress = 0.3f;

		m_sStatusMessage = PATCHER_BUSY_WITH_SEARCHING_TALKIES_SPY_FOX_3;

		std::vector<building::Talkie> sf3Talkies;
		sf3HE2->GetRoot().TryFindChildren(parsing::TALK_CHUNK_ID, [&sf3Talkies](parsing::Chunk* chunk)
		{
			sf3Talkies.emplace_back(*chunk);
		});

		if (sf3Talkies.empty())
		{
			throw std::runtime_error(PATCHER_ERROR_LOAD_TALKIES_SPY_FOX_3);
		}

		m_fPatcherProgress = 0.35f;

		m_sStatusMessage = PATCHER_BUSY_WITH_REPLACING_WITH_SPY_FOX_1_TALKIES;

		{
			fs::path sf1archivepath = m_Settings.GetSpyFox1Path() / "SPYFox.HE2";
			archive::Archive sf1HE2;
			if (!sf1HE2.Load(sf1archivepath))
			{
				throw std::runtime_error(PATCHER_ERROR_LOAD_TALKIES_SPY_FOX_1);
			}

			ReplaceTalkies(sf3Talkies, sf1HE2, SF1_TALKIE_INDEX, PATCHER_ERROR_LOAD_TALKIES_SPY_FOX_1, PATCHER_ERROR_REPLACING_TALKIES_OUT_OF_RANGE_SPY_FOX_1, PATCHER_ERROR_REPLACING_TALKIES_OUT_OF_RANGE_SPY_FOX_3);
		}

		m_fPatcherProgress = 0.4f;

		m_sStatusMessage = PATCHER_BUSY_WITH_REPLACING_WITH_SPY_FOX_2_TALKIES;

		{
			fs::path sf2archivepath = m_Settings.GetSpyFox2Path() / "Spyfox2.HE2";
			archive::Archive sf2HE2;
			if (!sf2HE2.Load(sf2archivepath))
			{
				throw std::runtime_error(PATCHER_ERROR_LOAD_SPY_FOX_1_HE2);
			}

			ReplaceTalkies(sf3Talkies, sf2HE2, SF2_TALKIE_INDEX, PATCHER_ERROR_LOAD_TALKIES_SPY_FOX_1, PATCHER_ERROR_REPLACING_TALKIES_OUT_OF_RANGE_SPY_FOX_2, PATCHER_ERROR_REPLACING_TALKIES_OUT_OF_RANGE_SPY_FOX_3);
		}

		m_sStatusMessage = PATCHER_BUSY_WITH_REPLACING_IMAGES_FROM_SPY_FOX_2;

		fs::path sf2archivepath = m_Settings.GetSpyFox2Path() / "Spyfox2.(A)";
		archive::Archive sf2A;
		if (!sf2A.Load(sf2archivepath))
		{
			throw std::runtime_error(PATCHER_ERROR_LOAD_SPY_FOX_1_A);
		}

		std::unordered_map<parsing::Chunk*, parsing::Chunk*> imagesToSF2;
		for (const auto& pair : SF2_ROOM_IMAGE_INDEX)
		{
			imagesToSF2.insert({ sf3A->GetRoot().FindChunkAt(pair.second), sf2A.GetRoot().FindChunkAt(pair.first) });
		}

		std::unordered_map<parsing::Chunk*, parsing::Chunk*> backgroundsToSF2;
		for (const auto& pair : SF2_BACKGROUND_INDEX)
		{
			backgroundsToSF2.insert({ sf3A->GetRoot().FindChunkAt(pair.second), sf2A.GetRoot().FindChunkAt(pair.first) });
		}

		m_fPatcherProgress = 0.5f;

		m_sStatusMessage = PATCHER_BUSY_WITH_REPLACING_BACKGROUND_FROM_SPY_FOX_2;

		ReplaceBackgrounds(backgroundsToSF2);
		ReplaceImages(imagesToSF2);

		m_fPatcherProgress = 0.6f;

		m_sStatusMessage = PATCHER_BUSY_WITH_BUILDING_SCRIPTS;

		if (!scriptBuilder.Build())
		{
			throw std::runtime_error(PATCHER_ERROR_BUILD_SCRIPTS_SPY_FOX_3);
		}

		m_fPatcherProgress = 0.7f;

		m_sStatusMessage = PATCHER_BUSY_WITH_BUILDING_INDEX;

		if (!he0Builder.Build())
		{
			throw std::runtime_error(PATCHER_ERROR_BUILD_HE0_SPY_FOX_3);
		}

		m_fPatcherProgress = 0.8f;

		m_sStatusMessage = PATCHER_BUSY_WITH_SAVING;

		fs::path spyfox3Path = m_Settings.GetSpyFox3Path();
		for (std::unique_ptr<archive::Archive>& archive : spyfox3.GetArchives())
		{
			core::DataStream data;
			archive->Build(data);
			fs::path newArchivePath = spyfox3Path.string() + "/" + archive->GetName();
			file::SaveFile(newArchivePath, data);
		}

		m_fPatcherProgress = 1.f;

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		m_ePatcherState = patch::EPatcherState::FINISHED;
	}

	//======================================================================================
	void OzonlaagPatcher::LoadSong()
	{
		m_sStatusMessage = PATCHER_BUSY_WITH_LOAD_SPY_FOX_1_MUSIC;

		fs::path sf1he4path = m_Settings.GetSpyFox1Path() / "SPYFox.HE4";

		std::unique_ptr<archive::Archive> he4 = std::make_unique<archive::Archive>();
		if (!he4->Load(sf1he4path))
		{
			throw std::runtime_error(PATCHER_ERROR_LOAD_SPY_FOX_1_HE4);
		}

		std::vector<parsing::Chunk*> songs;
		if (!he4->GetRoot().TryFindChildren(parsing::SGEN_CHUNK_ID, songs))
		{
			throw std::runtime_error(PATCHER_ERROR_LOAD_SONGS_SPY_FOX_1);
		}

		constexpr int SPYFOX_SONG = 38;
		if (songs.size() <= SPYFOX_SONG)
		{
			throw std::runtime_error(PATCHER_ERROR_LOAD_SF_SONG_SPY_FOX_1);
		}

		m_pSong = std::make_unique<building::Song>(*songs[SPYFOX_SONG]);

		// Artificial wait because otherwise the status message gets lost.
		std::this_thread::sleep_for(std::chrono::milliseconds(600));
	}

	//======================================================================================
	void OzonlaagPatcher::LoadEndSound()
	{
		m_sStatusMessage = PATCHER_BUSY_WITH_LOAD_SPY_FOX_1_SOUNDS;

		fs::path sf1apath = m_Settings.GetSpyFox1Path() / "SPYFox.(A)";

		std::unique_ptr<archive::Archive> a = std::make_unique<archive::Archive>();
		if (!a->Load(sf1apath))
		{
			throw std::runtime_error(PATCHER_ERROR_LOAD_SPY_FOX_1_A);
		}

		std::vector<parsing::Chunk*> rooms;
		if (!a->GetRoot().TryFindChildren(parsing::LFLF_CHUNK_ID, rooms))
		{
			throw std::runtime_error(PATCHER_ERROR_LOAD_END_ROOMS_SPY_FOX);
		}

		constexpr int END_SOUND_LFLF_ROOM = 8;
		if (rooms.size() <= END_SOUND_LFLF_ROOM)
		{
			throw std::runtime_error(PATCHER_ERROR_LOAD_END_LFLF_SPY_FOX);
		}

		std::vector<parsing::Chunk*> sfx;
		if (!rooms[END_SOUND_LFLF_ROOM]->TryFindChildren(parsing::DIGI_CHUNK_ID, sfx))
		{
			throw std::runtime_error(PATCHER_ERROR_LOAD_END_SOUND_SPY_FOX);
		}

		m_pEndSound = std::make_unique<building::SFX>(*sfx[0]);

		// Artificial wait because otherwise the status message gets lost.
		std::this_thread::sleep_for(std::chrono::milliseconds(600));
	}

	//======================================================================================
	void OzonlaagPatcher::LoadWaitingImages(archive::Archive& a_pArchive)
	{
		m_sStatusMessage = PATCHER_BUSY_WITH_LOADING_BACKGROUND_IMAGES;

		//m_aBackgroundImages.clear();
		//m_aBackgroundImages.reserve(PATCHER_BACKGROUND.size());

		std::vector<parsing::Chunk*> backgrounds;
		a_pArchive.GetRoot().TryFindChildren(parsing::IM00_CHUNK_ID, [](parsing::Chunk* chunk)
		{

		});

		//if (backgrounds.empty())
		//{
		//	throw std::runtime_error(PATCHER_ERROR_FIND_IMAGES_SPY_FOX_3);
		//}
	}

	//======================================================================================
	void OzonlaagPatcher::ReplaceTalkies(std::vector<building::Talkie>& a_aTargetTalkies, archive::Archive& a_pSourceArchive, const TALKIE_MAP& a_mMap,
		const std::string& a_sEmptyError,
		const std::string& a_sNotFoundErrorSource,
		const std::string& a_sNotFoundErrorTarget)
	{
		std::vector<building::Talkie> sourceTalkies;
		a_pSourceArchive.GetRoot().TryFindChildren(parsing::TALK_CHUNK_ID, [&sourceTalkies](parsing::Chunk* chunk)
		{
			sourceTalkies.emplace_back(*chunk);
		});

		if (sourceTalkies.empty())
		{
			throw std::runtime_error(a_sEmptyError);
		}

		for (const auto& patches : a_mMap)
		{
			if (sourceTalkies.size() <= patches.first)
			{
				throw std::runtime_error(a_sNotFoundErrorSource + std::to_string(patches.first));
			}
			building::Talkie& srcTalkie = sourceTalkies[patches.first];
			if (!srcTalkie.IsValid())
			{
				throw std::runtime_error(a_sNotFoundErrorSource + std::to_string(patches.first));
			}
			if (a_aTargetTalkies.size() <= patches.second)
			{
				throw std::runtime_error(a_sNotFoundErrorTarget + std::to_string(patches.second));
			}
			building::Talkie& targetTalkie = a_aTargetTalkies[patches.second];
			if (!targetTalkie.IsValid())
			{
				throw std::runtime_error(a_sNotFoundErrorTarget + std::to_string(patches.second));
			}

			targetTalkie.SetAudioData(srcTalkie.GetAudioData());
			if (srcTalkie.HasSBNGData() && targetTalkie.HasSBNGData())
			{
				targetTalkie.SetSBNGData(srcTalkie.GetSBNGData());
			}
		}
	}

	//======================================================================================
	void OzonlaagPatcher::ReplaceImages(std::unordered_map<parsing::Chunk*, parsing::Chunk*>& a_mChunks)
	{
		for (auto& pair : a_mChunks)
		{
			building::RoomImage targetImage(*pair.first);
			if (!targetImage.IsValid())
			{
				throw std::runtime_error(PATCHER_ERROR_REPLACING_IMAGES_OUT_OF_RANGE_SPY_FOX_3);
			}

			building::RoomImage srcImage(*pair.second);
			if (!srcImage.IsValid())
			{
				throw std::runtime_error(PATCHER_ERROR_REPLACING_IMAGES_OUT_OF_RANGE_SPY_FOX_2);
			}

			targetImage.ReplaceImage(srcImage);
		}
	}

	//======================================================================================
	void OzonlaagPatcher::ReplaceBackgrounds(std::unordered_map<parsing::Chunk*, parsing::Chunk*>& a_mChunks)
	{
		for (auto& pair : a_mChunks)
		{
			building::RoomBackground targetImage(*pair.first);
			if (!targetImage.IsValid())
			{
				throw std::runtime_error(PATCHER_ERROR_REPLACING_BACKGROUNDS_OUT_OF_RANGE_SPY_FOX_3);
			}

			building::RoomBackground srcImage(*pair.second);
			if (!srcImage.IsValid())
			{
				throw std::runtime_error(PATCHER_ERROR_REPLACING_BACKGROUNDS_OUT_OF_RANGE_SPY_FOX_2);
			}

			targetImage.ReplaceBackground(srcImage);
		}
	}

	//======================================================================================
	const OzonlaagPatcherSettings& OzonlaagPatcher::GetSettings() const
	{
		return m_Settings;
	}

	//======================================================================================
    OzonlaagPatcherSettings& OzonlaagPatcher::GetSettings()
    {
		return m_Settings;
    }

	//======================================================================================
	void OzonlaagPatcher::SaveSettings() const
	{
		m_Settings.Save(m_WorkingPath / OZONLAAG_SETTINGS_NAME);
	}

	//======================================================================================
	void OzonlaagPatcher::LoadSettings()
	{
		m_Settings.Load(m_WorkingPath / OZONLAAG_SETTINGS_NAME);
	}
}