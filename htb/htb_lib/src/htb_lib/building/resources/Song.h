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
	// Song
	//======================================================================================
	/// <summary>
	/// Contains the info for a song and can rebuild.
	/// </summary>
	class Song : public GameResource
	{
	public:
		/// <summary>
		/// Constructs an empty song. Not valid.
		/// </summary>
		Song();

		/// <summary>
		/// Constructs a song. Provided chunk MUST be SGEN.
		/// </summary>
		/// <param name="a_SGENChunk">The SGEN chunk.</param>
		Song(parsing::Chunk& a_SGENChunk);

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
		parsing::Chunk* m_pSGENChunk = nullptr; // Header chunk. Points to DIGI.
		parsing::Chunk* m_pDIGIChunk = nullptr; // Container chunk.
		parsing::Chunk* m_pHSHDChunk = nullptr; // Audio info like sample rate. Inside container chunk.
		parsing::Chunk* m_pSDATChunk = nullptr; // Audio data chunk. Inside container chunk.
	};
}