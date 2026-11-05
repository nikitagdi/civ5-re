/// origin guess
#pragma once
#include "FirePlace/FireWorks/FString.h"

class FTexture;

/// size ?
class FTexturePalette {
public:
    /// 0x08cf04b4
    FTexturePalette();
    /// 0x08cf04da
    UNK_RET SetTexture(char const* a1, FTexture* a2);
    /// 0x08cf07d8
    UNK_RET GetTexture(char const* a1) const;
    /// 0x08cf0946
    UNK_RET RemoveTexture(char const* a1, FTexture* a2);
    /// 0x08cf0a7a
    UNK_RET BiasMips(int a1);
    /// 0x08cf0a7c
    UNK_RET GetStats(bool a1, unsigned int* a2, unsigned int* a3, FStringA* a4);
    /// 0x08cf0a7e
    UNK_RET RemoveAllTextures();
    /// 0x08cf0ac2
    UNK_RET ReloadChangedTextures();
};
