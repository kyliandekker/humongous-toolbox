#pragma once

#include <cstdint>

#include <htb_lib/core/Data.h>
#include <htb_lib/building/GameResource.h>

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
	// SFX
	//======================================================================================
	/// <summary>
	/// Contains the info for sfx and can rebuild.
	/// </summary>
	class SFX : public GameResource
	{
	public:
		/// <summary>
		/// Constructs an empty sfx. Not valid.
		/// </summary>
		SFX();

		/// <summary>
		/// Constructs a sfx. Provided chunk MUST be DIGI.
		/// </summary>
		/// <param name="a_DIGIChunk">The DIGI chunk.</param>
		SFX(parsing::Chunk& a_DIGIChunk);

		/// <summary>
		/// Returns the audio data (PCM).
		/// </summary>
		/// <returns>Data container with byte data.</returns>
		const core::Data GetAudioData() const;

		/// <summary>
		/// Sets the audio data (PCM).
		/// </summary>
		/// <param name="a_Data">Data container with byte data.</param>
		void SetAudioData(const core::Data& a_Data);

		/// <summary>
		/// Retrieves the sample rate.
		/// </summary>
		/// <returns>The sample rate of the audio.</returns>
		uint16_t GetSampleRate() const;
	private:
		parsing::Chunk* m_pDIGIChunk = nullptr; // Container chunk.
		parsing::Chunk* m_pHSHDChunk = nullptr; // Audio info like sample rate. Inside container chunk.
		parsing::Chunk* m_pSDATChunk = nullptr; // Audio data chunk. Inside container chunk.
	};
}