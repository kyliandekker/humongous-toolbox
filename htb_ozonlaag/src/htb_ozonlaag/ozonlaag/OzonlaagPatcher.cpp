#include "./OzonlaagPatcher.h"

namespace htb::ozonlaag
{
	//======================================================================================
	OzonlaagPatcher* GetOzonlaagPatcher()
	{
		patch::Patcher& patcher = patch::GetPatcher();
		return dynamic_cast<OzonlaagPatcher*>(&patcher);
	}

	//======================================================================================
	// OzonlaagPatcher
	//======================================================================================
	OzonlaagPatcher::OzonlaagPatcher() : patch::Patcher("OperatieOzonlaag")
	{}

	//======================================================================================
	void OzonlaagPatcher::ApplyPatch()
	{
	}
}