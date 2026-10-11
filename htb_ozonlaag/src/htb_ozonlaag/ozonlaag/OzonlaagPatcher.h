#pragma once

#include <htb_lib_patcher/patch/Patcher.h>

namespace htb::patch
{
	/// <summary>
	/// Specific patcher for SpyFox 3: Operatie Ozonlaag.
	/// There is a virtual base class because we might in the future want to localize the minigames.
	/// </summary>
	class OzonlaagPatcher : public Patcher
	{
	public:
		/// <summary>
		/// Constructs the Ozonlaag patcher.
		/// </summary>
		OzonlaagPatcher();

		/// <summary>
		/// Starts the patch process on a background thread.
		/// </summary>
		void ApplyPatch() override;
	};
}