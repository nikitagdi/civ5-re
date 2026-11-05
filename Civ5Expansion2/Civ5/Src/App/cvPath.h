/// origin guess
#pragma once

class cvPath;

/// size 0x420
class cvPath {
public:
    char unk_0[0x420];  ///< +0x0

    /// 0x086decec
    cvPath();
    /// 0x086ded1e
    cvPath(cvPath const& a1);
    /// 0x086ded9c
    cvPath(wchar_t const* a1);
    /// 0x086deb10
    UNK_RET FromSystemLocale(char const* a1);
    /// 0x086debb2
    UNK_RET OnPathChanged();
    /// 0x086dec1e
    UNK_RET FromUTF8(char const* a1);
    /// 0x086decc0
    UNK_RET FromUTF16(wchar_t const* a1);
    /// 0x086dee38
    UNK_RET operator=(cvPath const& a1);
    /// 0x086deee2
    UNK_RET operator=(wchar_t const* a1);
    /// 0x086def28
    UNK_RET Append(cvPath const& a1);
    /// 0x086def58
    UNK_RET Append(wchar_t const* a1);
    /// 0x086def88
    UNK_RET ToSystemLocale() const;
    /// 0x086df08c
    UNK_RET ToUTF8() const;
    /// 0x086df190
    UNK_RET ToUTF16() const;
    /// 0x086df196
    UNK_RET CleanupPath();
    /// 0x086df206
    UNK_RET Clear();
    /// 0x086df286
    UNK_RET IsEmpty() const;
};

/// 0x086df6d4
UNK_RET CmndArgNum(char const* a1, char const* a2);
/// 0x086df71d
UNK_RET CmndArgString(char const* a1, char const* a2);

namespace Path { namespace UTF16 {
/// 0x086df430
UNK_RET GetDirectoryName(wchar_t const* a1, unsigned int a2, unsigned int& a3);
/// 0x086df4a5
UNK_RET GetFileName(wchar_t const* a1, unsigned int a2, unsigned int& a3);
/// 0x086df4f6
UNK_RET GetVolumeSeparatorChar();
/// 0x086df4fc
UNK_RET GetExtension(wchar_t const* a1, unsigned int a2, unsigned int& a3);
/// 0x086df52a
UNK_RET GetExtensionSeparatorChar();
/// 0x086df530
UNK_RET GetDirectorySeparatorChar();
/// 0x086df536
UNK_RET GetAltDirectorySeparatorChar();
/// 0x086df53c
UNK_RET GetFileNameWithoutExtension(wchar_t const* a1, unsigned int a2, unsigned int& a3);
/// 0x086df596
UNK_RET GetPathSeparatorChar();
/// 0x086df59c
UNK_RET NormalizePath(wchar_t* a1, unsigned int a2, wchar_t a3);
}}

namespace Path { namespace UTF8 {
/// 0x086df294
UNK_RET GetDirectoryName(char const* a1, unsigned int a2, unsigned int& a3);
/// 0x086df309
UNK_RET GetFileName(char const* a1, unsigned int a2, unsigned int& a3);
/// 0x086df35a
UNK_RET GetVolumeSeparatorChar();
/// 0x086df360
UNK_RET GetExtension(char const* a1, unsigned int a2, unsigned int& a3);
/// 0x086df38c
UNK_RET GetExtensionSeparatorChar();
/// 0x086df392
UNK_RET GetDirectorySeparatorChar();
/// 0x086df398
UNK_RET GetAltDirectorySeparatorChar();
/// 0x086df39e
UNK_RET GetFileNameWithoutExtension(char const* a1, unsigned int a2, unsigned int& a3);
/// 0x086df3fa
UNK_RET GetPathSeparatorChar();
/// 0x086df400
UNK_RET NormalizePath(char* a1, unsigned int a2, char a3);
}}
