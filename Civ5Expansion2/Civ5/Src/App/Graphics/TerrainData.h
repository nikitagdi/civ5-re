/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FString.h"
#include "FirePlace/FireGrafix/FGXInt2.h"

class HEXVec;
namespace FireGrafix {
class TextureData;
}

// Version-3 piece files store old hex codes: keys[i] maps through index[i] into values.
extern "C" {
/// 0x09215020
extern unsigned int g_aLegacyHexCodeValues[31];
/// 0x0921509c
extern unsigned int g_aLegacyHexCodeKeys[23];
/// 0x092150f8
extern unsigned int g_aLegacyHexCodeIndex[23];
}

// Same memory layout as FFastVector/BaseVector: data pointer, element count, capacity.
/// size 0xc
template <class T>
struct TerrainDataVec {
    T* m_pData;                ///< +0x0
    unsigned int m_uiSize;     ///< +0x4
    unsigned int m_uiCapacity; ///< +0x8
};

/// size 0x824
class TerrainData {
public:
    /// size 0x100
    struct TextureName {
        char m_szName[256]; ///< +0x0
    };

    /// size 0x1008
    struct TextureImage {
        unsigned int m_aPixels[32][32]; ///< +0x0
        unsigned int m_uiWidth;         ///< +0x1000
        unsigned int m_uiHeight;        ///< +0x1004
    };

    /// 0x0891d098
    UNK_RET Load(bool bKeepImages);
    /// 0x0891d0e2
    void Unload();
    /// 0x0891d24c
    UNK_RET LoadTerrainDefinition(char const* pszFile, bool bLoadTextureC, bool bLoadTextures);
    /// 0x0891dfa2
    static unsigned int GetTerrainHexCodeFromType(unsigned int uiTerrainType, unsigned int uiContinent);

    unsigned int m_uiNumTerrains;                  ///< +0x0
    unsigned int m_aTextureAHandle[64];            ///< +0x4
    unsigned int m_aTextureCHandle[64];            ///< +0x104
    unsigned int m_aTextureBHandle[64];            ///< +0x204
    unsigned char unk_304[4];                      ///< +0x304
    unsigned int field_308[64];                    ///< +0x308
    float m_afFileFloat[64];                       ///< +0x408
    int m_aFileInt[64];                            ///< +0x508
    unsigned int m_aHexCode[64];                   ///< +0x608
    int m_aContinentIndex[64];                     ///< +0x708
    TerrainDataVec<TextureName> m_aTextureNames;   ///< +0x808
    TerrainDataVec<TextureImage> m_aTextureImages; ///< +0x814
    bool m_bKeepImages;                            ///< +0x820
    bool m_bLoaded;                                ///< +0x821
};

// Whole-file read buffer: the file bytes (preceded by their length) are at m_pData, the FFileIO at +0x10014.
/// size 0x1045c
struct FileLoadBuffer {
    unsigned char unk_0[0x10010];  ///< +0x0
    unsigned char* m_pData;        ///< +0x10010
    unsigned char unk_10014[0x448]; ///< +0x10014
};

/// 0x08669c48
void Fun_08669c48(FileLoadBuffer* pBuffer, char const* pszFile);
/// 0x08669d48
unsigned int Fun_08669d48(FileLoadBuffer* pBuffer);
/// 0x08669d90
void Fun_08669d90(FileLoadBuffer* pBuffer);
/// 0x0891dfdc
void Fun_0891dfdc(TerrainDataVec<TerrainData::TextureImage>* pVec, TerrainData::TextureImage const* pImage);
/// 0x0891e03a
void Fun_0891e03a(TerrainDataVec<TerrainData::TextureImage>* pVec, unsigned int uiCount);
/// 0x0891e11e
void Fun_0891e11e(TerrainDataVec<TerrainData::TextureName>* pVec, unsigned int uiCount);

