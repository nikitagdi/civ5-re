/// origin guess
#include <stdio.h>
#include <string.h>
#include "Civ5/Src/App/Graphics/TerrainData.h"
#include "Civ5/Src/App/Graphics/CellBlitJob.h"
#include "Civ5/Src/App/Graphics/Terrain.h"
#include "FirePlace/FireEngine2/FTextureResource.h"
#include "FirePlace/FireGrafix/CRenderer9.h"
#include "FirePlace/FireGrafix/TextureData.h"
#include "FirePlace/FireWorks/FFileStream.h"
#include "FirePlace/FireWorks/FCSVFile.h"
#include "FirePlace/FireWorks/FTextFile.h"
#include "Civ5/Src/App/HEXTransform_i.h"
#include "Civ5/Src/App/HEXVec.h"
#include "Civ5/Src/App/Graphics/cvTerrainBlend.h"

extern "C" int _access(char const* pszPath, int iMode);

extern "C" {
/// 0x09215020
unsigned int g_aLegacyHexCodeValues[31] = {1, 2, 4, 8, 16, 32, 64, 128, 0x00000100, 0x00000200, 0x00000400, 0x00000800, 0x00001000, 0x00002000, 0x00004000, 0x00008000, 0x00010000, 0x00020000, 0x00040000, 0x00080000, 0x00100000, 0x00200000, 0x00400000, 0x00800000, 0x01000000, 0x02000000, 0x04000000, 0x08000000, 0xffffffff, 0xf3ef9fff, 0x00106000};

/// 0x0921509c
unsigned int g_aLegacyHexCodeKeys[23] = {1, 2, 4, 8, 16, 32, 64, 128, 0x00000100, 0x00000200, 0x00000400, 0x00000800, 0x00001000, 0x00002000, 0x00004000, 0x00008000, 0x00010000, 0x00020000, 0x00040000, 0x00080000, 0x000fffff, 0x000fe7ff, 0x000c1800};

/// 0x092150f8
unsigned int g_aLegacyHexCodeIndex[23] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 13, 14, 15, 16, 17, 18, 19, 20, 21, 28, 29, 30};
}


// Row-run-length data is stored as (run length, value) pairs.
static const unsigned int kPieceBinaryVersion = 0x14;
static const unsigned int kPieceStride = 0x56c;

/// 0x0891d098
UNK_RET TerrainData::Load(bool bKeepImages) {
    m_bKeepImages = bKeepImages;
    memset(this, 0, 0x304);
    m_bLoaded = true;
}

/// 0x0891d0e2
void TerrainData::Unload() {
    if (!m_bLoaded) {
        return;
    }
    std::set<unsigned int, std::less<unsigned int const>, FSTL_Tagged_Allocator<unsigned int, 4u, 9u, 4u> > kHandles;
    for (unsigned int uiTerrain = 0; uiTerrain < m_uiNumTerrains; ++uiTerrain) {
        if (m_aTextureAHandle[uiTerrain] != 0) {
            kHandles.insert(m_aTextureAHandle[uiTerrain]);
            Fun_08672408(FireGrafix::CRenderer9::m_TextureList, m_aTextureAHandle[uiTerrain]);
        }
        if (m_aTextureBHandle[uiTerrain] != 0) {
            kHandles.insert(m_aTextureBHandle[uiTerrain]);
            Fun_08672408(FireGrafix::CRenderer9::m_TextureList, m_aTextureBHandle[uiTerrain]);
        }
        if (m_aTextureCHandle[uiTerrain] != 0) {
            kHandles.insert(m_aTextureCHandle[uiTerrain]);
            Fun_08672408(FireGrafix::CRenderer9::m_TextureList, m_aTextureCHandle[uiTerrain]);
        }
    }
    FTextureResourceMgr::ReleaseTextureHandles(kHandles);
    m_bLoaded = false;
}

/// 0x0891d24c
UNK_RET TerrainData::LoadTerrainDefinition(char const* pszFile, bool bLoadTextureC, bool bLoadTextures) {
    FStringA kLine;
    FStringA::FStringAFixedBuffer<256> kLineBuffer(kLine);
    FStringA kPrefix;
    FStringA::FStringAFixedBuffer<256> kPrefixBuffer(kPrefix);
    FStringA kPathA;
    FStringA::FStringAFixedBuffer<256> kPathABuffer(kPathA);
    FStringA kPathB;
    FStringA::FStringAFixedBuffer<256> kPathBBuffer(kPathB);
    FStringA kPathC;
    FStringA::FStringAFixedBuffer<256> kPathCBuffer(kPathC);
    FStringA kPathChecker;
    FStringA::FStringAFixedBuffer<256> kPathCheckerBuffer(kPathChecker);

    FTextFile kFile(pszFile, FIFile::modeRead, 0, 0);
    // The first line of the definition file is a header.
    kFile.ReadString(kLine);
    m_uiNumTerrains = 0;
    kPrefix.Copy(0x1f, "assets\\terrain\\TerrainTextures\\");

    while (kFile.ReadString(kLine)) {
        kLine.Replace(',', ' ');
        int iContinent;
        char szTextureA[512];
        char szTextureB[512];
        char szTextureC[512];
        float fSkipped;
        float fFileFloat;
        int iFileInt;
        unsigned int uiHexCode;
        sscanf(kLine, "%i %s %s %s %f %f %i %x", &iContinent, szTextureA, szTextureB, szTextureC, &fSkipped, &fFileFloat, &iFileInt, &uiHexCode);
        kPathA.Format("%s%s", static_cast<char const*>(kPrefix), szTextureA);
        kPathB.Format("%s%s", static_cast<char const*>(kPrefix), szTextureB);
        kPathC.Format("%s%s", static_cast<char const*>(kPrefix), szTextureC);

        unsigned int uiName = m_aTextureNames.m_uiSize;
        if (uiName == m_aTextureNames.m_uiCapacity) {
            Fun_0891e11e(&m_aTextureNames, uiName + 1);
        }
        m_aTextureNames.m_uiSize = uiName + 1;
        TextureName* pName = &m_aTextureNames.m_pData[uiName];
        memset(pName, 0, 0x100);
        strcpy(pName->m_szName, szTextureA);

        if (!bLoadTextures) {
            m_aTextureAHandle[m_uiNumTerrains] = 0;
            m_aTextureBHandle[m_uiNumTerrains] = 0;
            m_aTextureCHandle[m_uiNumTerrains] = 0;
        } else {
            TextureImage* pImage = 0;
            if (m_bKeepImages) {
                unsigned int uiImage = m_aTextureImages.m_uiSize;
                if (uiImage == m_aTextureImages.m_uiCapacity) {
                    Fun_0891e03a(&m_aTextureImages, uiImage + 1);
                }
                m_aTextureImages.m_uiSize = uiImage + 1;
                pImage = &m_aTextureImages.m_pData[uiImage];
                pImage->m_uiWidth = 0x20;
                pImage->m_uiHeight = 0x20;
                memset(pImage->m_aPixels, 0, sizeof(pImage->m_aPixels));
            }

            if (_stricmp(szTextureA, "NULL") == 0) {
                m_aTextureAHandle[m_uiNumTerrains] = 0;
            } else if (!m_bKeepImages) {
                unsigned int hTexture = FTextureResourceMgr::LoadTextureHandle(kPathA, 0x20000, 0);
                Fun_0868a21a(FireGrafix::CRenderer9::m_TextureList, hTexture);
                m_aTextureAHandle[m_uiNumTerrains] = hTexture;
            } else {
                FileLoadBuffer kBuffer;
                Fun_08669c48(&kBuffer, kPathA);
                FireGrafix::TextureData kTexture(kBuffer.m_pData, Fun_08669d48(&kBuffer), 0x11);
                m_aTextureAHandle[m_uiNumTerrains] = FireGrafix::g_pRenderer->CreateTexture(&kTexture);
                // The image keeps the top mip level, transposed, with opaque alpha.
                FireGrafix::TextureData::MipLevel const& kMip = kTexture.m_aMips[0];
                for (unsigned int uiX = 0; uiX < (pImage->m_uiWidth < kMip.m_uWidth ? pImage->m_uiWidth : kMip.m_uWidth); ++uiX) {
                    for (unsigned int uiY = 0; uiY < (pImage->m_uiHeight < kMip.m_uHeight ? pImage->m_uiHeight : kMip.m_uHeight); ++uiY) {
                        unsigned int const* pRow = reinterpret_cast<unsigned int const*>(kMip.m_apFaces[0] + uiY * kMip.m_uPitch);
                        pImage->m_aPixels[uiX][uiY] = pRow[uiX] | 0xff000000;
                    }
                }
            }

            if (_stricmp(szTextureB, "NULL") == 0) {
                m_aTextureBHandle[m_uiNumTerrains] = 0;
            } else {
                unsigned int hTexture = FTextureResourceMgr::LoadTextureHandle(kPathB, 0x20000, 0);
                Fun_0868a21a(FireGrafix::CRenderer9::m_TextureList, hTexture);
                m_aTextureBHandle[m_uiNumTerrains] = hTexture;
            }

            if (_stricmp(szTextureC, "NULL") == 0 || !bLoadTextureC) {
                m_aTextureCHandle[m_uiNumTerrains] = 0;
            } else {
                unsigned int hTexture = FTextureResourceMgr::LoadTextureHandle(kPathC, 0x20000, 0);
                Fun_0868a21a(FireGrafix::CRenderer9::m_TextureList, hTexture);
                m_aTextureCHandle[m_uiNumTerrains] = hTexture;
            }
        }

        m_aFileInt[m_uiNumTerrains] = iFileInt;
        field_308[m_uiNumTerrains] = 0;
        m_afFileFloat[m_uiNumTerrains] = fFileFloat;
        m_aHexCode[m_uiNumTerrains] = uiHexCode | g_aContinentPrefix[iContinent];
        m_aContinentIndex[m_uiNumTerrains] = iContinent;
        ++m_uiNumTerrains;
    }

    // The last entry is the fallback terrain.
    char szChecker[17] = "Checkerboard.dds";
    kPathChecker.Format("%s%s", static_cast<char const*>(kPrefix), szChecker);
    unsigned int uiName = m_aTextureNames.m_uiSize;
    if (uiName == m_aTextureNames.m_uiCapacity) {
        Fun_0891e11e(&m_aTextureNames, uiName + 1);
    }
    m_aTextureNames.m_uiSize = uiName + 1;
    TextureName* pName = &m_aTextureNames.m_pData[uiName];
    memset(pName, 0, 0x100);
    strcpy(pName->m_szName, szChecker);
    m_aTextureBHandle[m_uiNumTerrains] = 0;
    m_aTextureCHandle[m_uiNumTerrains] = 0;
    if (bLoadTextures) {
        unsigned int hTexture = FireGrafix::g_pRenderer->LoadTexture(kPathChecker);
        m_aTextureAHandle[m_uiNumTerrains] = hTexture;
        m_aTextureBHandle[m_uiNumTerrains] = hTexture;
        Fun_0868a21a(FireGrafix::CRenderer9::m_TextureList, m_aTextureBHandle[m_uiNumTerrains]);
    }
    m_aFileInt[m_uiNumTerrains] = 0;
    field_308[m_uiNumTerrains] = 0;
    m_afFileFloat[m_uiNumTerrains] = 0.01f;
    m_aHexCode[m_uiNumTerrains] = 0xffffffff;
    m_aContinentIndex[m_uiNumTerrains] = -1;
    TextureImage kBlankImage;
    kBlankImage.m_uiWidth = 0x20;
    kBlankImage.m_uiHeight = 0x20;
    memset(kBlankImage.m_aPixels, 0, sizeof(kBlankImage.m_aPixels));
    Fun_0891dfdc(&m_aTextureImages, &kBlankImage);
    ++m_uiNumTerrains;
}

