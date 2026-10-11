#pragma once

#include <htb_lib_patcher/patch/Patcher.h>

#include "htb_ozonlaag/ozonlaag/OzonlaagPatcherSettings.h"

namespace htb::ozonlaag
{
	class OzonlaagPatcher;
	extern OzonlaagPatcher* GetOzonlaagPatcher();

	/// <summary>
	/// Specific patcher for SpyFox 3: Operatie Ozonlaag.
	/// There is a virtual base class because we might in the future want to localize the minigames.
	/// </summary>
	class OzonlaagPatcher : public patch::Patcher
	{
	public:
		/// <summary>
		/// Constructs the Ozonlaag patcher.
		/// </summary>
		OzonlaagPatcher();

		/// <summary>
		/// Initializes the Ozonlaag patcher.
		/// </summary>
		void Initialize();

		/// <summary>
		/// Starts the patch process on a background thread.
		/// </summary>
		void ApplyPatch() override;

		/// <summary>
		/// Retrieves the patcher settings.
		/// </summary>
		/// <returns>Const reference to the patche settings.</returns>
		const OzonlaagPatcherSettings& GetSettings() const;

		/// <summary>
		/// Retrieves the patcher settings.
		/// </summary>
		/// <returns>Reference to the patche settings.</returns>
		OzonlaagPatcherSettings& GetSettings();

		/// <summary>
		/// Saves the settings.
		/// </summary>
		void SaveSettings() const;

		/// <summary>
		/// Loads the settings.
		/// </summary>
		void LoadSettings();
	private:
		OzonlaagPatcherSettings m_Settings;
	};
}