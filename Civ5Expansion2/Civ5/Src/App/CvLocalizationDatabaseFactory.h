/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvDllInterfaces.h"
#include "Civ5/Src/CvGameDatabase/Database.h"
#include <memory>
#include <vector>

class cvContentPackage;
class cvPath;

/// size ?
class CvLocalizationDatabaseFactory {
public:
    class LanguageData;
    /// 0x0875a090
    CvLocalizationDatabaseFactory(cvPath& a1, cvPath& a2, cvPath& a3);
    /// 0x0875a146
    UNK_RET DeleteBaseGameLocalizationDatabase();
    /// 0x0875a1f0
    UNK_RET DeleteDatabaseFile(cvPath a1);
    /// 0x0875a232
    UNK_RET GetBaseGameDatabasePath();
    /// 0x0875a268
    UNK_RET DeleteContentLocalizationDatabase(cvContentPackage* a1);
    /// 0x0875a316
    UNK_RET GetDatabasePath(char const* a1);
    /// 0x0875a432
    UNK_RET DeleteAllContentLocalizationDatabases();
    /// 0x0875a48e
    UNK_RET DeleteMergedLocalizationDatabase();
    /// 0x0875a538
    UNK_RET GetMergedDatabasePath();
    /// 0x0875a56e
    UNK_RET DeleteAllLocalizationDatabases();
    /// 0x0875a5b4
    UNK_RET GenerateIndividualLocalizationDatabases();
    /// 0x0875a9a2
    UNK_RET GetBaseGameLanguageFiles(std::vector<cvPath, std::allocator<cvPath> >& a1);
    /// 0x0875ada2
    UNK_RET GenerateBaseGameLocalizationDatabase(char const* a1, std::vector<cvPath, std::allocator<cvPath> >& a2);
    /// 0x0875b2d4
    UNK_RET GenerateDlcLocalizationDatabase(char const* a1, std::vector<cvPath, std::allocator<cvPath> >& a2);
    /// 0x0875b790
    UNK_RET GenerateMergedLocalizationDatabase();
    /// 0x0875bef2
    UNK_RET GetBaseGameLanguages(Database::Connection& a1, std::vector<CvLocalizationDatabaseFactory::LanguageData, std::allocator<CvLocalizationDatabaseFactory::LanguageData> >& a2);
    /// 0x0875c21e
    UNK_RET UpdateBaseGameDatabaseToNewSchema(Database::Connection& a1, std::vector<CvLocalizationDatabaseFactory::LanguageData, std::allocator<CvLocalizationDatabaseFactory::LanguageData> > const& a2);
};

namespace CvUserHistory {
/// 0x0875d430
UNK_RET Initialize();
/// 0x0875d712
UNK_RET Shutdown();
/// 0x0875d74e
UNK_RET HasAdvisorMessageBeenSeen(char const* a1);
/// 0x0875d7b6
UNK_RET SetAdvisorMessageHasBeenSeen(char const* a1, bool a2);
/// 0x0875d886
UNK_RET ResetAdvisorMessages();
/// 0x0875d8ba
UNK_RET LoadAdvancedSettings(ICvPreGame1* a1);
/// 0x0875e402
UNK_RET SaveAdvancedSettings(ICvPreGame1* a1);
}

namespace cvFileManager {
/// 0x08760584
UNK_RET FindCatalogedFile(char const* a1);
/// 0x087606a2
UNK_RET FindCatalogedFile(wchar_t const* a1);
}