/// 0x0891dfa2
unsigned int TerrainData::GetTerrainHexCodeFromType(unsigned int uiTerrainType, unsigned int uiContinent) {
    return (g_aTTPostfix[uiTerrainType] & ~g_mTextureMask) | (g_aContinentPrefix[uiContinent] & g_mTextureMask);
}

/// 0x0891e38c
int cvBaseTerrainPiecesLibrary::ComputePatternType(TerrainPiece const& a1) {
    int iOtherCells = 0;
    int iMaskACells = 0;
    int iBit14Cells = 0;
    for (int iRow = 0; iRow < a1.m_iNumRows; ++iRow) {
        for (int iColumn = 0; iColumn < a1.m_iNumColumns; ++iColumn) {
            TerrainPieceCell const& kCell = a1.m_aCells[iColumn * 10 + iRow];
            if (!kCell.m_bValid) {
                continue;
            }
            unsigned long ulCode = kCell.m_ulHexCode;
            if ((ulCode & 0xf3ef9fff) != 0) {
                ++iOtherCells;
            } else if ((ulCode & 0x0c102000) != 0) {
                ++iMaskACells;
            } else {
                iBit14Cells += (ulCode >> 14) & 1;
            }
        }
    }
    if (iMaskACells != 0 && iOtherCells == 0 && iBit14Cells == 0) {
        return 1;
    }
    if (iBit14Cells != 0 && iOtherCells == 0 && iMaskACells == 0) {
        return 2;
    }
    return (iOtherCells == 0 || iBit14Cells != 0 || iMaskACells != 0) ? 4 : 0;
}

/// 0x0891e464
unsigned char cvBaseTerrainPiecesLibrary::ConvertIDToIndex(unsigned long a1) {
    for (unsigned int uiIndex = 0; uiIndex < m_uiNumPieceIDs; ++uiIndex) {
        if (m_aPieceIDs[uiIndex] == a1) {
            return static_cast<unsigned char>(uiIndex);
        }
    }
    return 0;
}

/// 0x0891e49e
unsigned long cvBaseTerrainPiecesLibrary::ConvertIndexToID(unsigned char a1) {
    return m_aPieceIDs[a1];
}

/// 0x0891e4ac
void cvCompressedTerrainPiecesLibrary::LoadTerrainPieces(bool a1, char const* a2, char const* a3, unsigned long const* a4, unsigned int a5) {
    memcpy(m_aPieceIDs, a4, a5 * 4);
    m_uiNumPieceIDs = a5;
    if (!a1) {
        LoadFromFlatFiles(a2);
        Fun_0892442a(this, a3);
        return;
    }
    FFileStream kStream;
    if (kStream.Open("Resource\\Common\\TerrainPiecesLibrary.fxsb", FIFile::modeRead, 0, 0)) {
        unsigned int uiVersion;
        kStream >> uiVersion;
        if (uiVersion == kPieceBinaryVersion) {
            LoadFromBinary(kStream);
            kStream.Close();
        } else {
            kStream.Close();
            LoadFromFlatFiles(a2);
        }
    } else {
        LoadFromFlatFiles(a2);
    }
}

/// 0x0891e5c0
void cvCompressedTerrainPiecesLibrary::LoadFromFlatFiles(char const* pszDirectory) {
    Fun_08665a1a(m_pFileBuffer);
    m_uiFileCapacity = 0x400000;
    m_pFileBuffer = Fun_08665a68(0x400000, 4, 0x2b, 0x109);

    FStringA kDirectory;
    FStringA kPattern;
    for (unsigned int uiBaseType = 0; uiBaseType < 0x1c; ++uiBaseType) {
        for (unsigned int uiContinent = 0; uiContinent < 5; ++uiContinent) {
            kDirectory.Format("%s\\%s\\%s", pszDirectory, g_pBaseTypeNames[uiBaseType], g_pContinentNames[uiContinent]);
            kPattern.Format("%s\\*.csv", static_cast<char const*>(kDirectory));
            WIN32_FIND_DATAA kFind;
            HANDLE hFind = FindFirstFileA(kPattern, &kFind);
            if (hFind == INVALID_HANDLE_VALUE) {
                continue;
            }
            do {
                FStringA kName;
                kName.Copy(strlen(kFind.cFileName), kFind.cFileName, 0);
                FStringA kPath;
                kPath.Format("%s\\%s", static_cast<char const*>(kDirectory), static_cast<char const*>(kName));

                unsigned int uiIndex = m_aPieces.m_uiSize;
                if (uiIndex == m_aPieces.m_uiCapacity) {
                    Fun_089251cc(&m_aPieces, uiIndex + 1);
                }
                m_aPieces.m_uiSize = uiIndex + 1;
                TerrainPiece* pPiece = &m_aPieces.m_pData[uiIndex];
                HEXTransform_i kTransform = HEXTransform_i::IDENTITY_TRANSFORM;
                Fun_0891cdd0(pPiece, &kTransform);
                pPiece->field_3c0 = pPiece->m_iNumRows;
                pPiece->field_3c4 = pPiece->m_iNumColumns;
                pPiece->m_iHeightmapIndex = 0xffff;
                pPiece->m_iIDMapIndex = 0xffff;
                pPiece->m_iBlendmap1Index = 0xffff;
                pPiece->m_iBlendmap2Index = 0xffff;
                pPiece->m_iLibraryIndex = m_aPieces.m_uiSize - 1;
                LoadTerrainPiece(kPath, kName, static_cast<unsigned short>(uiBaseType), static_cast<unsigned char>(uiContinent), pPiece);
            } while (FindNextFileA(hFind, &kFind));
            FindClose(hFind);
        }
    }

    Fun_08665a1a(m_pFileBuffer);
    m_uiFileSize = 0;
    m_pFileBuffer = 0;
    m_bFileLoaded = false;
    m_uiFileCapacity = 0;
}

