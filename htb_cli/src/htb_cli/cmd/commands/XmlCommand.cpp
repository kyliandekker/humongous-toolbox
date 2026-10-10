#include "XmlCommand.h"

#include <filesystem>
#include <string>

#include "tinyxml/tinyxml2.h"

#include <htb_lib/archive/Archive.h>
#include <htb_lib/core/Data.h>
#include <htb_lib/core/Log.h>
#include <htb_lib/file/file.h>
#include <htb_lib/xml/XMLCreator.h>
#include <htb_lib/xml/XMLSettings.h>

#include "htb_cli/cmd/CommandParser.h"

namespace htb::cmd
{
	//---------------------------------------------------------------------
	const char* XmlCommand::GetName() const
	{
		return "xml";
	}

	//---------------------------------------------------------------------
	const char* XmlCommand::GetDescription() const
	{
		return "Export archive files as XML";
	}

	//---------------------------------------------------------------------
	std::vector<CommandArg> XmlCommand::GetArgs() const
	{
		return {
			{ "files",  "Archive files to convert",        EArgType::File,   true,  "" },
			{ "depth",  "Max chunk tree depth",            EArgType::Option, false, "" },
			{ "output", "Output file or directory",        EArgType::Option, false, "" },
		};
	}

	//---------------------------------------------------------------------
	int XmlCommand::Execute(int a_iArgc, char* a_sArgv[])
	{
		CommandParser parser(GetArgs());
		if (!parser.Parse(a_iArgc, a_sArgv))
		{
			parser.PrintUsage(GetName(), GetDescription());
			return 1;
		}

		const auto& files = parser.GetFiles();

		int maxDepth = -1;
		if (parser.Has("depth"))
		{
			maxDepth = std::stoi(parser.Get("depth"));
		}

		std::string output = parser.Get("output");
		bool hasOutput = parser.Has("output");

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
				outputPath = filePath.parent_path();
				fs::create_directories(outputPath);
			}

			fs::path outputFile = outputPath / (filePath.filename().string() + ".xml");

			tinyxml2::XMLDocument doc;
			xml::XMLSettings xmlInfo;
			xmlInfo.m_iMaxDepth = maxDepth;
			xml::CreateXMLFromArchive(archive, doc, xmlInfo);

			tinyxml2::XMLPrinter printer;
			doc.Print(&printer);

			core::Data xmlData(printer.CStr(), printer.CStrSize() - 1);
			if (!htb::file::SaveFile(outputFile, xmlData))
			{
				core::Log(core::ELogLevel::_ERROR, "Failed to save " + outputFile.string() + ".");
				failures++;
				continue;
			}

			core::Log(core::ELogLevel::SUCCESS, "Created: \"" + outputFile.string() + "\".");
		}

		return failures;
	}
}