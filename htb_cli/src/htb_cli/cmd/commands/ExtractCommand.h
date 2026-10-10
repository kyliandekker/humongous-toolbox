#pragma once

#include "htb_cli/cmd/Command.h"

namespace htb::cmd
{
	//---------------------------------------------------------------------
	// ExtractCommand
	//---------------------------------------------------------------------
	/// <summary>
	/// Extracts all possible resources out of a file.
	/// </summary>
	class ExtractCommand : public Command
	{
	public:
		const char* GetName() const override;
		const char* GetDescription() const override;
		std::vector<CommandArg> GetArgs() const override;
		int Execute(int a_iArgc, char* a_sArgv[]) override;
	};
}