/// 0x0891eb7a
void cvCompressedTerrainPiecesLibrary::LoadFromBinary(FDataStream& a1) {
    unsigned int uiSavedUseTempHeap = TerrainPieceTempHeapAlloc::ms_bUseTempHeap;
    TerrainPieceTempHeapAlloc::ms_bUseTempHeap = 1;

    unsigned int uiCount;
    a1 >> uiCount;
    Fun_0892545c(&m_aRowOffsets, uiCount);
    m_aRowOffsets.m_uiSize = (m_aRowOffsets.m_uiCapacity > uiCount) ? uiCount : m_aRowOffsets.m_uiCapacity;
    a1.ReadIt(m_aRowOffsets.m_uiSize * 4, m_aRowOffsets.m_pData);

    a1 >> uiCount;
    Fun_089253c4(&m_aByteRuns, uiCount);
    m_aByteRuns.m_uiSize = (m_aByteRuns.m_uiCapacity > uiCount) ? uiCount : m_aByteRuns.m_uiCapacity;
    a1.ReadIt(m_aByteRuns.m_uiSize, m_aByteRuns.m_pData);

    a1 >> uiCount;
    Fun_08925328(&m_aHeightRuns, uiCount);
    m_aHeightRuns.m_uiSize = (m_aHeightRuns.m_uiCapacity > uiCount) ? uiCount : m_aHeightRuns.m_uiCapacity;
    a1.ReadIt(m_aHeightRuns.m_uiSize * 2, m_aHeightRuns.m_pData);

    a1 >> uiCount;
    Fun_08925284(&m_aPieces, uiCount);
    m_aPieces.m_uiSize = (m_aPieces.m_uiCapacity > uiCount) ? uiCount : m_aPieces.m_uiCapacity;
    a1.ReadIt(m_aPieces.m_uiSize * kPieceStride, m_aPieces.m_pData);

    TerrainPieceTempHeapAlloc::ms_bUseTempHeap = uiSavedUseTempHeap;
}

/// 0x0891ed02
void cvCompressedTerrainPiecesLibrary::UnloadTextures() {
    m_aPieces.m_uiSize = 0;
    m_aRowOffsets.m_uiSize = 0;
    m_aHeightRuns.m_uiSize = 0;
    m_aByteRuns.m_uiSize = 0;
}

/// 0x0891ed30
void cvCompressedTerrainPiecesLibrary::SaveToBinary(FDataStream& a1) {
    INT32 iVersion = kPieceBinaryVersion;
    a1 << iVersion;

    UINT32 uiCount = m_aRowOffsets.m_uiSize;
    a1 << uiCount;
    a1.WriteIt(m_aRowOffsets.m_uiSize * 4, m_aRowOffsets.m_pData);

    uiCount = m_aByteRuns.m_uiSize;
    a1 << uiCount;
    a1.WriteIt(m_aByteRuns.m_uiSize, m_aByteRuns.m_pData);

    uiCount = m_aHeightRuns.m_uiSize;
    a1 << uiCount;
    a1.WriteIt(m_aHeightRuns.m_uiSize * 2, m_aHeightRuns.m_pData);

    uiCount = m_aPieces.m_uiSize;
    a1 << uiCount;
    a1.WriteIt(m_aPieces.m_uiSize * kPieceStride, m_aPieces.m_pData);
}

/// 0x0891ee4a
UNK_RET cvCompressedTerrainPiecesLibrary::LoadTerrainPiece(FStringA const& rkPath, FStringA const& rkName, unsigned short usBaseType, unsigned char ucContinent, TerrainPiece* pPiece) {
    FStringA kMessage;
    FStringA kDirectory;
    FStringA kFileName;
    rkPath.ExtractFileDir(kDirectory, '\\');
    rkPath.ExtractFileName(kFileName, '\\');

    pPiece->m_usContinent = ucContinent;
    pPiece->m_usBaseType = usBaseType;
    strcpy(pPiece->m_szFileName, kFileName);
    strcpy(pPiece->m_szName, rkName);
    pPiece->m_ucFlags &= 0xfd;
    pPiece->field_3c8 = 2;

    for (int iRow = 0; iRow < pPiece->m_iNumRows; ++iRow) {
        for (int iColumn = 0; iColumn < pPiece->m_iNumColumns; ++iColumn) {
            TerrainPieceCell& kCell = pPiece->m_aCells[iColumn * 10 + iRow];
            kCell.m_ulHexCode = 0xffffffff;
            kCell.m_ucContinent = 0;
            kCell.m_bValid = false;
        }
    }

    FCSVFile kCsv(true, true, false);
    kCsv.Load(rkPath);
    int iVersion;
    bool bHasVersion = kCsv.GetInt(0, 0, iVersion);
    if (iVersion < 3 || !bHasVersion) {
        kMessage.Format("File %s is old and cannot be loaded!", static_cast<char const*>(kFileName));
        MessageBoxA(0, kMessage, "Old piece .csv file", MB_ICONEXCLAMATION);
        return;
    }

    int iNumColumns;
    int iNumRows;
    kCsv.GetInt(5, 0, iNumColumns);
    kCsv.GetInt(5, 1, iNumRows);
    for (int iRow = 0; iRow < iNumRows; ++iRow) {
        for (int iColumn = 0; iColumn < iNumColumns; ++iColumn) {
            FStringA kCellText;
            char const* pszCell = kCsv.Get(iRow + 6, iColumn);
            kCellText.Copy(pszCell ? strlen(pszCell) : 0, pszCell, 0);
            int iValue = kCellText.ConvertToInt(10, true);
            unsigned char ucCellContinent = (iValue < 0) ? 0 : ucContinent;
            unsigned int uiCode = static_cast<unsigned int>(iValue) & 0x7fffffff;
            if (uiCode == 0) {
                uiCode = 0xffffffff;
            }
            unsigned int uiHexCode = uiCode;
            if (iVersion == 3) {
                uiHexCode = 0xffffffff;
                if (uiCode != 0xffffff) {
                    if (uiCode == 0x1800) {
                        uiHexCode = 0x106000;
                    } else {
                        uiHexCode = uiCode;
                        for (unsigned int uiEntry = 0; uiEntry < 0x17; ++uiEntry) {
                            if (g_aLegacyHexCodeKeys[uiEntry] == uiCode) {
                                uiHexCode = g_aLegacyHexCodeValues[g_aLegacyHexCodeIndex[uiEntry]];
                                break;
                            }
                        }
                    }
                }
            }
            TerrainPieceCell& kCell = pPiece->m_aCells[iColumn * 10 + iRow];
            kCell.m_ulHexCode = uiHexCode;
            kCell.m_ucContinent = ucCellContinent;
            kCell.m_bValid = (iValue >= 0);
        }
    }

    pPiece->m_sPatternType = ComputePatternType(*pPiece);
    if (pPiece->m_sPatternType == 4) {
        kMessage.Format("Piece '%s' has invalid hex markup.", pPiece->m_szName);
        MessageBoxA(0, kMessage, "Terrain Piece Error", MB_ICONEXCLAMATION);
    }

    FStaticVector<HEXVec, 8u, true, 43u, 0u> kHexes;
    Fun_089244ae(this, &kHexes, pPiece);

    FStringA kHeightmapPath;
    FStringA kIDMapPath;
    FStringA kBlendmap1Path;
    FStringA kBlendmap2Path;
    char const* pszFile = kCsv.Get(1, 0);
    strcpy(pPiece->m_szHeightmapFile, pszFile);
    kHeightmapPath.Format("%s\\%s", static_cast<char const*>(kDirectory), pszFile);
    pszFile = kCsv.Get(2, 0);
    strcpy(pPiece->m_szIDMapFile, pszFile);
    kIDMapPath.Format("%s\\%s", static_cast<char const*>(kDirectory), pszFile);
    pszFile = kCsv.Get(3, 0);
    strcpy(pPiece->m_szBlendmap1File, pszFile);
    kBlendmap1Path.Format("%s\\%s", static_cast<char const*>(kDirectory), pszFile);
    pszFile = kCsv.Get(4, 0);
    strcpy(pPiece->m_szBlendmap2File, pszFile);
    kBlendmap2Path.Format("%s\\%s", static_cast<char const*>(kDirectory), pszFile);

    int iSize;
    pPiece->m_iHeightmapIndex = LoadHeightmapFromFile(kHeightmapPath, &iSize);
    pPiece->m_ucFlags = (pPiece->m_ucFlags & 0xfe) | (iSize & 1);
    pPiece->m_iIDMapIndex = LoadIDMapFromFile(kIDMapPath, &iSize, kHexes);
    if (iSize != (pPiece->m_ucFlags & 1)) {
        kMessage.Format("File %s is wrong size!", static_cast<char const*>(kIDMapPath));
        MessageBoxA(0, kMessage, "Terrain Texture Error", MB_ICONEXCLAMATION);
        return;
    }
    pPiece->m_iBlendmap1Index = LoadBlendmapFromFile(kBlendmap1Path, &iSize, kHexes, 2.0f);
    if (iSize != (pPiece->m_ucFlags & 1)) {
        kMessage.Format("File %s is wrong size!", static_cast<char const*>(kBlendmap1Path));
        MessageBoxA(0, kMessage, "Terrain Texture Error", MB_ICONEXCLAMATION);
        return;
    }
    pPiece->m_iBlendmap2Index = LoadBlendmapFromFile(kBlendmap2Path, &iSize, kHexes, 1.0f);
    if (iSize != (pPiece->m_ucFlags & 1)) {
        kMessage.Format("File %s is wrong size!", static_cast<char const*>(kBlendmap1Path));
        MessageBoxA(0, kMessage, "Terrain Texture Error", MB_ICONEXCLAMATION);
        return;
    }

    int iExtra;
    if (kCsv.GetInt(iNumRows + 6, 0, iExtra)) {
        pPiece->field_3c8 = iExtra + 1;
    }
    pPiece->m_ucFlags |= 4;
    if (kCsv.GetInt(iNumRows + 6, 1, iExtra)) {
        pPiece->m_ucFlags = (pPiece->m_ucFlags & 0xfb) | ((iExtra & 1) << 2);
    }
}

