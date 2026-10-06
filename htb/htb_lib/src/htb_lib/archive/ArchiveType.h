#pragma once

#include <string>

namespace htb::archive
{
	//======================================================================================
	// EArchiveType
	//======================================================================================
	/// <summary>
	/// Identifies the type of a Humongous Entertainment archive based on its file extension.
	/// </summary>
	enum class EArchiveType
	{
		UNKNOWN,

		HE0,
		HE1,
		HE2,
		HE3,
		HE4,
		HE7,
		HE8,
		HE9,

		A,
	};

	/// <summary>
	/// Determines the archive type from a file extension string.
	/// </summary>
	/// <param name="a_sExtension">The file extension (without the leading dot), case-insensitive.</param>
	/// <returns>The corresponding ArchiveType, or ArchiveType::Unknown if not recognized.</returns>
	EArchiveType GetArchiveTypeFromExtension(const std::string& a_sExtension);
}