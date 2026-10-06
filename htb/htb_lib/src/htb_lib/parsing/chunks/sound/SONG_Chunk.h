#pragma once

namespace htb::parsing
{
#pragma pack(push, 1)
	// This is the root chunk of a Song file (*.HE4).
	// Contains SGHD, SGEN, DIGI.
	struct SONG_Chunk : public ChunkHeader
	{ };
#pragma pack(pop)
}