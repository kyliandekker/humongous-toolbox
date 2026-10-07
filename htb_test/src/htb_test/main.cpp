#include <htb_lib/archive/ArchiveSet.h>
#include <htb_lib/building/HE4Builder.h>
#include <htb_lib/core/Log.h>
#include <htb_lib/building/resources/Song.h>
#include <htb_lib/parsing/ChunkIDs.h>

using namespace htb;

int main()
{
	core::InitializeLog();

	fs::path archivesPath = "C:/Program Files (x86)/Steam/steamapps/common/Spy Fox 3/SPYOZON.HE2";

	archive::ArchiveSet set;

	bool loaded = set.LoadArchives(archivesPath);

	if (!loaded)
	{
		core::DestroyLog();
		return 1;
	}

	archive::Archive* he4 = nullptr;
	archive::Archive* he2 = nullptr;
	archive::Archive* he0 = nullptr;
	archive::Archive* a = nullptr;
	for (std::unique_ptr<archive::Archive>& archive : set.GetArchives())
	{
		if (archive->GetType() == archive::EArchiveType::HE4)
		{
			he4 = archive.get();
		}
		else if (archive->GetType() == archive::EArchiveType::HE2)
		{
			he2 = archive.get();
		}
		else if (archive->GetType() == archive::EArchiveType::HE0)
		{
			he0 = archive.get();
		}
		else if (archive->GetType() == archive::EArchiveType::A)
		{
			a = archive.get();
		}
	}

	if (!he4)
	{
		core::DestroyLog();
		return 1;
	}

	building::HE4Builder he4Builder;
	if (!he4Builder.Bind(set))
	{
		core::Log(core::ELogLevel::_ERROR, "Could not bind HE4.");
		core::DestroyLog();
		return 0;
	}

	he4Builder.Build();

	core::DestroyLog();
	return 0;
}