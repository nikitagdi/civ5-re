/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"

class FLeanMap;

/// size ?
class FLeanMap {
public:
    class HeightMap;
    /// 0x08ce6f64
    UNK_RET Create(FLeanMap::HeightMap const& a1, void* a2, FLeanMap& a3);
    /// 0x08ce7b0a
    UNK_RET GetLeanMapSize(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4);
    /// 0x08ce7b5a
    UNK_RET Serialize(FDataStream& a1) const;
    // Checks the "LEAN" magic and points at two raw blocks (header +0x18, +0x1c); no decompression.
    /// 0x08ce7c5c
    static bool DeSerializeFromMemory(void* a1, FLeanMap& a2);
    /// 0x08ce7cc0
    UNK_RET ConvertToDX9();
};
