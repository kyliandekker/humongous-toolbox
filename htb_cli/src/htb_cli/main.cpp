#include <cstdio>
#include <memory>
#include <string>
#include <vector>

#include <htb_lib/core/Log.h>

#include "htb_cli/cmd/Command.h"
#include "htb_cli/cmd/commands/XmlCommand.h"
#include "htb_cli/cmd/commands/SummaryCommand.h"
#include "htb_cli/cmd/commands/DecryptCommand.h"

static void PrintUsage(const std::vector<std::unique_ptr<htb::cmd::Command>>& a_Commands)
{
	printf("Usage: <command> [options]\n\n");
	printf("Commands:\n");
	for (const auto& cmd : a_Commands)
	{
		printf("  %-12s %s\n", cmd->GetName(), cmd->GetDescription());
	}
	printf("\nRun '<command> --help' for command-specific help.\n");
}

int main(int argc, char* argv[])
{
	htb::core::InitializeLog();

	std::vector<std::unique_ptr<htb::cmd::Command>> commands;
	commands.push_back(std::make_unique<htb::cmd::XmlCommand>());
	commands.push_back(std::make_unique<htb::cmd::SummaryCommand>());
	commands.push_back(std::make_unique<htb::cmd::DecryptCommand>());

	if (argc < 2)
	{
		PrintUsage(commands);
		htb::core::DestroyLog();
		return 1;
	}

	std::string commandName = argv[1];

	for (const auto& cmd : commands)
	{
		if (commandName == cmd->GetName())
		{
			int result = cmd->Execute(argc - 2, argv + 2);
			htb::core::DestroyLog();
			return result;
		}
	}

	printf("Unknown command: %s\n\n", commandName.c_str());
	PrintUsage(commands);

	htb::core::DestroyLog();
	return 1;
}