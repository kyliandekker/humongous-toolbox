#include "./OzonlaagPatcher.h"

namespace htb::ozonlaag
{
	constexpr auto OZONLAAG_SETTINGS_NAME = "settings.conf";

	//======================================================================================
	OzonlaagPatcher* GetOzonlaagPatcher()
	{
		patch::Patcher& patcher = patch::GetPatcher();
		return dynamic_cast<OzonlaagPatcher*>(&patcher);
	}

	//======================================================================================
	// OzonlaagPatcher
	//======================================================================================
	OzonlaagPatcher::OzonlaagPatcher() : patch::Patcher("OperatieOzonlaag")
	{}

	//======================================================================================
	void OzonlaagPatcher::Initialize()
	{
		LoadSettings();
	}

	//======================================================================================
	void OzonlaagPatcher::ApplyPatch()
	{
	}

	//======================================================================================
	const OzonlaagPatcherSettings& OzonlaagPatcher::GetSettings() const
	{
		return m_Settings;
	}

	//======================================================================================
    OzonlaagPatcherSettings& OzonlaagPatcher::GetSettings()
    {
		return m_Settings;
    }

	//======================================================================================
	void OzonlaagPatcher::SaveSettings() const
	{
		m_Settings.Save(m_WorkingPath / OZONLAAG_SETTINGS_NAME);
	}

	//======================================================================================
	void OzonlaagPatcher::LoadSettings()
	{
		m_Settings.Load(m_WorkingPath / OZONLAAG_SETTINGS_NAME);
	}
}