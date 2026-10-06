#include "DecryptCommand.h"

#include <filesystem>
#include <string>

#include <htb_lib/archive/Archive.h>
#include <htb_lib/core/Log.h>
#include <htb_lib/core/DataStream.h>
#include <htb_lib/file/file.h>
#include <htb_lib/core/Memory.h>

#include "htb_cli/cmd/CommandParser.h"

namespace htb::cmd
{
	//---------------------------------------------------------------------
	const char* DecryptCommand::GetName() const
	{
		return "decrypt";
	}

	//---------------------------------------------------------------------
	const char* DecryptCommand::GetDescription() const
	{
		return "Decrypt a file and export it";
	}

	//---------------------------------------------------------------------
	std::vector<CommandArg> DecryptCommand::GetArgs() const
	{
		return {
			{ "files",  "Archive file to convert",							EArgType::File,   true,  "" },
			{ "output", "Output file or directory",							EArgType::Option, false, "" },
			{ "key",    "XOR key byte (0-255, decimal or 0x hex, default 0x69)", EArgType::Option, false, "0x69" },
		};
	}

	//---------------------------------------------------------------------
	int DecryptCommand::Execute(int a_iArgc, char* a_sArgv[])
	{
		CommandParser parser(GetArgs());
		if (!parser.Parse(a_iArgc, a_sArgv))
		{
			parser.PrintUsage(GetName(), GetDescription());
			return 1;
		}

		const auto& files = parser.GetFiles();

		fs::path output;
		bool hasOutput = parser.Has("output");
		if (hasOutput)
		{
			output = parser.Get("output");
		}

		// Parse XOR key (decimal or 0x hex). Default 0x69.
		int keyInt = 0x69;
		if (parser.Has("key"))
		{
			std::string keyStr = parser.Get("key");
			try
			{
				size_t pos = 0;
				// base 0: auto-detect 0x prefix, otherwise decimal
				keyInt = std::stoi(keyStr, &pos, 0);
				if (pos != keyStr.size())
				{
					throw std::invalid_argument("trailing characters");
				}
			}
			catch (const std::exception&)
			{
				// Also allow single char like 'a' after quote stripping -> "a" -> size 1
				if (keyStr.size() == 1)
				{
					keyInt = static_cast<unsigned char>(keyStr[0]);
				}
				else
				{
					printf("Invalid value for --key \"%s\": expected 0-255 decimal or 0x hex.\n", keyStr.c_str());
					return 1;
				}
			}
			if (keyInt < 0 || keyInt > 255)
			{
				printf("Invalid value for --key \"%s\": must be 0-255.\n", keyStr.c_str());
				return 1;
			}
		}
		char xorKey = static_cast<char>(keyInt);

		int failures = 0;
		for (const fs::path& filePath : files)
		{
			core::Data archiveData;
			if (!file::LoadFile(filePath, archiveData))
			{
				failures++;
				continue;
			}

			fs::path outputPath;
			if (hasOutput)
			{
				outputPath = output;
				if (fs::is_directory(outputPath) || !fs::exists(outputPath))
				{
					fs::create_directories(outputPath);
				}
			}
			else
			{
				outputPath = filePath.parent_path();
				std::error_code ec;
				fs::create_directories(outputPath, ec);
			}

			fs::path outputFile = outputPath / (filePath.stem().string() + "_decrypted" + filePath.extension().string());
			fs::create_directories(outputFile.parent_path());

			core::Data xorData = archiveData;
			unsigned char* xorredData = xorData.dataAs<unsigned char>();

			core::xorShift(xorredData, archiveData.size(), xorKey);
			if (!file::SaveFile(outputFile, xorData))
			{
				core::Log(core::ELogLevel::_ERROR, "Could not save file: \"" + outputFile.string() + "\".");
				failures++;
				continue;
			}

			core::Log(core::ELogLevel::SUCCESS, "Created: \"" + outputFile.string() + "\".");
		}

		return failures;
	}
}
