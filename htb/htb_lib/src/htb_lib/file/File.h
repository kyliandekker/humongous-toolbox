#pragma once

#include <cstdio>

#include <htb_lib/file/FILEPCH.h>

namespace htb::core
{
	class Data;
}
namespace htb::file
{
	/// <summary>
	/// Platform independent open-file.
	/// </summary>
	/// <param name="a_Path">The file to open.</param>
	/// <param name="a_Mode">The fopen mode string (e.g. "rb", "wb").</param>
	/// <returns>The opened file, or nullptr on failure.</returns>
	FILE* OpenFile(const fs::path& a_Path, const char* a_Mode);

	/// <summary>
	/// Loads a file and puts it in a data stream.
	/// </summary>
	/// <param name="a_Path">The file to open.</param>
	/// <param name="a_Data">The data container to save file info into.</param>
	/// <returns>True if operation was successful, otherwise false.</returns>
	bool LoadFile(const fs::path& a_Path, core::Data& a_Data);

	/// Saves a data container to a file.
	/// </summary>
	/// <param name="a_Path">The file path to save to.</param>
	/// <param name="a_Data">The data container containing the data to save.</param>
	/// <returns>True if operation was successful, otherwise false.</returns>
	bool SaveFile(const fs::path& a_Path, const core::Data& a_Data);

	/// <summary>
	/// Opens the explorer in a specified path.
	/// </summary>
	/// <param name="a_sPath">The directory to create.</param>
	/// <returns>True if successful, false otherwise.</returns>
	bool CreateFolder(const fs::path& a_Path);
}