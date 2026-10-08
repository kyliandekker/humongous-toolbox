#include "./Talkie.h"

#include <htb_lib/building/resources/HSHDData.h>
#include <htb_lib/core/Log.h>
#include <htb_lib/parsing/Chunk.h>
#include <htb_lib/parsing/ChunkIDs.h>

namespace htb::building
{
	//======================================================================================
	// Talkie
	//======================================================================================
	Talkie::Talkie() : GameResource()
	{}

	//======================================================================================
	Talkie::Talkie(parsing::Chunk& a_Chunk) : GameResource(),
		m_pTALKChunk(&a_Chunk)
	{
		m_pHSHDChunk = m_pTALKChunk->TryFindChild(parsing::HSHD_CHUNK_ID);
		m_pSBNGChunk = m_pTALKChunk->TryFindChild(parsing::SBNG_CHUNK_ID);
		m_pSDATChunk = m_pTALKChunk->TryFindChild(parsing::SDAT_CHUNK_ID);

		if (!m_pSDATChunk)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not construct talkie because the SDAT chunk could not be found.");
			return;
		}

		m_bValid = true;
	}

	//======================================================================================
	const core::Data Talkie::GetAudioData() const
	{
		if (!m_bValid)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not get audio data because the talkie was not valid.");
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
	void Talkie::SetAudioData(const core::Data& a_Data)
	{
		if (!m_bValid)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not set audio data because the talkie was not valid.");
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
	const core::Data Talkie::GetSBNGData() const
	{
		if (!m_bValid)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not get SBNG data because the talkie was not valid.");
			return {};
		}

		if (!m_pSBNGChunk)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not get SBNG data because there was no SBNG chunk.");
			return {};
		}

		return m_pSBNGChunk->GetData();
	}

	//======================================================================================
	void Talkie::SetSBNGData(const core::Data& a_Data)
	{
		if (!m_bValid)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not set SBNG data because the talkie was not valid.");
			return;
		}

		if (!m_pSBNGChunk)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not set SBNG data because there was no SBNG chunk.");
			return;
		}

		m_pSBNGChunk->SetData(a_Data);
	}

	//======================================================================================
	bool Talkie::HasSBNGData() const
	{
		return m_pSBNGChunk && !m_pSBNGChunk->GetData().empty();
	}

	//======================================================================================
	uint16_t Talkie::GetSampleRate() const
	{
		if (!m_bValid)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not get sample rate because the talkie was not valid.");
			return 0;
		}

		if (!m_pHSHDChunk)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not get sample rate because there was no HSHD chunk.");
			return 0;
		}

		const HSHDData* hshdData = m_pHSHDChunk->GetData().dataAs<HSHDData>();
		if (!hshdData)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not get sample rate because the HSHD data was null.");
			return 0;
		}

		return hshdData->sampleRate;
	}
}