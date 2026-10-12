#pragma once

#include <memory>

#include <htb_lib_patcher/patch/Patcher.h>

#include "htb_ozonlaag/ozonlaag/OzonlaagInfo.h"
#include "htb_ozonlaag/ozonlaag/OzonlaagPatcherSettings.h"

namespace htb::archive
{
	class Archive;
}
namespace htb::building
{
	class SFX;
	class Song;
	class Talkie;
}
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
		~OzonlaagPatcher();

		/// <summary>
		/// Constructs the Ozonlaag patcher.
		/// </summary>
		OzonlaagPatcher();

		/// <summary>
		/// Gets the version string of the patcher.
		/// </summary>
		/// <returns>A const reference to the version string.</returns>
		const std::string& GetVersion() const override;

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
		/// <summary>
		/// Initializes the Ozonlaag patcher.
		/// </summary>
		void OnInitialized() override;

		/// <summary>
		/// Starts the patch process on a background thread.
		/// </summary>
		void OnPatchApplied() override;

		/// <summary>
		/// Loads the patching song.
		/// </summary>
		void LoadSong();

		/// <summary>
		/// Loads the end sound.
		/// </summary>
		void LoadEndSound();

		/// <summary>
		/// Loads the images for the patcher.
		/// </summary>
		/// <param name="a_pArchive">The archive to fetch images from.</param>
		void LoadWaitingImages(archive::Archive& a_pArchive);

		/// <summary>
		/// Replaces the Talkies in an archive with the ones from a source archive.
		/// </summary>
		/// <param name="a_aTargetTalkies">The target HE2 talkies.</param>
		/// <param name="a_pSourceArchive">The source HE2 archive.</param>
		/// <param name="a_mMap">The map of which TALKies to replace.</param>
		void ReplaceTalkies(
			std::vector<building::Talkie>& a_aTargetTalkies,
			archive::Archive& a_pSourceArchive, // TODO: This is a design flaw in htb_lib. Source should be const.
			const TALKIE_MAP& a_mMap,
			const std::string a_sEmptyError,
			const std::string a_sNotFoundErrorSource,
			const std::string a_sNotFoundErrorTarget
		); 

		OzonlaagPatcherSettings m_Settings;
		std::string m_sOzonlaagVersion = "htb_ozonlaag 0.0.1b";

		std::unique_ptr<building::SFX> m_pEndSound;
		std::unique_ptr<building::Song> m_pSong;
	};
}