/// 0x0891ff36
int cvCompressedTerrainPiecesLibrary::LoadHeightmapFromFile(char const* pszFile, int* pbSmall) {
    FStringA kMessage;
    FStringA::FStringAFixedBuffer<256> kMessageBuffer(kMessage);
    if (!Fun_0892460c(this, pszFile)) {
        kMessage.Format("Texture load failed!\n%s", pszFile);
        MessageBoxA(0, kMessage, "Terrain Texture Error", MB_ICONEXCLAMATION);
        return -1;
    }
    FireGrafix::TextureData kTexture(m_pFileBuffer, m_uiFileSize, 0x15);
    int iFirstRow;
    if (kTexture.m_nMips == 0) {
        kMessage.Format("Texture has no data!\n%s", pszFile);
        MessageBoxA(0, kMessage, "Terrain Texture Error", MB_ICONEXCLAMATION);
        m_bFileLoaded = false;
        iFirstRow = -1;
    } else if (kTexture.m_ulFormat != 0x24) {
        kMessage.Format("Texture should be A16B16G16R16F or A16B16G16R16\n%s", pszFile);
        MessageBoxA(0, kMessage, "Terrain Texture Error", MB_ICONEXCLAMATION);
        m_bFileLoaded = false;
        iFirstRow = -1;
    } else {
        FireGrafix::TextureData::MipLevel const& kMip = kTexture.m_aMips[0];
        m_bFileLoaded = false;
        iFirstRow = m_aRowOffsets.m_uiSize;
        *pbSmall = (kMip.m_uWidth == 0x80);
        // Rows are stored bottom-up; the height is the second 16-bit channel of each texel.
        unsigned short* pDst = reinterpret_cast<unsigned short*>(m_aScratch);
        for (unsigned int uiRow = 0; uiRow < kMip.m_uHeight; ++uiRow) {
            unsigned short const* pSrc = reinterpret_cast<unsigned short const*>(kMip.m_apFaces[0] + kMip.m_uPitch * (kMip.m_uHeight - 1 - uiRow) + 4);
            for (unsigned int uiColumn = 0; uiColumn < kMip.m_uWidth; ++uiColumn) {
                pDst[uiColumn] = pSrc[uiColumn * 4];
            }
            pDst += kMip.m_uWidth;
        }
        unsigned short* pRow = reinterpret_cast<unsigned short*>(m_aScratch);
        for (unsigned int uiRow = 0; uiRow < kMip.m_uHeight; ++uiRow) {
            unsigned int uiRunStart = m_aHeightRuns.m_uiSize;
            Fun_08924794(&m_aRowOffsets, &uiRunStart);
            RUN_LENGTH_CODING::CompressUShrtDataBlock(pRow, kMip.m_uWidth,
                reinterpret_cast<FFastVector<unsigned short, true, 43u, 265u, TerrainPieceTempHeapAlloc>*>(&m_aHeightRuns));
            pRow += kMip.m_uWidth;
        }
    }
    return iFirstRow;
}

/// 0x0892026c
int cvCompressedTerrainPiecesLibrary::LoadIDMapFromFile(char const* pszFile, int* pbSmall, FStaticVector<HEXVec, 8u, true, 43u, 0u> const& rkHexes) {
    FStringA kMessage;
    FStringA::FStringAFixedBuffer<256> kMessageBuffer(kMessage);
    if (!Fun_0892460c(this, pszFile)) {
        kMessage.Format("Texture load failed!\n%s", pszFile);
        MessageBoxA(0, kMessage, "Terrain Texture Error", MB_ICONEXCLAMATION);
        return -1;
    }
    FireGrafix::TextureData kTexture(m_pFileBuffer, m_uiFileSize, 0x15);
    int iFirstRow;
    if (!Fun_0892089b(&kTexture, pszFile)) {
        m_bFileLoaded = false;
        iFirstRow = -1;
    } else {
        FireGrafix::TextureData::MipLevel const& kMip = kTexture.m_aMips[0];
        iFirstRow = m_aRowOffsets.m_uiSize;
        *pbSmall = (kMip.m_uWidth == 0x80);
        for (unsigned int uiRow = 0; uiRow < kMip.m_uHeight; ++uiRow) {
            unsigned long const* pSrc = reinterpret_cast<unsigned long const*>(kMip.m_apFaces[0] + kMip.m_uPitch * uiRow);
            for (unsigned int uiColumn = 0; uiColumn < kMip.m_uWidth; ++uiColumn) {
                m_aScratch[uiRow * kMip.m_uWidth + uiColumn] = ConvertIDToIndex(pSrc[uiColumn]);
            }
        }
        m_bFileLoaded = false;
        Fun_08924890(this, m_aScratch, kMip.m_uWidth, kMip.m_uHeight, &rkHexes);
        unsigned char* pRow = m_aScratch;
        for (unsigned int uiRow = 0; uiRow < kMip.m_uHeight; ++uiRow) {
            unsigned int uiRunStart = m_aByteRuns.m_uiSize;
            Fun_08924794(&m_aRowOffsets, &uiRunStart);
            RUN_LENGTH_CODING::CompressUCharDataBlock(pRow, kMip.m_uWidth,
                reinterpret_cast<FFastVector<unsigned char, true, 43u, 265u, TerrainPieceTempHeapAlloc>*>(&m_aByteRuns));
            pRow += kMip.m_uWidth;
        }
    }
    return iFirstRow;
}

/// 0x089205a2
int cvCompressedTerrainPiecesLibrary::LoadBlendmapFromFile(char const* pszFile, int* pbSmall, FStaticVector<HEXVec, 8u, true, 43u, 0u> const& rkHexes, float fRadius) {
    FStringA kMessage;
    FStringA::FStringAFixedBuffer<256> kMessageBuffer(kMessage);
    if (!Fun_0892460c(this, pszFile)) {
        kMessage.Format("Texture load failed!\n%s", pszFile);
        MessageBoxA(0, kMessage, "Terrain Texture Error", MB_ICONEXCLAMATION);
        return -1;
    }
    FireGrafix::TextureData kTexture(m_pFileBuffer, m_uiFileSize, 0x15);
    int iFirstRow;
    if (!Fun_0892089b(&kTexture, pszFile)) {
        m_bFileLoaded = false;
        iFirstRow = -1;
    } else {
        FireGrafix::TextureData::MipLevel const& kMip = kTexture.m_aMips[0];
        iFirstRow = m_aRowOffsets.m_uiSize;
        *pbSmall = (kMip.m_uWidth == 0x80);
        // Rows are stored bottom-up; the weight is the third byte of each texel.
        unsigned char* pDst = m_aScratch;
        for (unsigned int uiRow = 0; uiRow < kMip.m_uHeight; ++uiRow) {
            unsigned char const* pSrc = kMip.m_apFaces[0] + kMip.m_uPitch * (kMip.m_uHeight - 1 - uiRow) + 2;
            for (unsigned int uiColumn = 0; uiColumn < kMip.m_uWidth; ++uiColumn) {
                pDst[uiColumn] = pSrc[uiColumn * 4];
            }
            pDst += kMip.m_uWidth;
        }
        m_bFileLoaded = false;
        Fun_08924b3e(this, m_aScratch, kMip.m_uWidth, kMip.m_uHeight, &rkHexes, fRadius);
        unsigned char* pRow = m_aScratch;
        for (unsigned int uiRow = 0; uiRow < kMip.m_uHeight; ++uiRow) {
            unsigned int uiRunStart = m_aByteRuns.m_uiSize;
            Fun_08924794(&m_aRowOffsets, &uiRunStart);
            RUN_LENGTH_CODING::CompressUCharDataBlock(pRow, kMip.m_uWidth,
                reinterpret_cast<FFastVector<unsigned char, true, 43u, 265u, TerrainPieceTempHeapAlloc>*>(&m_aByteRuns));
            pRow += kMip.m_uWidth;
        }
    }
    return iFirstRow;
}