/// 0x08665a68
unsigned char* Fun_08665a68(UNK_PARAMS);
/// 0x0891cdd0
UNK_RET Fun_0891cdd0(UNK_PARAMS);
/// 0x0892442a
UNK_RET Fun_0892442a(UNK_PARAMS);
/// 0x089244ae
UNK_RET Fun_089244ae(UNK_PARAMS);
/// 0x08924d12
UNK_RET Fun_08924d12(UNK_PARAMS);
/// 0x089251cc
UNK_RET Fun_089251cc(UNK_PARAMS);
/// 0x08925284
UNK_RET Fun_08925284(UNK_PARAMS);
/// 0x08925328
UNK_RET Fun_08925328(UNK_PARAMS);
/// 0x089253c4
UNK_RET Fun_089253c4(UNK_PARAMS);
/// 0x0892545c
UNK_RET Fun_0892545c(UNK_PARAMS);

namespace RUN_LENGTH_CODING {
/// 0x089247e2 CompressUShrtDataBlock<FFastVector<unsigned short, true, 43u, 265u, cvCompressedTerrainPiecesLibrary::TerrainPieceTempHeapAlloc> >
template <class T0>
void CompressUShrtDataBlock(unsigned short* a1, unsigned int a2, T0* a3);
/// 0x08924aa0 CompressUCharDataBlock<FFastVector<unsigned char, true, 43u, 265u, cvCompressedTerrainPiecesLibrary::TerrainPieceTempHeapAlloc> >
template <class T0>
void CompressUCharDataBlock(unsigned char* a1, unsigned int a2, T0* a3);
}

// One cell of a piece's hex grid; cells are addressed [column * 10 + row].
/// size 0x8
struct TerrainPieceCell {
    unsigned long m_ulHexCode;   ///< +0x0
    unsigned char m_ucContinent; ///< +0x4
    bool m_bValid;               ///< +0x5
    unsigned char unk_6[2];      ///< +0x6
};

/// size 0x56c
class TerrainPiece {
public:
    unsigned char unk_0[0x94];            ///< +0x0
    int m_iNumRows;                       ///< +0x94
    int m_iNumColumns;                    ///< +0x98
    TerrainPieceCell m_aCells[100];       ///< +0x9c
    int m_iLibraryIndex;                  ///< +0x3bc
    int field_3c0;                        ///< +0x3c0
    int field_3c4;                        ///< +0x3c4
    int field_3c8;                        ///< +0x3c8
    int field_3cc;                        ///< +0x3cc
    unsigned char m_ucFlags;              ///< +0x3d0
    unsigned char unk_3d1[3];             ///< +0x3d1
    int m_iHeightmapIndex;                ///< +0x3d4
    int m_iIDMapIndex;                    ///< +0x3d8
    int m_iBlendmap1Index;                ///< +0x3dc
    int m_iBlendmap2Index;                ///< +0x3e0
    short m_sPatternType;                 ///< +0x3e4
    unsigned short m_usBaseType;          ///< +0x3e6
    unsigned short m_usContinent;         ///< +0x3e8
    char m_szName[64];                    ///< +0x3ea
    char m_szFileName[64];                ///< +0x42a
    char m_szHeightmapFile[64];           ///< +0x46a
    char m_szIDMapFile[64];               ///< +0x4aa
    char m_szBlendmap1File[64];           ///< +0x4ea
    char m_szBlendmap2File[64];           ///< +0x52a
    unsigned char unk_56a[2];             ///< +0x56a
};

/// size 0x160
class cvBaseTerrainPiecesLibrary {
public:
    /// 0x0891e38c
    static int ComputePatternType(TerrainPiece const& a1);
    /// 0x0891e464
    unsigned char ConvertIDToIndex(unsigned long a1);
    /// 0x0891e49e
    unsigned long ConvertIndexToID(unsigned char a1);

    unsigned char* m_pFileBuffer;                  ///< +0x0
    unsigned int m_uiFileSize;                     ///< +0x4
    unsigned int m_uiFileCapacity;                 ///< +0x8
    bool m_bFileLoaded;                            ///< +0xc
    unsigned long m_aPieceIDs[64];                 ///< +0x10
    unsigned int m_uiNumPieceIDs;                  ///< +0x110
    unsigned char unk_114[0x40];                   ///< +0x114
    TerrainDataVec<TerrainPiece> m_aPieces;        ///< +0x154
};

