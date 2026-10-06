#pragma once

#include <atomic>
#include <mutex>
#include <thread>
#include <string>
#include <vector>

#include <htb_lib/file/FILEPCH.h>

#include "htb_patcher/Image.h"
#include "htb_patcher/Event.h"

namespace htb::patch
{
	enum class PatchState
	{
		NONE,
		PATCHING,
		FINISHED,
		FINALIZED,
		FAILED
	};

	class Patcher;
	extern Patcher& GetPatcher();

	//======================================================================================
	// Patcher
	//======================================================================================
	/// <summary>
	/// Patches the game and holds all application info.
	/// </summary>
	class Patcher
	{
	public:
		/// <summary>
		/// Starts the patch process on a background thread.
		/// </summary>
		void ApplyPatch();

		/// <summary>
		/// Waits for the background patch thread to finish.
		/// </summary>
		void Join();

		/// <summary>
		/// Gets the current patch progress as a value between 0.0 and 1.0.
		/// </summary>
		/// <returns>The progress percentage.</returns>
		float GetProgress() const;

		/// <summary>
		/// Checks whether the patch process has finished.
		/// </summary>
		/// <returns>True if finished, otherwise false.</returns>
		bool IsFinished() const;

		/// <summary>
		/// Checks whether the patch process has failed.
		/// </summary>
		/// <returns>True if failed, otherwise false.</returns>
		bool HasFailed() const;

		/// <summary>
		/// Gets the reason for a patch failure.
		/// </summary>
		/// <returns>A const reference to the fail reason string.</returns>
		const std::string& GetFailReason() const;

		/// <summary>
		/// Sets the current patch state and notifies all listeners.
		/// </summary>
		/// <param name="a_ePatchState">The new patch state to set.</param>
		void SetPatchState(PatchState a_ePatchState);

		/// <summary>
		/// Gets the event that fires when the patch state changes.
		/// </summary>
		/// <returns>A const reference to the patch state changed event.</returns>
		const core::Event<PatchState>& GetOnPatchStateChanged() const;

		/// <summary>
		/// Sets the directory path where settings are saved.
		/// </summary>
		/// <param name="a_SavePath">The directory path for saving settings.</param>
		void SetSavePath(const fs::path& a_SavePath);

		/// <summary>
		/// Gets the directory path where settings are saved.
		/// </summary>
		/// <returns>A const reference to the save path.</returns>
		const fs::path& GetSavePath() const;

		/// <summary>
		/// Gets the installation path of Spy Fox 1.
		/// </summary>
		/// <returns>A const reference to the Spy Fox 1 path.</returns>
		const fs::path& GetSpyFox1Path() const;

		/// <summary>
		/// Sets the installation path of Spy Fox 1 and saves settings.
		/// </summary>
		/// <param name="a_SpyFox1Path">The path to the Spy Fox 1 installation.</param>
		void SetSpyFox1Path(const fs::path& a_SpyFox1Path);

		/// <summary>
		/// Gets the installation path of Spy Fox 2.
		/// </summary>
		/// <returns>A const reference to the Spy Fox 2 path.</returns>
		const fs::path& GetSpyFox2Path() const;

		/// <summary>
		/// Sets the installation path of Spy Fox 2 and saves settings.
		/// </summary>
		/// <param name="a_SpyFox2Path">The path to the Spy Fox 2 installation.</param>
		void SetSpyFox2Path(const fs::path& a_SpyFox2Path);

		/// <summary>
		/// Gets the installation path of Spy Fox 3.
		/// </summary>
		/// <returns>A const reference to the Spy Fox 3 path.</returns>
		const fs::path& GetSpyFox3Path() const;

		/// <summary>
		/// Sets the installation path of Spy Fox 3 and saves settings.
		/// </summary>
		/// <param name="a_SpyFox3Path">The path to the Spy Fox 3 installation.</param>
		void SetSpyFox3Path(const fs::path& a_SpyFox3Path);

		/// <summary>
		/// Loads the settings from the appData path.
		/// </summary>
		void LoadSettings();

		/// <summary>
		/// Saves the settings to the appData path.
		/// </summary>
		void SaveSettings() const;

		/// <summary>
		/// Gets the version string of the patcher.
		/// </summary>
		/// <returns>A const reference to the version string.</returns>
		const std::string& GetVersion() const;

		/// <summary>
		/// Gets the current status message describing what the patcher is doing.
		/// </summary>
		/// <returns>The current status message.</returns>
		std::string GetBusyWith() const;

		/// <summary>
		/// Returns a vector of background images for in the patcher UI.
		/// </summary>
		/// <returns>Vector of background images.</returns>
		std::vector<Image>& GetBackgroundImages();

		/// <summary>
		/// Returns the song for the patcher.
		/// </summary>
		/// <returns>Byte data from the song.</returns>
		const core::Data& GetSong() const;

		/// <summary>
		/// Returns the sound that plays when the patcher is done.
		/// </summary>
		/// <returns>Byte data from the song.</returns>
		const core::Data& GetEndSound() const;
	private:
		/// <summary>
		/// The main patch routine that runs on the background thread.
		/// </summary>
		void RunPatch();

		/// <summary>
		/// Thread-safe setter for the busy status message.
		/// </summary>
		void SetBusyWith(const std::string& a_sMessage);

		std::vector<Image> m_aBackgroundImages;

		core::Event<PatchState> m_fnOnPatchStateChanged;
		PatchState m_ePatchState = PatchState::NONE;
		fs::path m_SavePath;

		std::string m_sVersion = "htb_patcher v0.0.1b";

		fs::path m_SpyFox1Path;
		fs::path m_SpyFox2Path;
		fs::path m_SpyFox3Path;

		core::Data m_Song;
		core::Data m_EndSound;

		std::thread m_PatchThread;
		std::atomic<float> m_fProgress{0.0f};
		mutable std::mutex m_BusyWithMutex;
		std::string m_sBusyWith;
		std::atomic<bool> m_bPatchFinished{false};
		std::atomic<bool> m_bPatchFailed{false};
		std::string m_sFailReason;
	};
}