/// 0x08920a04
void cvCompressedTerrainPiecesLibrary::DecompressHeightmapBlock(unsigned int uiFirstRow, FGXInt2 kStart, FGXInt2 kEnd, unsigned short* pOut, bool bSmall) {
    if (kStart.y >= kEnd.y) {
        return;
    }
    int iWidth = bSmall ? 0x80 : 0x200;
    unsigned short aRow[512];
    int iWritten = 0;
    for (int iY = kStart.y; iY < kEnd.y; ++iY) {
        unsigned short const* pRun = m_aHeightRuns.m_pData + m_aRowOffsets.m_pData[iY + uiFirstRow];
        unsigned short* pDst = aRow;
        int iRemaining = iWidth;
        do {
            int iRunLength = pRun[0];
            int iCount = (iRemaining < iRunLength) ? iRemaining : iRunLength;
            for (int i = 0; i < iCount; ++i) {
                *pDst++ = pRun[1];
            }
            iRemaining -= iRunLength;
            pRun += 2;
        } while (iRemaining > 0);
        if (kStart.x < kEnd.x) {
            int iSpan = kEnd.x - kStart.x;
            memcpy(pOut + iWritten, aRow + kStart.x, iSpan * 2);
            iWritten += iSpan;
        }
    }
}

/// 0x08920b24
void cvCompressedTerrainPiecesLibrary::DecompressBlendmapBlock(unsigned int uiFirstRow, FGXInt2 kStart, FGXInt2 kEnd, unsigned char* pOut, bool bSmall) {
    if (kStart.y >= kEnd.y) {
        return;
    }
    int iWidth = bSmall ? 0x80 : 0x200;
    unsigned char aRow[512];
    int iWritten = 0;
    for (int iY = kStart.y; iY < kEnd.y; ++iY) {
        unsigned char const* pRun = m_aByteRuns.m_pData + m_aRowOffsets.m_pData[iY + uiFirstRow];
        unsigned char* pDst = aRow;
        int iRemaining = iWidth;
        do {
            int iRunLength = pRun[0];
            int iCount = (iRemaining < iRunLength) ? iRemaining : iRunLength;
            for (int i = 0; i < iCount; ++i) {
                *pDst++ = pRun[1];
            }
            iRemaining -= iRunLength;
            pRun += 2;
        } while (iRemaining > 0);
        if (kStart.x < kEnd.x) {
            int iSpan = kEnd.x - kStart.x;
            memcpy(pOut + iWritten, aRow + kStart.x, iSpan);
            iWritten += iSpan;
        }
    }
}

/// 0x08920c3e
void cvCompressedTerrainPiecesLibrary::DecompressIDMapBlock(unsigned int uiFirstRow, FGXInt2 kStart, FGXInt2 kEnd, unsigned char* pOut, int bSmall) {
    if (kStart.y >= kEnd.y) {
        return;
    }
    int iWidth = (bSmall != 0) ? 0x80 : 0x200;
    unsigned char aRow[512];
    int iWritten = 0;
    for (int iY = kStart.y; iY < kEnd.y; ++iY) {
        unsigned char const* pRun = m_aByteRuns.m_pData + m_aRowOffsets.m_pData[iY + uiFirstRow];
        unsigned char* pDst = aRow;
        int iRemaining = iWidth;
        do {
            int iRunLength = pRun[0];
            int iCount = (iRemaining < iRunLength) ? iRemaining : iRunLength;
            for (int i = 0; i < iCount; ++i) {
                *pDst++ = pRun[1];
            }
            iRemaining -= iRunLength;
            pRun += 2;
        } while (iRemaining > 0);
        if (kStart.x < kEnd.x) {
            int iSpan = kEnd.x - kStart.x;
            memcpy(pOut + iWritten, aRow + kStart.x, iSpan);
            iWritten += iSpan;
        }
    }
}

/// 0x08920d58
void cvTerrainPiecesLibrary::UnloadTextures() {
    m_aSmallHeightmaps.m_uiSize = 0;
    m_aLargeHeightmaps.m_uiSize = 0;
    m_aSmallIDMaps.m_uiSize = 0;
    m_aLargeIDMaps.m_uiSize = 0;
    m_aSmallBlendmaps.m_uiSize = 0;
    m_aLargeBlendmaps.m_uiSize = 0;
    m_aPieces.m_uiSize = 0;
}

/// 0x08920da4
void cvTerrainPiecesLibrary::LoadTerrainPieces(char const* a1, unsigned long const* a2, unsigned int a3) {
    memcpy(m_aPieceIDs, a2, a3 * 4);
    m_uiNumPieceIDs = a3;
    strcpy(m_szPath, a1);
    LoadFromFlatFiles(a1);
}

/// 0x08920e08
void cvTerrainPiecesLibrary::LoadFromFlatFiles(char const* pszDirectory) {
    Fun_08665a1a(m_pFileBuffer);
    m_uiFileCapacity = 0x400000;
    m_pFileBuffer = Fun_08665a68(0x400000, 4, 0x2b, 0x109);

    FStringA kDirectory;
    FStringA kPattern;
    for (unsigned int uiBaseType = 0; uiBaseType < 0x1c; ++uiBaseType) {
        for (unsigned int uiContinent = 0; uiContinent < 5; ++uiContinent) {
            kDirectory.Format("%s\\%s\\%s", pszDirectory, g_pBaseTypeNames[uiBaseType], g_pContinentNames[uiContinent]);
            kPattern.Format("%s\\*.csv", static_cast<char const*>(kDirectory));
            WIN32_FIND_DATAA kFind;
            HANDLE hFind = FindFirstFileA(kPattern, &kFind);
            if (hFind == INVALID_HANDLE_VALUE) {
                continue;
            }
            do {
                FStringA kName;
                kName.Copy(strlen(kFind.cFileName), kFind.cFileName, 0);
                FStringA kPath;
                kPath.Format("%s\\%s", static_cast<char const*>(kDirectory), static_cast<char const*>(kName));

                unsigned int uiIndex = m_aPieces.m_uiSize;
                if (uiIndex == m_aPieces.m_uiCapacity) {
                    Fun_089251cc(&m_aPieces, uiIndex + 1);
                }
                m_aPieces.m_uiSize = uiIndex + 1;
                TerrainPiece* pPiece = &m_aPieces.m_pData[uiIndex];
                HEXTransform_i kTransform = HEXTransform_i::IDENTITY_TRANSFORM;
                Fun_0891cdd0(pPiece, &kTransform);
                pPiece->field_3c0 = pPiece->m_iNumRows;
                pPiece->field_3c4 = pPiece->m_iNumColumns;
                pPiece->m_iHeightmapIndex = 0xffff;
                pPiece->m_iIDMapIndex = 0xffff;
                pPiece->m_iBlendmap1Index = 0xffff;
                pPiece->m_iBlendmap2Index = 0xffff;
                pPiece->m_iLibraryIndex = m_aPieces.m_uiSize - 1;
                LoadTerrainPiece(kPath, kName, static_cast<unsigned short>(uiBaseType), static_cast<unsigned char>(uiContinent), pPiece);
            } while (FindNextFileA(hFind, &kFind));
            FindClose(hFind);
        }
    }

    unsigned int uiIndex = m_aPieces.m_uiSize;
    if (uiIndex == m_aPieces.m_uiCapacity) {
        Fun_089251cc(&m_aPieces, uiIndex + 1);
    }
    m_aPieces.m_uiSize = uiIndex + 1;
    TerrainPiece* pMissing = &m_aPieces.m_pData[uiIndex];
    HEXTransform_i kTransform = HEXTransform_i::IDENTITY_TRANSFORM;
    Fun_0891cdd0(pMissing, &kTransform);
    pMissing->field_3c0 = pMissing->m_iNumRows;
    pMissing->field_3c4 = pMissing->m_iNumColumns;
    pMissing->m_iHeightmapIndex = 0xffff;
    pMissing->m_iIDMapIndex = 0xffff;
    pMissing->m_iBlendmap1Index = 0xffff;
    pMissing->m_iBlendmap2Index = 0xffff;
    pMissing->m_iLibraryIndex = m_aPieces.m_uiSize - 1;

    FStringA kMissingName;
    kMissingName.Copy(0xf, "missing_hex.csv", 0);
    FStringA kMissingPath;
    kMissingPath.Format("%s\\%s", pszDirectory, static_cast<char const*>(kMissingName));
    LoadTerrainPiece(kMissingPath, kMissingName, 0x1c, 0, pMissing);

    unsigned char* pIDMap = m_aSmallIDMaps.m_pData[pMissing->m_iIDMapIndex].m_aData;
    unsigned char ucMissingID = static_cast<unsigned char>(m_uiNumPieceIDs - 1);
    for (int iRow = 0; iRow < 0x80; ++iRow) {
        for (int iColumn = 0; iColumn < 0x80; ++iColumn) {
            pIDMap[iRow * 0x80 + iColumn] = ucMissingID;
        }
    }
    pMissing->m_sPatternType = 0;

    Fun_08665a1a(m_pFileBuffer);
    m_uiFileSize = 0;
    m_pFileBuffer = 0;
    m_bFileLoaded = false;
    m_uiFileCapacity = 0;
}