/// size 0x80184
class cvCompressedTerrainPiecesLibrary : public cvBaseTerrainPiecesLibrary {
public:
    /// size ?
    class TerrainPieceTempHeapAlloc {
    public:
        /// 0x0c06096c
        static unsigned int ms_bUseTempHeap;
    };

    /// 0x0891e4ac
    void LoadTerrainPieces(bool a1, char const* a2, char const* a3, unsigned long const* a4, unsigned int a5);
    /// 0x0891e5c0
    void LoadFromFlatFiles(char const* pszDirectory);
    /// 0x0891eb7a
    void LoadFromBinary(FDataStream& a1);
    /// 0x0891ed02
    void UnloadTextures();
    /// 0x0891ed30
    void SaveToBinary(FDataStream& a1);
    /// 0x0891ee4a
    UNK_RET LoadTerrainPiece(FStringA const& rkPath, FStringA const& rkName, unsigned short usBaseType, unsigned char ucContinent, TerrainPiece* pPiece);
    // The loaders return the first row-offset (or -1 on failure) and set *pbSmall to 1 for a 128-wide map.
    /// 0x0891ff36
    int LoadHeightmapFromFile(char const* pszFile, int* pbSmall);
    /// 0x0892026c
    int LoadIDMapFromFile(char const* pszFile, int* pbSmall, FStaticVector<HEXVec, 8u, true, 43u, 0u> const& rkHexes);
    /// 0x089205a2
    int LoadBlendmapFromFile(char const* pszFile, int* pbSmall, FStaticVector<HEXVec, 8u, true, 43u, 0u> const& rkHexes, float fRadius);
    /// 0x08920a04
    void DecompressHeightmapBlock(unsigned int uiFirstRow, FGXInt2 kStart, FGXInt2 kEnd, unsigned short* pOut, bool bSmall);
    /// 0x08920b24
    void DecompressBlendmapBlock(unsigned int uiFirstRow, FGXInt2 kStart, FGXInt2 kEnd, unsigned char* pOut, bool bSmall);
    /// 0x08920c3e
    void DecompressIDMapBlock(unsigned int uiFirstRow, FGXInt2 kStart, FGXInt2 kEnd, unsigned char* pOut, int bSmall);

    unsigned char m_aScratch[0x80000];               ///< +0x160
    TerrainDataVec<unsigned char> m_aByteRuns;       ///< +0x80160
    TerrainDataVec<unsigned short> m_aHeightRuns;    ///< +0x8016c
    TerrainDataVec<unsigned int> m_aRowOffsets;      ///< +0x80178
};

/// size ?
class cvTerrainPiecesLibrary : public cvBaseTerrainPiecesLibrary {
public:
    /// size 0x8008
    struct Heightmap128 {
        unsigned short m_aHeights[0x4000]; ///< +0x0
        unsigned int m_uiWidth;            ///< +0x8000
        unsigned int m_uiHeight;           ///< +0x8004
    };

    /// size 0x80008
    struct Heightmap512 {
        unsigned short m_aHeights[0x40000]; ///< +0x0
        unsigned int m_uiWidth;             ///< +0x80000
        unsigned int m_uiHeight;            ///< +0x80004
    };

    /// size 0x4008
    struct Bytemap128 {
        unsigned char m_aData[0x4000]; ///< +0x0
        unsigned int m_uiWidth;        ///< +0x4000
        unsigned int m_uiHeight;       ///< +0x4004
    };

    /// size 0x40008
    struct Bytemap512 {
        unsigned char m_aData[0x40000]; ///< +0x0
        unsigned int m_uiWidth;         ///< +0x40000
        unsigned int m_uiHeight;        ///< +0x40004
    };

