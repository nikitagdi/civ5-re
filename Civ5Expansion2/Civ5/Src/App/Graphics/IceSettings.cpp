/// origin guess
#include <cmath>
#include <cstring>
#include "Civ5/Src/App/Graphics/IceSettings.h"
#include "FirePlace/FireWorks/FString.h"
#include "FirePlace/FireWorks/FXmlAttribute.h"
#include "FirePlace/FireWorks/FXmlDocument.h"
#include "FirePlace/FireWorks/FXmlTraverser.h"

/// 0x0890a86c
bool TerrainIce::IceSettings::ReadXML(char const* pszFile) {
    m_bDumpInput = false;
    m_uSplineSubdivision = 6;
    field_8 = 0;
    m_uNearSamples = 0x10;
    m_uFarSamples = 0x30;
    m_fNearSampleDist = 0.1f;
    m_fFarSampleDist = 2.8f;
    m_fInitialPotential = 40.0f;
    m_fCrackCost = 1.0f;
    m_fBoundaryKillRate = 0.6f;
    m_fInteriorKillRate = 0.05f;
    m_fCrackMinClip = 0.05f;
    m_fCrackMaxClip = 0.8f;
    m_fEdgePullAmount = 0.25f;
    m_fSmallPolyShrinkAmount = 0.35f;
    m_fLatticeVertsPerHex = 12.0f;
    m_fSmallPolyHeightMin = 0.6f;
    m_fSmallPolyHeightMax = 0.6f;
    m_fLargePolyHeight = 10.0f;
    m_fLargePolyEdgeHeightMin = 0.0f;
    m_fLargePolyEdgeHeightMax = 0.0f;
    m_fLargePolyEdgeHeightFreq = 1.0f;
    m_fSkirtSlopeSmall = 0.0078125f;
    m_fSkirtSlopeLarge = 0.0015625f;
    m_fMaxHeight = 10.0f;
    m_fTextureRepeat = 0.005f;
    m_fBumpRepeat = 0.3f;
    m_fSpecularPower = 50.0f;
    m_afSubsurface[0] = 0.0f;
    m_afSubsurface[1] = 0.0f;
    m_afSubsurface[2] = 1.0f;
    m_afUnderwater1[0] = 1.0f;
    m_afUnderwater1[1] = 1.0f;
    m_afUnderwater1[2] = 1.0f;
    m_afUnderwater2[0] = 0.0f;
    m_afUnderwater2[1] = 0.1f;
    m_afUnderwater2[2] = 0.0f;
    m_fUnderwaterColorRamp = 0.25f;
    m_fUnderwaterAlphaRamp = 0.25f;

    FXmlDocument kDocument;
    FXmlTraverser kRoot;
    if (!kDocument.Load(pszFile)) {
        return false;
    }
    kDocument.GetRootElement(kRoot);
    FStringA kName;
    FStringA::FStringAFixedBuffer<128> kNameBuffer(kName);
    if (!kRoot.GetName(kName) || strcmp(kName.c_str(), "IceSettings") != 0) {
        return false;
    }

    unsigned int uCount = kRoot.GetAttributeCount();
    FXmlAttribute kAttribute;
    kRoot.GetFirstAttribute(kAttribute);
    for (unsigned int i = 0; i < uCount; ++i) {
        kAttribute.GetName(kName);
        char const* pszKey = kName.c_str();
        float fValue;
        kAttribute.GetVal(fValue);
        kAttribute.GetNextAttribute(kAttribute);
        const auto is = [pszKey](char const* pszName) { return strcmp(pszKey, pszName) == 0; };
        // RGB attributes are 0..255 sRGB values stored as linear floats.
        const auto toLinear = [fValue]() { return powf(fValue / 255.0f, 2.2f); };

        if (is("DumpInput")) {
            m_bDumpInput = fValue != 0.0f;
        } else if (is("SplineSubdivision")) {
            m_uSplineSubdivision = static_cast<unsigned int>(fValue);
        } else if (is("FarSampleDist")) {
            m_fFarSampleDist = fValue;
        } else if (is("FarSamples")) {
            m_uFarSamples = static_cast<unsigned int>(fValue);
        } else if (is("NearSampleDist")) {
            m_fNearSampleDist = fValue;
        } else if (is("NearSamples")) {
            m_uNearSamples = static_cast<unsigned int>(fValue);
        } else if (is("CrackCost")) {
            m_fCrackCost = fValue;
        } else if (is("InitialPotential")) {
            m_fInitialPotential = fValue;
        } else if (is("CrackMinClip")) {
            m_fCrackMinClip = fValue;
        } else if (is("CrackMaxClip")) {
            m_fCrackMaxClip = fValue;
        } else if (is("EdgePullAmount")) {
            m_fEdgePullAmount = fValue;
        } else if (is("SmallPolyShrinkAmount")) {
            m_fSmallPolyShrinkAmount = fValue;
        } else if (is("LatticeVertsPerHex")) {
            m_fLatticeVertsPerHex = fValue;
        } else if (is("SmallPolyHeightMin")) {
            m_fSmallPolyHeightMin = fValue;
        } else if (is("SmallPolyHeightMax")) {
            m_fSmallPolyHeightMax = fValue;
        } else if (is("LargePolyHeight")) {
            m_fLargePolyHeight = fValue;
        } else if (is("LargePolyEdgeHeightMin")) {
            m_fLargePolyEdgeHeightMin = fValue;
        } else if (is("LargePolyEdgeHeightMax")) {
            m_fLargePolyEdgeHeightMax = fValue;
        } else if (is("LargePolyEdgeHeightFreq")) {
            m_fLargePolyEdgeHeightFreq = fValue;
        } else if (is("SkirtSlopeSmall")) {
            m_fSkirtSlopeSmall = fValue;
        } else if (is("SkirtSlopeLarge")) {
            m_fSkirtSlopeLarge = fValue;
        } else if (is("DisplaceFrequency")) {
            m_fDisplaceFrequency = fValue;
        } else if (is("DisplaceMin")) {
            m_fDisplaceMin = fValue;
        } else if (is("DisplaceMax")) {
            m_fDisplaceMax = fValue;
        } else if (is("DisplaceExponent")) {
            m_fDisplaceExponent = fValue;
        } else if (is("BoundaryKillRate")) {
            m_fBoundaryKillRate = fValue;
        } else if (is("InteriorKillRate")) {
            m_fInteriorKillRate = fValue;
        } else if (is("TextureRepeat")) {
            m_fTextureRepeat = fValue;
        } else if (is("BumpRepeat")) {
            m_fBumpRepeat = fValue;
        } else if (is("SideTextureRepeat_U")) {
            m_fSideTextureRepeatU = fValue;
        } else if (is("SideBumpRepeat_U")) {
            m_fSideBumpRepeatU = fValue;
        } else if (is("SideTextureRepeat_V")) {
            m_fSideTextureRepeatV = fValue;
        } else if (is("SideBumpRepeat_V")) {
            m_fSideBumpRepeatV = fValue;
        } else if (is("SpecularPower")) {
            m_fSpecularPower = fValue;
        } else if (is("Subsurface_R")) {
            m_afSubsurface[0] = toLinear();
        } else if (is("Subsurface_G")) {
            m_afSubsurface[1] = toLinear();
        } else if (is("Subsurface_B")) {
            m_afSubsurface[2] = toLinear();
        } else if (is("LargePolyDepth")) {
            m_fLargePolyDepth = fValue;
        } else if (is("SmallPolyDepthMin")) {
            m_fSmallPolyDepthMin = fValue;
        } else if (is("SmallPolyDepthMax")) {
            m_fSmallPolyDepthMax = fValue;
        } else if (is("Underwater_R1")) {
            m_afUnderwater1[0] = toLinear();
        } else if (is("Underwater_G1")) {
            m_afUnderwater1[1] = toLinear();
        } else if (is("Underwater_B1")) {
            m_afUnderwater1[2] = toLinear();
        } else if (is("Underwater_R2")) {
            m_afUnderwater2[0] = toLinear();
        } else if (is("Underwater_G2")) {
            m_afUnderwater2[1] = toLinear();
        } else if (is("Underwater_B2")) {
            m_afUnderwater2[2] = toLinear();
        } else if (is("Underwater_ColorRamp")) {
            m_fUnderwaterColorRamp = 1.0f / fValue;
        } else if (is("Underwater_AlphaRamp")) {
            m_fUnderwaterAlphaRamp = 1.0f / fValue;
        }
    }

    m_fMaxHeight = m_fLargePolyHeight + m_fDisplaceMax;
    m_fMaxDepth = std::fmax(std::fmax(m_fLargePolyDepth, m_fSmallPolyDepthMax), m_fSmallPolyDepthMin);
    return true;
}
