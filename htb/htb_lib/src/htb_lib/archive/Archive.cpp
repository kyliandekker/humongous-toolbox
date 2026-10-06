#include "./Archive.h"

#include <htb_lib/archive/ArchiveType.h>
#include <htb_lib/core/Data.h>
#include <htb_lib/core/DataStream.h>
#include <htb_lib/core/Log.h>
#include <htb_lib/core/Memory.h>
#include <htb_lib/file/file.h>
#include <htb_lib/parsing/Chunk.h>
#include <htb_lib/parsing/ChunkParser.h>

namespace htb::archive
{
	//======================================================================================
	// Archive
	//======================================================================================
	bool Archive::Load(const fs::path& a_Path)
	{
		fs::path sanitizedPath = a_Path.lexically_normal();
		if (!fs::exists(sanitizedPath))
		{
			core::Log(core::ELogLevel::_ERROR, "Failed to load: \"" + sanitizedPath.string() + "\": Path did not exist.");
			return false;
		}

		if (!fs::is_regular_file(sanitizedPath))
		{
			core::Log(core::ELogLevel::_ERROR, "Failed to load: \"" + sanitizedPath.string() + "\": Path was not a file.");
			return false;
		}

		std::string extension = sanitizedPath.extension().string().substr(1);

		// Unknown or Folder.
		EArchiveType archiveType = archive::GetArchiveTypeFromExtension(extension);
		if (archiveType < archive::EArchiveType::HE0)
		{
			core::Log(core::ELogLevel::_ERROR, "Failed to load: \"" + sanitizedPath.string() + "\": Unsupported archive type.");
			return false;
		}

		m_eType = archiveType;

		core::Data data;
		if (!file::LoadFile(sanitizedPath, data))
		{
			core::Log(core::ELogLevel::_ERROR, "Failed to load: \"" + sanitizedPath.string() + "\": Could not read file.");
			return false;
		}

		if (data.empty())
		{
			core::Log(core::ELogLevel::_ERROR, "Failed to load: \"" + sanitizedPath.string() + "\": Archive file is empty.");
			return false;
		}

		m_pRoot = std::make_unique<parsing::Chunk>();
		if (!parsing::ParseArchive(*m_pRoot, data))
		{
			core::Data xorredDataContainer = data;
			unsigned char* xorredData = xorredDataContainer.dataAs<unsigned char>();

			m_pRoot->SetEncrypted(true);
			m_pRoot->SetEncryptionKey(0x69);

			core::xorShift(xorredData, xorredDataContainer.size(), m_pRoot->GetEncryptionKey());
			if (!parsing::ParseArchive(*m_pRoot, xorredDataContainer))
			{
				core::Log(core::ELogLevel::_ERROR, "Failed to load: \"" + sanitizedPath.string() + "\": Failed to parse archive data.");
				return false;
			}
		}

		m_sName = sanitizedPath.filename().string();
		core::Log(core::ELogLevel::SUCCESS, "Successfully loaded: \"" + sanitizedPath.string() + "\".");
		return true;
	}

	//======================================================================================
	void Archive::Build(core::DataStream& a_Data, bool a_bEncrypt) const
	{
		if (!m_pRoot)
		{
			a_Data.Free();
			return;
		}

		const size_t size = m_pRoot->ChunkSize();
		if (size == 0)
		{
			a_Data.Free();
			return;
		}

		a_Data = core::DataStream(size);
		m_pRoot->ToData(a_Data);

		if (a_bEncrypt && m_pRoot->IsEncrypted())
		{
			unsigned char* data = a_Data.dataAs<unsigned char>();
			core::xorShift(data, a_Data.size(), m_pRoot->GetEncryptionKey());
		}
	}

	//======================================================================================
	const parsing::Chunk& Archive::GetRoot() const
	{
		return *m_pRoot.get();
	}

	//======================================================================================
	parsing::Chunk& Archive::GetRoot()
	{
		return *m_pRoot.get();
	}

	//======================================================================================
	EArchiveType Archive::GetType() const
	{
		return m_eType;
	}
}