    /// 0x08920d58
    void UnloadTextures();
    /// 0x08920da4
    void LoadTerrainPieces(char const* a1, unsigned long const* a2, unsigned int a3);
    /// 0x08920e08
    void LoadFromFlatFiles(char const* pszDirectory);
    /// 0x089216f6
    UNK_RET LoadTerrainPiece(FStringA const& rkPath, FStringA const& rkName, unsigned short usBaseType, unsigned char ucContinent, TerrainPiece* pPiece);
    /// 0x0892296a
    int SaveTerrainPiece(unsigned int uiPieceIndex);
    /// 0x0892336a
    void SaveHeightmapToFile(char const* pszFile, TerrainPiece const& rkPiece);
    /// 0x089234ae
    void SaveIDMapToFile(char const* pszFile, TerrainPiece const& rkPiece);
    /// 0x089235d4
    void SaveBlendmapToFile(char const* pszFile, bool bSmall, unsigned int uiIndex);
    /// 0x08923750
    int LoadHeightmapFromFile(char const* pszFile, int* pbSmall);
    /// 0x08923ada
    int LoadIDMapFromFile(char const* pszFile, int* pbSmall);
    /// 0x08923e20
    int LoadBlendmapFromFile(char const* pszFile, int* pbSmall, FStaticVector<HEXVec, 8u, true, 43u, 0u> const& rkHexes, float fRadius);
    /// 0x08924152
    void AddTerrainPiece_Small(TerrainPiece const& a1);
    /// 0x089242be
    void AddTerrainPiece_Large(TerrainPiece const& a1);

    TerrainDataVec<Heightmap128> m_aSmallHeightmaps; ///< +0x160
    TerrainDataVec<Heightmap512> m_aLargeHeightmaps; ///< +0x16c
    TerrainDataVec<Bytemap128> m_aSmallBlendmaps;    ///< +0x178
    TerrainDataVec<Bytemap512> m_aLargeBlendmaps;    ///< +0x184
    TerrainDataVec<Bytemap128> m_aSmallIDMaps;       ///< +0x190
    TerrainDataVec<Bytemap512> m_aLargeIDMaps;       ///< +0x19c
    // The path buffer's capacity is not known.
    char m_szPath[1];                                ///< +0x1a8
};

// Reads the whole file into pLibrary's file buffer (once: m_bFileLoaded) and sets m_uiFileSize.
/// 0x0892460c
bool Fun_0892460c(cvBaseTerrainPiecesLibrary* pLibrary, char const* pszFile);
// Appends *puValue to the vector.
/// 0x08924794
void Fun_08924794(TerrainDataVec<unsigned int>* pVec, unsigned int const* puValue);
// Reports a missing mip chain or a format other than 0x15 in a message box.
/// 0x0892089b fastcc
bool Fun_0892089b(FireGrafix::TextureData* pTexture, char const* pszFile);
/// 0x08924890
unsigned int Fun_08924890(cvBaseTerrainPiecesLibrary* pLibrary, unsigned char* pMap, int iWidth, int iHeight, FStaticVector<HEXVec, 8u, true, 43u, 0u> const* pHexes);
/// 0x08924b3e
void Fun_08924b3e(cvBaseTerrainPiecesLibrary* pLibrary, unsigned char* pMap, int iWidth, int iHeight, FStaticVector<HEXVec, 8u, true, 43u, 0u> const* pHexes, float fRadius);
/// 0x08925726
void Fun_08925726(TerrainDataVec<cvTerrainPiecesLibrary::Heightmap128>* pVec, unsigned int uiCount);
/// 0x0892566e
void Fun_0892566e(TerrainDataVec<cvTerrainPiecesLibrary::Heightmap512>* pVec, unsigned int uiCount);
/// 0x089255b6
void Fun_089255b6(TerrainDataVec<cvTerrainPiecesLibrary::Bytemap128>* pVec, unsigned int uiCount);
/// 0x089254fe
void Fun_089254fe(TerrainDataVec<cvTerrainPiecesLibrary::Bytemap512>* pVec, unsigned int uiCount);
