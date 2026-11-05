/// origin guess
#pragma once

namespace FireGrafix {
class FGXModelNode;
class FGXShaderPackageInstanceView;
}
class FGXVector4;

namespace FireGrafix {
/// size 0x50
class FGXModelTemplate {
public:
    unsigned int* m_pSurfaceHandlesBegin;                      ///< +0x0
    unsigned int* m_pSurfaceHandlesEnd;                        ///< +0x4
    unsigned int* m_pSurfaceHandlesCap;                        ///< +0x8
    FireGrafix::FGXShaderPackageInstanceView* m_pShaderViews;  ///< +0xc
    unsigned int m_uShaderViewCount;                           ///< +0x10
    unsigned int m_uShaderViewCapacity;                        ///< +0x14
    FireGrafix::FGXModelNode* m_pNodesBegin;                   ///< +0x18
    FireGrafix::FGXModelNode* m_pNodesEnd;                     ///< +0x1c
    FireGrafix::FGXModelNode* m_pNodesCap;                     ///< +0x20
    unsigned int m_uPoolHead;                                  ///< +0x24
    unsigned int m_uPoolCount;                                 ///< +0x28
    void* m_pPoolEntries;                                      ///< +0x2c
    unsigned int m_uPoolField_30;                              ///< +0x30
    unsigned char m_PoolRest[0x10];                            ///< +0x34
    FGXVector4* m_pVectorsBegin;                               ///< +0x44
    FGXVector4* m_pVectorsEnd;                                 ///< +0x48
    FGXVector4* m_pVectorsCap;                                 ///< +0x4c

    /// 0x08cf4a30
    UNK_RET Reserve(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4);
    /// 0x08cf4a9e
    UNK_RET Release();
};
}

namespace FireGrafix {
/// 0x08cf4bbe
UNK_RET DeActivateModelByNodeShift(FireGrafix::FGXModelNode* a1, unsigned int a2, unsigned int a3);
/// 0x08cf4c00
UNK_RET ActivateModelByNodeShift(FireGrafix::FGXModelNode* a1, unsigned int a2, unsigned int a3);
}
