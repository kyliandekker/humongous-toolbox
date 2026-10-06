#pragma once

namespace htb::building
{
	//======================================================================================
	// GameResource
	//======================================================================================
	/// <summary>
	/// This class represents a game resource.
	/// Could be a script, image, song, etc.
	/// </summary>
	class GameResource
	{
	public:
		/// <summary>
		/// Returns whether the resource was correctly loaded or not.
		/// Used for determining whether a resource can be rebuilt or inspected.
		/// </summary>
		/// <returns>True if the resource was loaded successfully, false otherwise.</returns>
		bool IsValid() const
		{
			return m_bValid;
		}
	protected:
		bool m_bValid = false;
	};
}