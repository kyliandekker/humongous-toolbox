#include "./OzonlaagPatcher.h"

#include <chrono>

#include <htb_lib_patcher/patch/PatcherState.h>

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
    const std::string& OzonlaagPatcher::GetVersion() const
    {
		return m_sOzonlaagVersion;
    }

	//======================================================================================
	void OzonlaagPatcher::OnInitialized()
	{
		LoadSettings();
	}

	//======================================================================================
	void OzonlaagPatcher::OnPatchApplied()
	{
		std::this_thread::sleep_for(std::chrono::seconds(4));

		throw std::runtime_error("Could not find SDAT in TALK of Spy Fox 3.");
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