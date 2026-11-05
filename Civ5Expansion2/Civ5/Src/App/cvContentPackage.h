/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FString.h"
#include <memory>
#include <string>
#include <vector>

class FXmlTraverser;
class cvContentTypeAttributeList;

/// size ?
class cvContentPackage {
public:
    enum CONTENT_TYPES : int;
    /// 0x0873e72c
    cvContentPackage();
    /// 0x0873e852
    cvContentPackage(FStringA const& a1);
    /// 0x0873e99a
    ~cvContentPackage();
    /// 0x0873eaa0
    UNK_RET LoadDefinition(char const* a1);
    /// 0x0873ecb4
    UNK_RET UnloadDefinition();
    /// 0x0873ece4
    UNK_RET IsContentActive(cvContentPackage::CONTENT_TYPES a1) const;
    /// 0x0873ecf8
    UNK_RET SetContentActive(cvContentPackage::CONTENT_TYPES a1, bool a2);
    /// 0x0873edd8
    UNK_RET ActivateFileManager(cvContentPackage::CONTENT_TYPES a1);
    /// 0x0873f91e
    UNK_RET DeactivateFileManager(cvContentPackage::CONTENT_TYPES a1);
    /// 0x0873f9e8
    UNK_RET IsDisabled() const;
    /// 0x0873f9f8
    UNK_RET SetDisabled(bool a1);
    /// 0x0873fa3a
    UNK_RET GetDirectoryName(FStringA* a1) const;
    /// 0x0873fa6c
    UNK_RET MakeSubDirectory(char const* a1, FStringA* a2) const;
    /// 0x0873fc28
    UNK_RET DetermineContent(FFastVector<unsigned char, false, 0u, 0u, BaseVector<unsigned char, false>::FDefaultFastVectorAllocator> const& a1, FFastVector<unsigned char, false, 0u, 0u, BaseVector<unsigned char, false>::FDefaultFastVectorAllocator> const& a2);
    /// 0x0873fe8c
    UNK_RET LoadHeader();
    /// 0x087413f2
    UNK_RET ValidateOwnership();
    /// 0x087414c4
    UNK_RET DeactivateFileManager();
    /// 0x08741500
    UNK_RET ValidateContent();
    /// 0x0874167e
    UNK_RET GetPackageFiles(char const* a1, std::vector<std::string, std::allocator<std::string> >* a2, unsigned int a3, cvContentPackage::CONTENT_TYPES a4, int a5, cvContentTypeAttributeList* a6);
    /// 0x0874209e
    UNK_RET AddPackageFile(char const* a1, std::vector<std::string, std::allocator<std::string> >* a2, unsigned int a3, cvContentPackage::CONTENT_TYPES a4);
    /// 0x08742bd6
    UNK_RET GetPackageFiles(FXmlTraverser a1, char const* a2, std::vector<std::string, std::allocator<std::string> >* a3, unsigned int a4, cvContentPackage::CONTENT_TYPES a5);
    /// 0x08742fc2
    UNK_RET GetPackageContentTypeCount(cvContentPackage::CONTENT_TYPES a1, bool a2) const;
    /// 0x08743162
    UNK_RET GetPackageContentTypeEntryAttributes(cvContentPackage::CONTENT_TYPES a1, int a2, cvContentTypeAttributeList* a3) const;
    /// 0x08743c6c
    UNK_RET ActivateFileManager();
};
