#pragma once

#include <Windows.h>
#include <mmsystem.h>

#include <cstdint>
#include <vector>

const uint16_t HE_SAMPLE_RATE = 11025;
const uint8_t HE_BITS_PER_SAMPLE = 8;
const uint8_t HE_CHANNELS = 1;

namespace htb::audio
{
	class AudioPlayer
	{
	public:
		AudioPlayer();
		~AudioPlayer();

		AudioPlayer(const AudioPlayer&) = delete;
		AudioPlayer& operator=(const AudioPlayer&) = delete;

		bool Open(
			uint32_t sampleRate,
			uint8_t bitsPerSample,
			uint8_t channels);

		void Close();

		bool IsOpen() const;

		bool Play(const void* data, size_t size);
		void Pause();
		void Resume();
		void Stop();

		bool IsPlaying() const;
		bool IsPaused() const;

		void Seek(size_t position);

		size_t GetBytePosition() const;
		size_t GetTotalBytes() const;

	private:
		bool StartFromPosition(size_t position);

		size_t GetDevicePosition() const;

		void Unprepare();

	private:
		HWAVEOUT m_hWaveOut = nullptr;

		WAVEFORMATEX m_Format{};
		WAVEHDR m_WaveHeader{};

		std::vector<uint8_t> m_aBuffer;

		size_t m_iPosition = 0;

		bool m_bOpen = false;
		bool m_bPlaying = false;
		bool m_bPaused = false;
		bool m_bHeaderPrepared = false;
	};
}