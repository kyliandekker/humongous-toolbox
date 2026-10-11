#include "./OzonlaagPatcherSettings.h"

namespace htb::patch
{
	//======================================================================================
	// OzonlaagPatcherSettings
	//======================================================================================
	const fs::path& OzonlaagPatcherSettings::GetSpyFox1Path() const
	{
		return m_SpyFox1Path;
	}

	//======================================================================================
	void OzonlaagPatcherSettings::SetSpyFox1Path(const fs::path& a_SpyFox1Path)
	{
		m_SpyFox1Path = a_SpyFox1Path;
	}

	//======================================================================================
	const fs::path& OzonlaagPatcherSettings::GetSpyFox2Path() const
	{
		return m_SpyFox2Path;
	}

	//======================================================================================
	void OzonlaagPatcherSettings::SetSpyFox2Path(const fs::path& a_SpyFox2Path)
	{
		m_SpyFox2Path = a_SpyFox2Path;
	}

	//======================================================================================
	const fs::path& OzonlaagPatcherSettings::GetSpyFox3Path() const
	{
		return m_SpyFox3Path;
	}

	//======================================================================================
	void OzonlaagPatcherSettings::SetSpyFox3Path(const fs::path& a_SpyFox3Path)
	{
		m_SpyFox3Path = a_SpyFox3Path;
	}
}