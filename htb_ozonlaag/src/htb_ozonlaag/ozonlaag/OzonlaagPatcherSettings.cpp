#include "./OzonlaagPatcherSettings.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/utils.h>

#include <htb_lib/core/Data.h>
#include <htb_lib/file/file.h>

constexpr const char* USE_SPYFOX_1 = "useSpyFox1";
constexpr const char* USE_SPYFOX_2 = "useSpyFox2";
constexpr const char* SPYFOX_1_PATH = "spyFox1Path";
constexpr const char* SPYFOX_2_PATH = "spyFox2Path";
constexpr const char* SPYFOX_3_PATH = "spyFox3Path";

namespace htb::ozonlaag
{
	//======================================================================================
	// OzonlaagPatcherSettings
	//======================================================================================
	void htb::ozonlaag::OzonlaagPatcherSettings::Load(const fs::path& a_SavePath)
	{
		if (!fs::exists(a_SavePath))
		{
			Save(a_SavePath);
			return;
		}

		core::Data data;
		file::LoadFile(a_SavePath.string(), data);

		rapidjson::Document document;
		document.Parse(reinterpret_cast<char*>(data.data()), data.size());

		if (document.HasParseError())
		{
			return;
		}

		std::string spyfox1Path;
		rapidjson::GetString(document, SPYFOX_1_PATH, spyfox1Path);
		m_SpyFox1Path = spyfox1Path;

		std::string spyfox2Path;
		rapidjson::GetString(document, SPYFOX_2_PATH, spyfox2Path);
		m_SpyFox2Path = spyfox2Path;

		std::string spyfox3Path;
		rapidjson::GetString(document, SPYFOX_3_PATH, spyfox3Path);
		m_SpyFox3Path = spyfox3Path;
	}

	//======================================================================================
	void htb::ozonlaag::OzonlaagPatcherSettings::Save(const fs::path& a_SavePath) const
	{
		rapidjson::Document document;
		document.SetObject();
		rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

		document.AddMember(
			rapidjson::Value(SPYFOX_1_PATH, allocator),
			rapidjson::Value(m_SpyFox1Path.string().c_str(), allocator),
			allocator
		);

		document.AddMember(
			rapidjson::Value(SPYFOX_2_PATH, allocator),
			rapidjson::Value(m_SpyFox2Path.string().c_str(), allocator),
			allocator
		);

		document.AddMember(
			rapidjson::Value(SPYFOX_3_PATH, allocator),
			rapidjson::Value(m_SpyFox3Path.string().c_str(), allocator),
			allocator
		);

		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
		document.Accept(writer);

		file::SaveFile(a_SavePath, core::Data(buffer.GetString(), buffer.GetSize()));
	}

	//======================================================================================
	const fs::path& OzonlaagPatcherSettings::GetSpyFox1Path() const
	{
		return m_SpyFox1Path;
	}

	//======================================================================================
	void OzonlaagPatcherSettings::SetSpyFox1Path(const fs::path& a_SpyFox1Path)
	{
		m_SpyFox1Path = a_SpyFox1Path;
	}

	//======================================================================================
	const fs::path& OzonlaagPatcherSettings::GetSpyFox2Path() const
	{
		return m_SpyFox2Path;
	}

	//======================================================================================
	void OzonlaagPatcherSettings::SetSpyFox2Path(const fs::path& a_SpyFox2Path)
	{
		m_SpyFox2Path = a_SpyFox2Path;
	}

	//======================================================================================
	const fs::path& OzonlaagPatcherSettings::GetSpyFox3Path() const
	{
		return m_SpyFox3Path;
	}

	//======================================================================================
	void OzonlaagPatcherSettings::SetSpyFox3Path(const fs::path& a_SpyFox3Path)
	{
		m_SpyFox3Path = a_SpyFox3Path;
	}
}