#pragma once

#include <string>
#include <thread>

#include <htb_lib/core/Event.h>
#include <htb_lib/core/Observable.h>
#include <htb_lib/file/FILEPCH.h>

namespace htb::patch
{
	enum class EPatcherState;

	class Patcher;
	extern Patcher& GetPatcher();

	//======================================================================================
	// Patcher
	//======================================================================================
	/// <summary>
	/// Contains the current state of the patch.
	/// Reports errors.
	/// Abstract, needs to be overridden.
	/// </summary>
	class Patcher
	{
	public:
		/// <summary>
		/// Constructs the patcher.
		/// </summary>
		Patcher(const std::string& a_sName);

		/// <summary>
		/// Initializes the patcher.
		/// </summary>
		void Initialize();

		/// <summary>
		/// Starts the patch process on a background thread.
		/// </summary>
		void ApplyPatch();

		/// <summary>
		/// Ends the patch thread.
		/// </summary>
		void Join();

		/// <summary>
		/// Gets the version string of the patcher.
		/// </summary>
		/// <returns>A const reference to the version string.</returns>
		virtual const std::string& GetVersion() const;

		/// <summary>
		/// Retrieves the working path.
		/// </summary>
		/// <returns>The path to the save directory.</returns>
		const fs::path& GetWorkingPath() const;

		/// <summary>
		/// Sets the working path, where all settings and saved data go.
		/// </summary>
		/// <param name="a_WorkingPath">Path to the working folder.</param>
		void SetWorkingPath(const fs::path& a_WorkingPath);

		/// <summary>
		/// Returns an observable patcher state.
		/// </summary>
		/// <returns>Observable patcher state that can be listened to, or read.</returns>
		const core::Observable<EPatcherState>& GetPatcherState() const;

		/// <summary>
		/// Returns observable patcher progress.
		/// </summary>
		/// <returns>Observable patcher progress that can be listened to, or read.</returns>
		const core::Observable<float>& GetPatcherProgress() const;

		/// <summary>
		/// Returns an observable status message.
		/// </summary>
		/// <returns>Observable status message that can be listened to, or read.</returns>
		const core::Observable<std::string>& GetStatusMessage() const;
	private:
		/// <summary>
		/// Initializes patcher-specific functionality.
		/// </summary>
		virtual void OnInitialized() = 0;

		/// <summary>
		/// Starts the patch.
		/// </summary>
		virtual void OnPatchApplied() = 0;
	protected:
		std::string m_sName = "Unknown Patch";
		std::string m_sVersion = "htb_patcher v0.0.1b";
		fs::path m_WorkingPath;

		core::Observable<EPatcherState> m_ePatcherState;
		core::Observable<float> m_fPatcherProgress;
		core::Observable<std::string> m_sStatusMessage;

		std::thread m_PatchThread;
	};
}