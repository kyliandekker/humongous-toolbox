#include "./SFX.h"

#include <htb_lib/building/resources/HSHDData.h>
#include <htb_lib/core/Log.h>
#include <htb_lib/parsing/Chunk.h>
#include <htb_lib/parsing/ChunkIDs.h>

namespace htb::building
{
	//======================================================================================
	// SFX
	//======================================================================================
	SFX::SFX() : GameResource()
	{}

	//======================================================================================
	SFX::SFX(parsing::Chunk& a_DIGIChunk) : GameResource()
	{
		if (a_DIGIChunk.GetTag() != parsing::DIGI_CHUNK_ID)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not construct sfx because the provided chunk was not a DIGI chunk.");
			return;
		}
		m_pDIGIChunk = &a_DIGIChunk;

		m_pHSHDChunk = m_pDIGIChunk->TryFindChild(parsing::HSHD_CHUNK_ID);
		m_pSDATChunk = m_pDIGIChunk->TryFindChild(parsing::SDAT_CHUNK_ID);

		if (!m_pSDATChunk)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not construct sfx because the SDAT chunk could not be found.");
			return;
		}

		m_bValid = true;
	}

	//======================================================================================
	const core::Data SFX::GetAudioData() const
	{
		if (!m_bValid)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not get audio data because the sfx was not valid.");
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
	void SFX::SetAudioData(const core::Data& a_Data)
	{
		if (!m_bValid)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not set audio data because the sfx was not valid.");
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
	uint16_t SFX::GetSampleRate() const
	{
		if (!m_bValid)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not get sample rate because the sfx was not valid.");
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