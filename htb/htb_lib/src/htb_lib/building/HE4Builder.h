#pragma once

// external
#include <vector>

#include <htb_lib/building/BuilderBase.h>
#include <htb_lib/building/sound/Song.h>

namespace htb::archive
{
	class Archive;
}
namespace htb::parsing
{
	class Chunk;
}
namespace htb::building
{
	//======================================================================================
	// HE4Builder
	//======================================================================================
	/// <summary>
	/// Rebuilds a HE4 with the associated data.
	/// </summary>
	class HE4Builder : public BuilderBase
	{
	public:
		/// <summary>
		/// Associates the chunks before rebuilding other archive this archive is dependent on.
		/// </summary>
		bool Bind(archive::ArchiveSet& a_ArchiveSet) override;

		/// <summary>
		/// Builds the chunks in the associated archive.
		/// </summary>
		bool Build() override;
	protected:
		archive::Archive* m_pHE4 = nullptr;
		std::vector<Song> m_aSongs;
	};
}