#include "ExtractCommand.h"

#include <filesystem>
#include <string>

#include <htb_lib/archive/Archive.h>
#include <htb_lib/core/Log.h>
#include <htb_lib/core/DataStream.h>
#include <htb_lib/file/file.h>
#include <htb_lib/parsing/ChunkIDs.h>
#include <htb_lib/building/resources/Talkie.h>
#include <htb_lib/audio/WaveLoader.h>

#include "htb_cli/cmd/CommandParser.h"

namespace htb::cmd
{
	//---------------------------------------------------------------------
	const char* ExtractCommand::GetName() const
	{
		return "extract";
	}

	//---------------------------------------------------------------------
	const char* ExtractCommand::GetDescription() const
	{
		return "Extracts all game resources from a file";
	}

	//---------------------------------------------------------------------
	std::vector<CommandArg> ExtractCommand::GetArgs() const
	{
		return {
			{ "files",  "Archive file to convert",							EArgType::File,   true,  "" },
			{ "output", "Output file or directory",							EArgType::Option, false, "" }
		};
	}

	//---------------------------------------------------------------------
	int ExtractCommand::Execute(int a_iArgc, char* a_sArgv[])
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

		int failures = 0;
		for (const fs::path& filePath : files)
		{
			archive::Archive archive;
			if (!archive.Load(filePath))
			{
				failures++;
				continue;
			}

			fs::path outputPath;
			if (hasOutput)
			{
				outputPath = output;
				if (fs::is_directory(outputPath))
				{
					fs::create_directories(outputPath);
				}
			}
			else
			{
				outputPath = filePath.parent_path() / filePath.extension();
				fs::create_directories(outputPath);
			}

			if (archive.GetType() == archive::EArchiveType::HE2)
			{
				size_t number = 0;
				archive.GetRoot().TryFindChildren(parsing::TALK_CHUNK_ID, [outputPath, &number](parsing::Chunk* chunk)
					{
						building::Talkie talkie(*chunk);
						if (!talkie.IsValid())
						{
							return;
						}

						audio::WaveLoader::Save(outputPath / std::string(std::to_string(number) + ".wav"), talkie.GetAudioData(), talkie.GetSampleRate());
						number++;
					}
				);
			}
		}

		return failures;
	}
}