/// 0x089216f6
UNK_RET cvTerrainPiecesLibrary::LoadTerrainPiece(FStringA const& rkPath, FStringA const& rkName, unsigned short usBaseType, unsigned char ucContinent, TerrainPiece* pPiece) {
    FStringA kMessage;
    FStringA kDirectory;
    FStringA kFileName;
    rkPath.ExtractFileDir(kDirectory, '\\');
    rkPath.ExtractFileName(kFileName, '\\');

    pPiece->m_usContinent = ucContinent;
    pPiece->m_usBaseType = usBaseType;
    strcpy(pPiece->m_szFileName, kFileName);
    strcpy(pPiece->m_szName, rkName);
    pPiece->m_ucFlags &= 0xfd;
    pPiece->field_3c8 = 2;

    for (int iRow = 0; iRow < pPiece->m_iNumRows; ++iRow) {
        for (int iColumn = 0; iColumn < pPiece->m_iNumColumns; ++iColumn) {
            TerrainPieceCell& kCell = pPiece->m_aCells[iColumn * 10 + iRow];
            kCell.m_ulHexCode = 0xffffffff;
            kCell.m_ucContinent = 0;
            kCell.m_bValid = false;
        }
    }

    FCSVFile kCsv(true, true, false);
    kCsv.Load(rkPath);
    int iVersion;
    bool bHasVersion = kCsv.GetInt(0, 0, iVersion);
    if (iVersion < 3 || !bHasVersion) {
        kMessage.Format("File %s is old and cannot be loaded!", static_cast<char const*>(kFileName));
        MessageBoxA(0, kMessage, "Old piece .csv file", MB_ICONEXCLAMATION);
        return;
    }

    int iNumColumns;
    int iNumRows;
    kCsv.GetInt(5, 0, iNumColumns);
    kCsv.GetInt(5, 1, iNumRows);
    for (int iRow = 0; iRow < iNumRows; ++iRow) {
        for (int iColumn = 0; iColumn < iNumColumns; ++iColumn) {
            FStringA kCellText;
            char const* pszCell = kCsv.Get(iRow + 6, iColumn);
            kCellText.Copy(pszCell ? strlen(pszCell) : 0, pszCell, 0);
            int iValue = kCellText.ConvertToInt(10, true);
            unsigned char ucCellContinent = (iValue < 0) ? 0 : ucContinent;
            unsigned int uiCode = static_cast<unsigned int>(iValue) & 0x7fffffff;
            if (uiCode == 0) {
                uiCode = 0xffffffff;
            }
            unsigned int uiHexCode = uiCode;
            if (iVersion == 3) {
                uiHexCode = 0xffffffff;
                if (uiCode != 0xffffff) {
                    if (uiCode == 0x1800) {
                        uiHexCode = 0x106000;
                    } else {
                        uiHexCode = uiCode;
                        for (unsigned int uiEntry = 0; uiEntry < 0x17; ++uiEntry) {
                            if (g_aLegacyHexCodeKeys[uiEntry] == uiCode) {
                                uiHexCode = g_aLegacyHexCodeValues[g_aLegacyHexCodeIndex[uiEntry]];
                                break;
                            }
                        }
                    }
                }
            }
            TerrainPieceCell& kCell = pPiece->m_aCells[iColumn * 10 + iRow];
            kCell.m_ulHexCode = uiHexCode;
            kCell.m_ucContinent = ucCellContinent;
            kCell.m_bValid = (iValue >= 0);
        }
    }

    pPiece->m_sPatternType = ComputePatternType(*pPiece);
    if (pPiece->m_sPatternType == 4) {
        kMessage.Format("Piece '%s' has invalid hex markup.", pPiece->m_szName);
        MessageBoxA(0, kMessage, "Terrain Piece Error", MB_ICONEXCLAMATION);
    }

    FStaticVector<HEXVec, 8u, true, 43u, 0u> kHexes;
    Fun_089244ae(this, &kHexes, pPiece);

    FStringA kHeightmapPath;
    FStringA kIDMapPath;
    FStringA kBlendmap1Path;
    FStringA kBlendmap2Path;
    char const* pszFile = kCsv.Get(1, 0);
    strcpy(pPiece->m_szHeightmapFile, pszFile);
    kHeightmapPath.Format("%s\\%s", static_cast<char const*>(kDirectory), pszFile);
    pszFile = kCsv.Get(2, 0);
    strcpy(pPiece->m_szIDMapFile, pszFile);
    kIDMapPath.Format("%s\\%s", static_cast<char const*>(kDirectory), pszFile);
    pszFile = kCsv.Get(3, 0);
    strcpy(pPiece->m_szBlendmap1File, pszFile);
    kBlendmap1Path.Format("%s\\%s", static_cast<char const*>(kDirectory), pszFile);
    pszFile = kCsv.Get(4, 0);
    strcpy(pPiece->m_szBlendmap2File, pszFile);
    kBlendmap2Path.Format("%s\\%s", static_cast<char const*>(kDirectory), pszFile);

    // The loaders report 1 for 128x128 maps and 0 for 512x512 ones; all maps of a piece must agree.
    int iSize;
    pPiece->m_iHeightmapIndex = LoadHeightmapFromFile(kHeightmapPath, &iSize);
    pPiece->m_ucFlags = (pPiece->m_ucFlags & 0xfe) | (iSize & 1);
    pPiece->m_iIDMapIndex = LoadIDMapFromFile(kIDMapPath, &iSize);
    if (iSize != (pPiece->m_ucFlags & 1)) {
        kMessage.Format("File %s is wrong size!", static_cast<char const*>(kIDMapPath));
        MessageBoxA(0, kMessage, "Terrain Texture Error", MB_ICONEXCLAMATION);
        return;
    }
    pPiece->m_iBlendmap1Index = LoadBlendmapFromFile(kBlendmap1Path, &iSize, kHexes, 5.0f);
    if (iSize != (pPiece->m_ucFlags & 1)) {
        kMessage.Format("File %s is wrong size!", static_cast<char const*>(kBlendmap1Path));
        MessageBoxA(0, kMessage, "Terrain Texture Error", MB_ICONEXCLAMATION);
        return;
    }
    pPiece->m_iBlendmap2Index = LoadBlendmapFromFile(kBlendmap2Path, &iSize, kHexes, 1.0f);
    if (iSize != (pPiece->m_ucFlags & 1)) {
        kMessage.Format("File %s is wrong size!", static_cast<char const*>(kBlendmap1Path));
        MessageBoxA(0, kMessage, "Terrain Texture Error", MB_ICONEXCLAMATION);
        return;
    }

    int iExtra;
    if (kCsv.GetInt(iNumRows + 6, 0, iExtra)) {
        pPiece->field_3c8 = iExtra + 1;
    }
    pPiece->m_ucFlags |= 4;
    bool bAllLoaded = false;
    if (kCsv.GetInt(iNumRows + 6, 1, iExtra)) {
        pPiece->m_ucFlags = (pPiece->m_ucFlags & 0xfb) | ((iExtra & 1) << 2);
        bAllLoaded = pPiece->m_iHeightmapIndex != -1 && pPiece->m_iIDMapIndex != -1
            && pPiece->m_iBlendmap1Index != -1 && pPiece->m_iBlendmap2Index != -1;
    }
    // A piece whose maps did not all load gives back the slots its loaders appended.
    if (!bAllLoaded) {
        if (iSize != 0) {
            if (pPiece->m_iHeightmapIndex != -1) {
                --m_aSmallHeightmaps.m_uiSize;
            }
            if (pPiece->m_iIDMapIndex != -1) {
                --m_aSmallIDMaps.m_uiSize;
            }
            if (pPiece->m_iBlendmap2Index != -1) {
                --m_aSmallBlendmaps.m_uiSize;
            }
            if (pPiece->m_iBlendmap1Index != -1) {
                --m_aSmallBlendmaps.m_uiSize;
            }
        } else {
            if (pPiece->m_iHeightmapIndex != -1) {
                --m_aLargeHeightmaps.m_uiSize;
            }
            if (pPiece->m_iIDMapIndex != -1) {
                --m_aLargeIDMaps.m_uiSize;
            }
            if (pPiece->m_iBlendmap2Index != -1) {
                --m_aLargeBlendmaps.m_uiSize;
            }
            if (pPiece->m_iBlendmap1Index != -1) {
                --m_aLargeBlendmaps.m_uiSize;
            }
        }
    }
}

