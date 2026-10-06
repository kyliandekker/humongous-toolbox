#include "./Song.h"

#include <htb_lib/core/Log.h>
#include <htb_lib/parsing/Chunk.h>
#include <htb_lib/parsing/ChunkIDs.h>

namespace htb::building
{
#pragma pack(push, 1)
	//======================================================================================
	// SGENData
	//======================================================================================
	// This chunk is a pointer chunk that appears several times at the start of the Song file (*.HE4).
	// It says where the song related to the SGEN appears and what the size is.
	// SGENs are not always in order of appearance.
	struct SGENData
	{
		uint32_t id = 0; // For some reason it does not start at 0 most of the time.
		uint32_t songPos = 0; // Direct pointer to the DIGI header of the song.
		uint32_t songSize = 0; // This is the ENTIRE DIGI chunk.
		uint8_t padding = 0;
	};

	//======================================================================================
	// HSHDData
	//======================================================================================
	// This is a HSHD chunk. The chunk appears in every chunk that contains sound data.
	// It appears in TALK chunks and DIGI chunks.
	// It describes info about the sound data, such as what the sample rate is.
	struct HSHDData
	{
	public:
		unsigned char unknown1[2] = {
			0,
			0
		};
		uint16_t unknown2 = 32896;
		uint16_t unknown3 = 65535;
		uint16_t sampleRate = 11025;
		unsigned char unknown4 = 0; // We need to figure out what this is, because unlike the other ones, this one is different every time.
		unsigned char unknown5 = 0;
		unsigned char unknown6[2] = {
			0,
			0
		};
		uint32_t unknown7 = 6747836;
	};
#pragma pack(pop)

	//======================================================================================
	// Song
	//======================================================================================
	Song::Song() : GameResource()
	{}

	//======================================================================================
	Song::Song(parsing::Chunk& a_SGENChunk) : GameResource(),
		m_pSGENChunk(&a_SGENChunk)
	{
		parsing::Chunk* songChunk = m_pSGENChunk->GetRoot();
		if (!songChunk)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not construct song because the root was null.");
			return;
		}

		if (songChunk->GetTag() != parsing::SONG_CHUNK_ID)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not construct song because the SONG chunk could not be found.");
			return;
		}

		const SGENData* sgenData = m_pSGENChunk->GetData().dataAs<const SGENData>();
		if (!sgenData)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not construct song because the SGEN data was null.");
			return;
		}

		m_pDIGIChunk = songChunk->FindChunkAt(sgenData->songPos);
		if (!m_pDIGIChunk)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not construct song because the DIGI chunk could not be found.");
			return;
		}

		m_pHSHDChunk = m_pDIGIChunk->TryFindChild(parsing::HSHD_CHUNK_ID);
		m_pSDATChunk = m_pDIGIChunk->TryFindChild(parsing::SDAT_CHUNK_ID);

		m_bValid = true;
	}

	//======================================================================================
	const core::Data Song::GetAudioData() const
	{
		if (!m_bValid)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not get audio data because the song was not valid.");
			return {};
		}

		if (!m_pSDATChunk)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not get audio data because there was no SDAT chunk.");
			return {};
		}

		return m_pSDATChunk->GetData();
	}

	//======================================================================================
	void Song::SetAudioData(const core::Data& a_Data)
	{
		if (!m_bValid)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not set audio data because the song was not valid.");
			return;
		}

		if (!m_pSDATChunk)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not set audio data because there was no SDAT chunk.");
			return;
		}

		m_pSDATChunk->SetData(a_Data);
	}

	//======================================================================================
	uint16_t Song::GetSampleRate() const
	{
		if (!m_bValid)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not get sample rate because the song was not valid.");
			return 0;
		}

		if (!m_pHSHDChunk)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not get sample rate because there was no HSHD chunk.");
			return 0;
		}

		HSHDData* hshdData = m_pHSHDChunk->GetData().dataAs<HSHDData>();
		if (!hshdData)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not get sample rate because the HSHD data was null.");
			return 0;
		}

		return hshdData->sampleRate;
	}

	//======================================================================================
	void Song::Update()
	{
		if (!m_bValid)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not update song because the song was not valid.");
			return;
		}

		if (!m_pDIGIChunk)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not update song because there was no SDAT chunk.");
			return;
		}

		SGENData* sgenData = m_pSGENChunk->GetData().dataAs<SGENData>();
		if (!sgenData)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not update song because the SGEN data was null.");
			return;
		}

		sgenData->songPos = m_pDIGIChunk->GetOffsetFromRoot();
		sgenData->songSize = m_pDIGIChunk->WholeChunkSize();
	}
}