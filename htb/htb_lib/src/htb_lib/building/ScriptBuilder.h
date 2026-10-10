#pragma once

#include <htb_lib/building/BuilderBase.h>

#include <vector>

#include <htb_lib/script/OPCodesHENew.h>
#include <htb_lib/building/resources/Script.h>

namespace htb::archive
{
	class Archive;
}
namespace htb::parsing
{
	class Chunk;
}
namespace htb::building
{
	//======================================================================================
	// ScriptBuilder
	//======================================================================================
	/// <summary>
	/// Rebuilds scripts by updating their TALK data.
	/// </summary>
	class ScriptBuilder : public BuilderBase
	{
	public:
		/// <summary>
		/// Associates the chunks before rebuilding other archive this archive is dependent on.
		/// </summary>
		bool Bind(archive::ArchiveSet& a_ArchiveSet) override;

		/// <summary>
		/// Builds the chunks in the associated archive.
		/// </summary>
		bool Build() override;
	protected:
		archive::Archive* m_pA = nullptr;
		archive::Archive* m_pHE2 = nullptr;

		script::OPCodeMap m_mOPCodeMap;
		std::vector<Script> m_aTalkScripts;
	};
}