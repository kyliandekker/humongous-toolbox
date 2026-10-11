#pragma once

#include <string>

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
		/// Starts the patch process on a background thread.
		/// </summary>
		virtual void ApplyPatch() = 0;

		/// <summary>
		/// Gets the version string of the patcher.
		/// </summary>
		/// <returns>A const reference to the version string.</returns>
		const std::string& GetVersion() const;

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
		/// Returns an observable status message.
		/// </summary>
		/// <returns>Observable status message that can be listened to, or read.</returns>
		const core::Observable<std::string>& GetStatusMessage() const;
	protected:
		std::string m_sName = "Unknown Patch";
		std::string m_sVersion = "htb_patcher v0.0.1b";
		fs::path m_WorkingPath;

		core::Observable<EPatcherState> m_ePatcherState;
		core::Observable<std::string> m_sStatusMessage;
	};
}