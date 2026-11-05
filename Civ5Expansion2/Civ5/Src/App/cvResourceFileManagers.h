/// origin guess
#pragma once
#include "Civ5/Src/App/cvContentPackage.h"
#include "FirePlace/FireWorks/FString.h"
#include <Windows.h>
#include <memory>
#include <string>
#include <vector>

class cvContentPackage;
class cvContentPackageIDList;

/// size ?
class cvResourceFileManagers {
public:
    enum MANAGER_TYPE : int;
    /// 0x08743380
    cvResourceFileManagers();
    /// 0x08743392
    ~cvResourceFileManagers();
    /// 0x08742b7c
    UNK_RET IsEnabled();
    /// 0x08742b96
    UNK_RET GetFileManager(cvResourceFileManagers::MANAGER_TYPE a1);
    /// 0x087433bc
    UNK_RET Clear();
    /// 0x08743400
    UNK_RET IsValid() const;
    /// 0x0874340e
    UNK_RET SetEnabled(bool a1);
    /// 0x08743450
    UNK_RET SetEnabled(cvResourceFileManagers::MANAGER_TYPE a1, bool a2);
    /// 0x0874348a
    UNK_RET SetID(char const* a1, unsigned int a2, unsigned int a3);
    /// 0x087434fe
    UNK_RET AddPaks(std::vector<std::string, std::allocator<std::string> > const& a1);
    /// 0x087439e6
    UNK_RET AllowOverridesInPaks(std::vector<std::string, std::allocator<std::string> > const& a1, bool a2);
};

namespace cvContentPackageManager {
/// 0x08743ca8
UNK_RET FindPackages(char const* a1);
/// 0x08743efe
UNK_RET ClearPackages();
/// 0x08743f56
UNK_RET LoadPackageHeader(FStringA const& a1, cvContentPackage* a2);
/// 0x08743f9a
UNK_RET LoadPackageDefinition(FStringA const& a1, cvContentPackage* a2);
/// 0x08743fd0
UNK_RET GetPackageCount();
/// 0x08743fe6
UNK_RET GetPackage(unsigned int a1);
/// 0x0874400a
UNK_RET GetPackageFiles(unsigned int a1, char const* a2, std::vector<std::string, std::allocator<std::string> >* a3, unsigned int a4);
/// 0x08744076
UNK_RET GetAllPackageFiles(char const* a1, std::vector<std::string, std::allocator<std::string> >* a2, unsigned int a3, cvContentPackageIDList const* a4, cvContentPackage::CONTENT_TYPES a5);
/// 0x0874413e
UNK_RET UncachePackages();
/// 0x0874418e
UNK_RET ValidateOwnership();
/// 0x087441ce
UNK_RET ValidateContent();
/// 0x0874420e
UNK_RET GetAllPackageIDs(cvContentPackageIDList& a1, unsigned int a2, cvContentPackage::CONTENT_TYPES a3);
/// 0x087442ca
UNK_RET GetPackageIndex(_GUID const& a1, bool a2);
/// 0x08744356
UNK_RET ArePackagesAvailable(cvContentPackageIDList const& a1, cvContentPackageIDList* a2);
/// 0x087443fa
UNK_RET FilterPackageList(cvContentPackageIDList& a1, unsigned int a2, cvContentPackageIDList* a3);
/// 0x0874448e
UNK_RET ActivateFileSystems();
}