/// 0x0892296a
int cvTerrainPiecesLibrary::SaveTerrainPiece(unsigned int uiPieceIndex) {
    TerrainPiece* pPiece = &m_aPieces.m_pData[uiPieceIndex];
    FStringA kDirectory;
    FStringA::FStringAFixedBuffer<256> kDirectoryBuffer(kDirectory);
    FStringA kCsvPath;
    FStringA::FStringAFixedBuffer<256> kCsvPathBuffer(kCsvPath);
    FStringA kHeightmapPath;
    FStringA::FStringAFixedBuffer<256> kHeightmapPathBuffer(kHeightmapPath);
    FStringA kIDMapPath;
    FStringA::FStringAFixedBuffer<256> kIDMapPathBuffer(kIDMapPath);
    FStringA kBlendmap1Path;
    FStringA::FStringAFixedBuffer<256> kBlendmap1PathBuffer(kBlendmap1Path);
    FStringA kBlendmap2Path;
    FStringA::FStringAFixedBuffer<256> kBlendmap2PathBuffer(kBlendmap2Path);

    kDirectory.Format("%s\\%s\\%s", m_szPath, g_pBaseTypeNames[pPiece->m_usBaseType], g_pContinentNames[pPiece->m_usContinent]);
    kCsvPath.Format("%s\\%s", static_cast<char const*>(kDirectory), pPiece->m_szFileName);
    // The heightmap is saved under the first blendmap's file name, which the blendmap then overwrites.
    kHeightmapPath.Format("%s\\%s", static_cast<char const*>(kDirectory), pPiece->m_szBlendmap1File);
    kIDMapPath.Format("%s\\%s", static_cast<char const*>(kDirectory), pPiece->m_szIDMapFile);
    kBlendmap1Path.Format("%s\\%s", static_cast<char const*>(kDirectory), pPiece->m_szBlendmap1File);
    kBlendmap2Path.Format("%s\\%s", static_cast<char const*>(kDirectory), pPiece->m_szBlendmap2File);

    // An existing file that is not writable aborts the save.
    if (_access(kCsvPath, 0) != -1 && _access(kCsvPath, 2) != 0) {
        return 0;
    }
    if (_access(kHeightmapPath, 0) != -1 && _access(kHeightmapPath, 2) != 0) {
        return 0;
    }
    if (_access(kIDMapPath, 0) != -1 && _access(kIDMapPath, 2) != 0) {
        return 0;
    }
    if (_access(kBlendmap1Path, 0) != -1 && _access(kBlendmap1Path, 2) != 0) {
        return 0;
    }
    if (_access(kBlendmap2Path, 0) != -1 && _access(kBlendmap2Path, 2) != 0) {
        return 0;
    }

    {
        FTextFile kFile(kCsvPath, FIFile::modeWrite, 0, 0);
        kFile.WriteF("%d\n", 4);
        kFile.WriteF("%s\n%s\n%s\n%s\n", pPiece->m_szHeightmapFile, pPiece->m_szIDMapFile, pPiece->m_szBlendmap1File, pPiece->m_szBlendmap2File);
        int iSize = (pPiece->m_ucFlags & 1) ? 4 : 10;
        kFile.WriteF("%d, %d\n", iSize, iSize);
        for (int iRow = 0; iRow < iSize; ++iRow) {
            for (int iColumn = 0; iColumn < iSize; ++iColumn) {
                TerrainPieceCell const& kCell = pPiece->m_aCells[iColumn * 10 + iRow];
                unsigned int uiCode = kCell.m_ulHexCode;
                if (!kCell.m_bValid) {
                    uiCode |= 0x80000000;
                }
                kFile.WriteF(iColumn == 0 ? "0x%x" : ",0x%x", uiCode);
            }
            kFile.WriteF("\n");
        }
        kFile.WriteF("%d, %d \n", pPiece->field_3c8 - 1, (pPiece->m_ucFlags >> 2) & 1);
        kFile.Close();
    }

    SaveHeightmapToFile(kHeightmapPath, *pPiece);
    SaveIDMapToFile(kIDMapPath, *pPiece);
    SaveBlendmapToFile(kBlendmap1Path, (pPiece->m_ucFlags & 1) != 0, pPiece->m_iBlendmap1Index);
    SaveBlendmapToFile(kBlendmap2Path, (pPiece->m_ucFlags & 1) != 0, pPiece->m_iBlendmap2Index);
    return 1;
}

/// 0x0892336a
void cvTerrainPiecesLibrary::SaveHeightmapToFile(char const* pszFile, TerrainPiece const& rkPiece) {
    bool bSmall = (rkPiece.m_ucFlags & 1) != 0;
    int iSize = bSmall ? 0x80 : 0x200;
    FireGrafix::TextureData kTexture(0x24, static_cast<FireGrafix::_TextureType>(0), iSize, iSize, 1, 1, 0, 0, 0);
    unsigned short const* pSource = bSmall
        ? m_aSmallHeightmaps.m_pData[rkPiece.m_iHeightmapIndex].m_aHeights
        : m_aLargeHeightmaps.m_pData[rkPiece.m_iHeightmapIndex].m_aHeights;
    FireGrafix::TextureData::MipLevel const& kMip = kTexture.m_aMips[0];
    // Rows are written bottom-up with the height in all four channels.
    for (int iRow = 0; iRow < iSize; ++iRow) {
        unsigned short* pDest = reinterpret_cast<unsigned short*>(kMip.m_apFaces[0] + kMip.m_uPitch * (iSize - 1 - iRow));
        for (int iColumn = 0; iColumn < iSize; ++iColumn) {
            unsigned short usHeight = pSource[iColumn];
            pDest[iColumn * 4] = usHeight;
            pDest[iColumn * 4 + 1] = usHeight;
            pDest[iColumn * 4 + 2] = usHeight;
            pDest[iColumn * 4 + 3] = usHeight;
        }
        pSource += iSize;
    }
    kTexture.SaveDDS(pszFile);
}

/// 0x089234ae
void cvTerrainPiecesLibrary::SaveIDMapToFile(char const* pszFile, TerrainPiece const& rkPiece) {
    bool bSmall = (rkPiece.m_ucFlags & 1) != 0;
    int iSize = bSmall ? 0x80 : 0x200;
    FireGrafix::TextureData kTexture(0x15, static_cast<FireGrafix::_TextureType>(0), iSize, iSize, 1, 1, 0, 0, 0);
    unsigned char const* pSource = bSmall
        ? m_aSmallIDMaps.m_pData[rkPiece.m_iIDMapIndex].m_aData
        : m_aLargeIDMaps.m_pData[rkPiece.m_iIDMapIndex].m_aData;
    FireGrafix::TextureData::MipLevel const& kMip = kTexture.m_aMips[0];
    for (int iRow = 0; iRow < iSize; ++iRow) {
        unsigned long* pDest = reinterpret_cast<unsigned long*>(kMip.m_apFaces[0] + kMip.m_uPitch * iRow);
        for (int iColumn = 0; iColumn < iSize; ++iColumn) {
            pDest[iColumn] = m_aPieceIDs[pSource[iColumn]];
        }
        pSource += iSize;
    }
    kTexture.SaveDDS(pszFile);
}

/// 0x089235d4
void cvTerrainPiecesLibrary::SaveBlendmapToFile(char const* pszFile, bool bSmall, unsigned int uiIndex) {
    int iSize = bSmall ? 0x80 : 0x200;
    FireGrafix::TextureData kTexture(0x15, static_cast<FireGrafix::_TextureType>(0), iSize, iSize, 1, 1, 0, 0, 0);
    unsigned char const* pSource = bSmall
        ? m_aSmallBlendmaps.m_pData[uiIndex].m_aData
        : m_aLargeBlendmaps.m_pData[uiIndex].m_aData;
    FireGrafix::TextureData::MipLevel const& kMip = kTexture.m_aMips[0];
    // Rows are written bottom-up with the weight replicated into all four channels.
    for (int iRow = 0; iRow < iSize; ++iRow) {
        unsigned int* pDest = reinterpret_cast<unsigned int*>(kMip.m_apFaces[0] + kMip.m_uPitch * (iSize - 1 - iRow));
        for (int iColumn = 0; iColumn < iSize; ++iColumn) {
            pDest[iColumn] = pSource[iColumn] * 0x1010101u;
        }
        pSource += iSize;
    }
    kTexture.SaveDDS(pszFile);
}

