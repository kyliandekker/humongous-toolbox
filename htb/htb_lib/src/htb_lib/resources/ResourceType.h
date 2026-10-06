#pragma once

namespace htb::resources
{
	//======================================================================================
	// EResourceType
	//======================================================================================
	/// <summary>
	/// The recognized resource type of a game asset.
	/// </summary>
	enum class EResourceType
	{
		NONE,

		TALKIE,
		SONG,
		SFX,

		ROOM_BACKGROUND,
		ROOM_IMAGE,

		SCRIPT
	};
}