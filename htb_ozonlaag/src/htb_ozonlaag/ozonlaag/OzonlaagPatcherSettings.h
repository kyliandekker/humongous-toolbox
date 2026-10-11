#pragma once

#include <htb_lib/file/FILEPCH.h>

namespace htb::ozonlaag
{
	/// <summary>
	/// Holds all the settings for the Ozone patch.
	/// </summary>
	class OzonlaagPatcherSettings
	{
	public:
		const fs::path& GetSpyFox1Path() const;
		void SetSpyFox1Path(const fs::path& a_SpyFox1Path);

		const fs::path& GetSpyFox2Path() const;
		void SetSpyFox2Path(const fs::path& a_SpyFox2Path);

		const fs::path& GetSpyFox3Path() const;
		void SetSpyFox3Path(const fs::path& a_SpyFox3Path);
	private:
		fs::path m_SpyFox1Path;
		fs::path m_SpyFox2Path;
		fs::path m_SpyFox3Path;
	};
}