/// 0x08923750
int cvTerrainPiecesLibrary::LoadHeightmapFromFile(char const* pszFile, int* pbSmall) {
    FStringA kMessage;
    FStringA::FStringAFixedBuffer<256> kMessageBuffer(kMessage);
    if (!Fun_0892460c(this, pszFile)) {
        kMessage.Format("Texture load failed!\n%s", pszFile);
        MessageBoxA(0, kMessage, "Terrain Texture Error", MB_ICONEXCLAMATION);
        return -1;
    }
    FireGrafix::TextureData kTexture(m_pFileBuffer, m_uiFileSize, 0x15);
    int iIndex;
    if (kTexture.m_nMips == 0) {
        kMessage.Format("Texture has no data!\n%s", pszFile);
        MessageBoxA(0, kMessage, "Terrain Texture Error", MB_ICONEXCLAMATION);
        m_bFileLoaded = false;
        iIndex = -1;
    } else if (kTexture.m_ulFormat != 0x24) {
        kMessage.Format("Texture should be A16B16G16R16F or A16B16G16R16\n%s", pszFile);
        MessageBoxA(0, kMessage, "Terrain Texture Error", MB_ICONEXCLAMATION);
        m_bFileLoaded = false;
        iIndex = -1;
    } else {
        FireGrafix::TextureData::MipLevel const& kMip = kTexture.m_aMips[0];
        m_bFileLoaded = false;
        unsigned short* pDest;
        if (kMip.m_uWidth == 0x80) {
            *pbSmall = 1;
            iIndex = m_aSmallHeightmaps.m_uiSize;
            if (m_aSmallHeightmaps.m_uiSize == m_aSmallHeightmaps.m_uiCapacity) {
                Fun_08925726(&m_aSmallHeightmaps, iIndex + 1);
            }
            m_aSmallHeightmaps.m_uiSize = iIndex + 1;
            Heightmap128& kMap = m_aSmallHeightmaps.m_pData[iIndex];
            kMap.m_uiWidth = 0x80;
            kMap.m_uiHeight = 0x80;
            pDest = kMap.m_aHeights;
        } else {
            *pbSmall = 0;
            iIndex = m_aLargeHeightmaps.m_uiSize;
            if (m_aLargeHeightmaps.m_uiSize == m_aLargeHeightmaps.m_uiCapacity) {
                Fun_0892566e(&m_aLargeHeightmaps, iIndex + 1);
            }
            m_aLargeHeightmaps.m_uiSize = iIndex + 1;
            Heightmap512& kMap = m_aLargeHeightmaps.m_pData[iIndex];
            kMap.m_uiWidth = 0x200;
            kMap.m_uiHeight = 0x200;
            pDest = kMap.m_aHeights;
        }
        // Rows are stored bottom-up; the height is the second 16-bit channel of each texel.
        for (unsigned int uiRow = 0; uiRow < kMip.m_uHeight; ++uiRow) {
            unsigned short const* pSrc = reinterpret_cast<unsigned short const*>(kMip.m_apFaces[0] + kMip.m_uPitch * (kMip.m_uHeight - 1 - uiRow) + 4);
            for (unsigned int uiColumn = 0; uiColumn < kMip.m_uWidth; ++uiColumn) {
                pDest[uiColumn] = pSrc[uiColumn * 4];
            }
            pDest += kMip.m_uWidth;
        }
    }
    return iIndex;
}

/// 0x08923ada
int cvTerrainPiecesLibrary::LoadIDMapFromFile(char const* pszFile, int* pbSmall) {
    FStringA kMessage;
    FStringA::FStringAFixedBuffer<256> kMessageBuffer(kMessage);
    if (!Fun_0892460c(this, pszFile)) {
        kMessage.Format("Texture load failed!\n%s", pszFile);
        MessageBoxA(0, kMessage, "Terrain Texture Error", MB_ICONEXCLAMATION);
        return -1;
    }
    FireGrafix::TextureData kTexture(m_pFileBuffer, m_uiFileSize, 0x15);
    int iIndex;
    if (!Fun_0892089b(&kTexture, pszFile)) {
        m_bFileLoaded = false;
        iIndex = -1;
    } else {
        FireGrafix::TextureData::MipLevel const& kMip = kTexture.m_aMips[0];
        unsigned char* pDest;
        if (kMip.m_uWidth == 0x80) {
            *pbSmall = 1;
            iIndex = m_aSmallIDMaps.m_uiSize;
            if (m_aSmallIDMaps.m_uiSize == m_aSmallIDMaps.m_uiCapacity) {
                Fun_089255b6(&m_aSmallIDMaps, iIndex + 1);
            }
            m_aSmallIDMaps.m_uiSize = iIndex + 1;
            Bytemap128& kMap = m_aSmallIDMaps.m_pData[iIndex];
            kMap.m_uiWidth = 0x80;
            kMap.m_uiHeight = 0x80;
            pDest = kMap.m_aData;
        } else {
            *pbSmall = 0;
            iIndex = m_aLargeIDMaps.m_uiSize;
            if (m_aLargeIDMaps.m_uiSize == m_aLargeIDMaps.m_uiCapacity) {
                Fun_089254fe(&m_aLargeIDMaps, iIndex + 1);
            }
            m_aLargeIDMaps.m_uiSize = iIndex + 1;
            Bytemap512& kMap = m_aLargeIDMaps.m_pData[iIndex];
            kMap.m_uiWidth = 0x200;
            kMap.m_uiHeight = 0x200;
            pDest = kMap.m_aData;
        }
        for (unsigned int uiRow = 0; uiRow < kMip.m_uHeight; ++uiRow) {
            unsigned long const* pSrc = reinterpret_cast<unsigned long const*>(kMip.m_apFaces[0] + kMip.m_uPitch * uiRow);
            for (unsigned int uiColumn = 0; uiColumn < kMip.m_uWidth; ++uiColumn) {
                pDest[uiRow * kMip.m_uWidth + uiColumn] = ConvertIDToIndex(pSrc[uiColumn]);
            }
        }
        m_bFileLoaded = false;
    }
    return iIndex;
}

/// 0x08923e20
int cvTerrainPiecesLibrary::LoadBlendmapFromFile(char const* pszFile, int* pbSmall, FStaticVector<HEXVec, 8u, true, 43u, 0u> const& rkHexes, float fRadius) {
    FStringA kMessage;
    FStringA::FStringAFixedBuffer<256> kMessageBuffer(kMessage);
    if (!Fun_0892460c(this, pszFile)) {
        kMessage.Format("Texture load failed!\n%s", pszFile);
        MessageBoxA(0, kMessage, "Terrain Texture Error", MB_ICONEXCLAMATION);
        return -1;
    }
    FireGrafix::TextureData kTexture(m_pFileBuffer, m_uiFileSize, 0x15);
    int iIndex;
    if (!Fun_0892089b(&kTexture, pszFile)) {
        m_bFileLoaded = false;
        iIndex = -1;
    } else {
        FireGrafix::TextureData::MipLevel const& kMip = kTexture.m_aMips[0];
        unsigned char* pMap;
        if (kMip.m_uWidth == 0x80) {
            *pbSmall = 1;
            iIndex = m_aSmallBlendmaps.m_uiSize;
            if (m_aSmallBlendmaps.m_uiSize == m_aSmallBlendmaps.m_uiCapacity) {
                Fun_089255b6(&m_aSmallBlendmaps, iIndex + 1);
            }
            m_aSmallBlendmaps.m_uiSize = iIndex + 1;
            Bytemap128& kMap = m_aSmallBlendmaps.m_pData[iIndex];
            kMap.m_uiWidth = 0x80;
            kMap.m_uiHeight = 0x80;
            pMap = kMap.m_aData;
        } else {
            *pbSmall = 0;
            iIndex = m_aLargeBlendmaps.m_uiSize;
            if (m_aLargeBlendmaps.m_uiSize == m_aLargeBlendmaps.m_uiCapacity) {
                Fun_089254fe(&m_aLargeBlendmaps, iIndex + 1);
            }
            m_aLargeBlendmaps.m_uiSize = iIndex + 1;
            Bytemap512& kMap = m_aLargeBlendmaps.m_pData[iIndex];
            kMap.m_uiWidth = 0x200;
            kMap.m_uiHeight = 0x200;
            pMap = kMap.m_aData;
        }
        // Rows are stored bottom-up; the weight is the third byte of each texel.
        unsigned char* pDest = pMap;
        for (unsigned int uiRow = 0; uiRow < kMip.m_uHeight; ++uiRow) {
            unsigned char const* pSrc = kMip.m_apFaces[0] + kMip.m_uPitch * (kMip.m_uHeight - 1 - uiRow) + 2;
            for (unsigned int uiColumn = 0; uiColumn < kMip.m_uWidth; ++uiColumn) {
                pDest[uiColumn] = pSrc[uiColumn * 4];
            }
            pDest += kMip.m_uWidth;
        }
        m_bFileLoaded = false;
        Fun_08924b3e(this, pMap, kMip.m_uWidth, kMip.m_uHeight, &rkHexes, fRadius);
    }
    return iIndex;
}

/// 0x08924152
void cvTerrainPiecesLibrary::AddTerrainPiece_Small(TerrainPiece const& a1) {
    unsigned int uiIndex = m_aPieces.m_uiSize;
    if (uiIndex == m_aPieces.m_uiCapacity) {
        Fun_089251cc(&m_aPieces, uiIndex + 1);
    }
    m_aPieces.m_uiSize = uiIndex + 1;
    TerrainPiece* pNew = &m_aPieces.m_pData[uiIndex];
    memcpy(pNew, &a1, kPieceStride - 2);
    pNew->m_sPatternType = ComputePatternType(*pNew);
    Fun_08924d12(this, pNew);
}

/// 0x089242be
void cvTerrainPiecesLibrary::AddTerrainPiece_Large(TerrainPiece const& a1) {
    unsigned int uiIndex = m_aPieces.m_uiSize;
    if (uiIndex == m_aPieces.m_uiCapacity) {
        Fun_089251cc(&m_aPieces, uiIndex + 1);
    }
    m_aPieces.m_uiSize = uiIndex + 1;
    TerrainPiece* pNew = &m_aPieces.m_pData[uiIndex];
    memcpy(pNew, &a1, kPieceStride - 2);
    pNew->m_sPatternType = ComputePatternType(*pNew);
    Fun_08924d12(this, pNew);
}
