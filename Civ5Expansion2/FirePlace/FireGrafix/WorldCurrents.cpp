/// origin guess
#include "FirePlace/FireGrafix/WorldCurrents.h"
#include <Windows.h>

namespace FireGrafix {
/// 0x0878cf2d
int dispatchShaderPackageGetID_ShaderDefDX9(char const* a1) {
    if (a1 == nullptr) {
        return -1;
    }
    for (int i = 0x5e - 1; i >= 0; --i) {
        if (_stricmp(g_ShaderPackageNameTable[i].pszName, a1) == 0) {
            return g_ShaderPackageNameTable[i].nPackageID;
        }
    }
    return -1;
}
}
