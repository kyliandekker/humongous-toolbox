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
	// Talkie
	//======================================================================================
	/// <summary>
	/// Contains the info for a talk and can rebuild.
	/// </summary>
	class Talkie : public GameResource
	{
	public:
		/// <summary>
		/// Constructs an empty talk. Not valid.
		/// </summary>
		Talkie();

		/// <summary>
		/// Constructs a talk. Provided chunk MUST be SGEN.
		/// </summary>
		/// <param name="a_SGENChunk">The SGEN chunk.</param>
		Talkie(parsing::Chunk& a_SGENChunk);

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
		/// Returns the audio data (PCM).
		/// </summary>
		/// <returns>Data container with byte data.</returns>
		const core::Data GetSBNGData() const;

		/// <summary>
		/// Sets the SBNG data.
		/// </summary>
		/// <param name="a_Data">Data container with byte data.</param>
		void SetSBNGData(const core::Data& a_Data);

		/// <summary>
		/// Checks whether the talkie has SBNG data.
		/// </summary>
		/// <returns>True if SBNG data was found, false otherwise.</returns>
		bool HasSBNGData() const;

		/// <summary>
		/// Retrieves the sample rate.
		/// </summary>
		/// <returns>The sample rate of the audio.</returns>
		uint16_t GetSampleRate() const;
	private:
		parsing::Chunk* m_pTALKChunk = nullptr; // Container chunk.
		parsing::Chunk* m_pHSHDChunk = nullptr; // Audio info like sample rate. Inside container chunk.
		parsing::Chunk* m_pSBNGChunk = nullptr; // Audio info, unsure what exactly. Inside container chunk.
		parsing::Chunk* m_pSDATChunk = nullptr; // Audio data chunk. Inside container chunk.
	};
}