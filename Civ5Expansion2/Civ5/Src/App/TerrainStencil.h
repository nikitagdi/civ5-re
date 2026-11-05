/// origin guess
#pragma once

class FGXVector2;

/// size 0x8
class TerrainStencilVertex {
public:
    float x;  ///< +0x0
    float y;  ///< +0x4
};

/// size 0x10
class TerrainStencilSurface {
public:
    unsigned int m_uiVertexCount;       ///< +0x0
    int field_4;                        ///< +0x4
    TerrainStencilVertex* m_pVertices;  ///< +0x8
    int field_c;                        ///< +0xc
};

// Payload of the local-machine event with id 32 (slot 0xc80 of the event system); filled by AddStencil.
/// size ?
class TerrainStencilUpdatedData {
public:
    float m_fMinX;                ///< +0x0
    float m_fMinY;                ///< +0x4
    float m_fMaxX;                ///< +0x8
    float m_fMaxY;                ///< +0xc
    unsigned int m_uiStencilBits; ///< +0x10
};

/// 0x087fd914
UNK_RET Fun_087fd914(UNK_PARAMS);

/// 0x087fd9f8
UNK_RET Fun_087fd9f8(UNK_PARAMS);

/// 0x087fda4a
UNK_RET Fun_087fda4a(UNK_PARAMS);

/// 0x087fdaa4
UNK_RET Fun_087fdaa4(UNK_PARAMS);

/// 0x087fdb22
UNK_RET Fun_087fdb22(UNK_PARAMS);

/// 0x087fe06c
UNK_RET Fun_087fe06c(UNK_PARAMS);

/// 0x087fe098
UNK_RET Fun_087fe098(UNK_PARAMS);

/// 0x087fe5d2
unsigned char Fun_087fe5d2(UNK_PARAMS);

/// 0x087fe8d6
TerrainStencilUpdatedData* Fun_087fe8d6(UNK_PARAMS);

/// 0x087fea4a
UNK_RET Fun_087fea4a(UNK_PARAMS);

/// size 0x30
class TerrainStencil {
public:
    /// 0x087fd1e0
    TerrainStencil();
    /// 0x087fd22e
    UNK_RET StartupGame();
    /// 0x087fd2e2
    UNK_RET ClearStencil();
    /// 0x087fd39c
    UNK_RET ShutdownGame();
    /// 0x087fd3cc
    UNK_RET AddStencil(TerrainStencilSurface surface, unsigned char ucStencilBits, bool a3);
    /// 0x087fd588
    UNK_RET AddStencil(TerrainStencilVertex const* pVertices, unsigned int uiVertexCount, unsigned char ucStencilBits, bool a4);
    /// 0x087fd746
    unsigned char SampleWorldPosition(FGXVector2 const& vPos, unsigned char ucMask) const;
    /// 0x087fd7d6
    void SaveToFile(char const* szPath) const;

private:
    bool m_bStarted;   ///< +0x0
    char unk_1[3];     ///< +0x1
    char field_4;      ///< +0x4
    char field_5;      ///< +0x5
    char unk_6[2];     ///< +0x6
    int m_iWidth;      ///< +0x8
    int m_iHeight;     ///< +0xc
    int field_10;      ///< +0x10
    int field_14;      ///< +0x14
    int field_18;      ///< +0x18
    int field_1c;      ///< +0x1c
    int field_20;      ///< +0x20
    int field_24;      ///< +0x24
    int field_28;      ///< +0x28
    int field_2c;      ///< +0x2c
};
