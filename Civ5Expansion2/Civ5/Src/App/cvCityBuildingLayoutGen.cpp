/// origin guess
#include "Civ5/Src/App/cvCityBuildingLayoutGen.h"
#include <cmath>
#include <cstring>
#include "FirePlace/FireWorks/FMemHooks.h"

/// 0x08a034d8
void cvCityBuildingLayoutGen::InitializePopBuildingPlacementSet(FGXMathRect::Rect const& rect, TensorFieldType const& tensorField)
{
    const unsigned int kPoolListEnd = 0x0fffffff;

    // Takes the free-list head or appends a slot (capacity doubles from 1); returns the particle index.
    auto allocateParticle = [this, kPoolListEnd](const PlacementParticle& init) -> unsigned int {
        ParticlePool& pool = m_Particles;
        unsigned int nIndex = pool.m_nFreeHead;
        if (nIndex == kPoolListEnd) {
            nIndex = pool.m_nParticleCount;
            if (nIndex == pool.m_nParticleCap) {
                unsigned int nOldCap = pool.m_nParticleCap;
                unsigned int nCap = nOldCap == 0 ? 1 : nOldCap;
                while (nCap <= nIndex) {
                    if (nCap * 2 < nCap) {
                        nCap = nIndex;
                        break;
                    }
                    nCap *= 2;
                }
                pool.m_nParticleCap = nCap;
                PlacementParticle* pNew = static_cast<PlacementParticle*>(FireMallocAlignedNoTracking(
                    nCap * sizeof(PlacementParticle), 4, "Port/FirePlace/FireWorks/FFastVector.h", 0x55));
                if (pNew != nullptr) {
                    std::memcpy(pNew, pool.m_pParticles, pool.m_nParticleCount * sizeof(PlacementParticle));
                    FireFreeAlignedNoTracking(pool.m_pParticles);
                    pool.m_pParticles = pNew;
                } else {
                    pool.m_nParticleCap = nOldCap;
                }
            }
            pool.m_pParticles[nIndex] = init;
            ++pool.m_nParticleCount;
        } else {
            pool.m_nFreeHead = pool.m_pParticles[nIndex].m_nNextFree;
            pool.m_pParticles[nIndex] = init;
        }
        pool.m_pParticles[nIndex].m_bFree = 0;
        ++pool.m_nLiveCount;
        return nIndex;
    };

    m_Particles.m_nFreeHead = kPoolListEnd;
    m_Particles.m_nParticleCount = 0;
    m_Particles.m_nLiveCount = 0;
    m_pTensorField = &tensorField;
    m_fMinX = rect.m_fMinX;
    m_fMinY = rect.m_fMinY;
    m_fMaxX = rect.m_fMaxX;
    m_fMaxY = rect.m_fMaxY;

    PlacementParticle seed = {};
    seed.m_nHashNext = 0xffff;
    std::memset(m_anHashHeads, 0xff, sizeof(m_anHashHeads));

    // Diagonal lattice: the (i, j) grid is rotated by 45 degrees and scaled to the 128 unit square.
    const int nSide = static_cast<int>(181.01933f / m_fSpacing);
    if (nSide <= 0) {
        return;
    }
    const float fStepX = 128.0f / static_cast<float>(nSide);
    const float fStepY = -128.0f / static_cast<float>(nSide);
    for (int i = 0; i != nSide; ++i) {
        const float fBase = static_cast<float>(i) * fStepX;
        for (int j = 0; j != nSide; ++j) {
            float fX = fStepX * static_cast<float>(j) + (fBase + -64.0f);
            float fY = static_cast<float>(j) * fStepY + (fBase + 64.0f);
            int nBinX = static_cast<int>(fX * 0.25f);
            int nBinY = static_cast<int>(fY * 0.25f);
            if ((nBinX | nBinY) < 0 || nBinX > 0x1f || nBinY > 0x1f) {
                continue;
            }
            seed.m_nHashNext = m_anHashHeads[nBinY][nBinX];
            seed.m_fX = fX + rect.m_fMinX;
            seed.m_fY = fY + rect.m_fMinY;
            m_anHashHeads[nBinY][nBinX] = static_cast<unsigned short>(allocateParticle(seed));
        }
    }
}

/// 0x08a0374c
void cvCityBuildingLayoutGen::Tick(unsigned char nFlags)
{
    const float kTensorScale = 1.0f / 128.0f;
    const bool bPrune = (nFlags & 1) != 0;
    const bool bKillOnMask = (nFlags & 2) != 0;
    const unsigned int nCount = m_Particles.m_nParticleCount;
    if (nCount == 0) {
        std::memset(m_anHashHeads, 0xff, sizeof(m_anHashHeads));
        return;
    }

    // Free list insert: a lower index becomes the head, otherwise it follows the head.
    auto freeParticle = [this](unsigned int nKill) {
        PlacementParticle* pParticles = m_Particles.m_pParticles;
        pParticles[nKill].m_bFree = 1;
        if (nKill < m_Particles.m_nFreeHead) {
            pParticles[nKill].m_nNextFree = m_Particles.m_nFreeHead;
            m_Particles.m_nFreeHead = nKill;
        } else {
            pParticles[nKill].m_nNextFree = pParticles[m_Particles.m_nFreeHead].m_nNextFree;
            pParticles[m_Particles.m_nFreeHead].m_nNextFree = nKill;
        }
        --m_Particles.m_nLiveCount;
    };

    // t is the band position clamped to 0..1, u its complement.
    auto forceMagnitude = [](const ForceProfile& profile, float fDistance) -> float {
        float fBand = (fDistance - profile.m_fRadius) / profile.m_fRange;
        float t = fBand < 1.0f ? fBand : 1.0f;
        t = 0.0f > t ? 0.0f : t;
        float u = 1.0f - t;
        float fOuter = ((u * 3.0f + t) * (t * t)) * profile.m_fStrengthB;
        float fInner = ((t * 3.0f + u) * (u * u)) * profile.m_fStrengthA;
        return fInner + fOuter;
    };

    // Each channel: two truncations to a signed byte, then the vertical weighting.
    auto interpolateChannel = [](signed char c00, signed char c01, signed char c10, signed char c11,
                                 float fx, float fy) -> signed char {
        float fTop = static_cast<float>(static_cast<signed char>(static_cast<int>(c00 * (1.0f - fx) + c01 * fx)));
        float fBottom = static_cast<float>(static_cast<signed char>(static_cast<int>(c10 * (1.0f - fx) + c11 * fx)));
        return static_cast<signed char>(static_cast<int>(fTop * (1.0f - fy) + fBottom * fy));
    };

    for (unsigned int nIndex = 0; nIndex < m_Particles.m_nParticleCount; ++nIndex) {
        PlacementParticle* pParticles = m_Particles.m_pParticles;
        if (pParticles[nIndex].m_bFree) {
            continue;
        }
        PlacementParticle& particle = pParticles[nIndex];
        const bool bInside = m_fMinX <= particle.m_fX && m_fMaxX > particle.m_fX &&
                             m_fMinY <= particle.m_fY && m_fMaxY > particle.m_fY;
        if (!bInside) {
            freeParticle(nIndex);
            continue;
        }

        const int nBinX = static_cast<int>((particle.m_fX - m_fMinX) * 0.25f);
        const int nBinY = static_cast<int>((particle.m_fY - m_fMinY) * 0.25f);
        const int nBinX0 = nBinX - m_nHashRadius < 0 ? 0 : nBinX - m_nHashRadius;
        const int nBinY0 = nBinY - m_nHashRadius < 0 ? 0 : nBinY - m_nHashRadius;
        const int nBinXEnd = nBinX + m_nHashRadius + 1 >= 32 ? 0x1f : nBinX + m_nHashRadius + 1;
        const int nBinYEnd = nBinY + m_nHashRadius + 1 >= 32 ? 0x1f : nBinY + m_nHashRadius + 1;
        const FieldFrame& frame = *reinterpret_cast<const FieldFrame*>(
            reinterpret_cast<const unsigned char*>(m_pTensorField) + sizeof(m_pTensorField->m_Data));
        const FTensor::TensorC* cells = &m_pTensorField->m_Data.m_aCells[0][0];

        for (int nRow = nBinY0; nRow < nBinYEnd; ++nRow) {
            for (int nCol = nBinX0; nCol < nBinXEnd; ++nCol) {
                unsigned int nOther = m_anHashHeads[nRow][nCol];
                while (nOther != 0xffff && m_Particles.m_nParticleCount > nOther) {
                    PlacementParticle& other = m_Particles.m_pParticles[nOther];
                    if (other.m_bFree) {
                        break;
                    }
                    const unsigned int nCurrent = nOther;
                    nOther = other.m_nHashNext;
                    if (nCurrent == nIndex) {
                        continue;
                    }

                    const float fOtherX = other.m_fX;
                    const float fOtherY = other.m_fY;
                    float fMidX = (fOtherX + particle.m_fX) * 0.5f;
                    while (m_fWrapMin > fMidX) {
                        fMidX += m_fWrapWidth;
                    }
                    while (fMidX > m_fWrapMax) {
                        fMidX -= m_fWrapWidth;
                    }
                    const float fMidY = (fOtherY + particle.m_fY) * 0.5f;
                    if (!(frame.m_fMaxX > fMidX && fMidX >= frame.m_fMinX &&
                          frame.m_fMaxY > fMidY && fMidY >= frame.m_fMinY)) {
                        continue;
                    }

                    const float fDeltaX = particle.m_fX - fOtherX;
                    const float fDeltaY = particle.m_fY - fOtherY;

                    const int nTensorRow = static_cast<int>((fMidY - frame.m_fMinY) * frame.m_fScaleY);
                    const int nTensorCol = static_cast<int>((fMidX - frame.m_fMinX) * frame.m_fScaleX);
                    const float fx = fMidX - std::floor(fMidX);
                    const float fy = fMidY - std::floor(fMidY);
                    const int nStride = frame.m_nWidth;
                    const FTensor::TensorC& c00 = cells[nTensorRow * nStride + nTensorCol];
                    const FTensor::TensorC& c01 = cells[nTensorRow * nStride + nTensorCol + 1];
                    const FTensor::TensorC& c10 = cells[(nTensorRow + 1) * nStride + nTensorCol];
                    const FTensor::TensorC& c11 = cells[(nTensorRow + 1) * nStride + nTensorCol + 1];
                    const signed char nDirX = interpolateChannel(c00.m_cX, c01.m_cX, c10.m_cX, c11.m_cX, fx, fy);
                    const signed char nDirY = interpolateChannel(c00.m_cY, c01.m_cY, c10.m_cY, c11.m_cY, fx, fy);
                    const float fTensorX = nDirX * kTensorScale;
                    const float fTensorY = nDirY * kTensorScale;
                    particle.m_fTensorX = fTensorX;
                    particle.m_fTensorY = fTensorY;

                    const float fTensorLength = std::sqrt(fTensorY * fTensorY + fTensorX * fTensorX);
                    float fUnitX = 0.0f;
                    float fUnitY = 0.0f;
                    if (fTensorLength != 0.0f || std::isnan(fTensorLength)) {
                        fUnitX = fTensorX / fTensorLength;
                        fUnitY = fTensorY / fTensorLength;
                    }
                    const float fFrameX = m_afFrameMatrix[1] * fUnitY + m_afFrameMatrix[0] * fUnitX;
                    const float fFrameY = fUnitY * m_afFrameMatrix[3] + fUnitX * m_afFrameMatrix[2];
                    const float fFrameLength = sqrtf(fFrameY * fFrameY + fFrameX * fFrameX);
                    const float fAxisX = fFrameX / fFrameLength;
                    const float fAxisY = fFrameY / fFrameLength;

                    // The along-axis term adds fDeltaY and fAxisY instead of multiplying (as in the binary).
                    const float fAcross = std::fabs(fAxisY * fDeltaX - fAxisX * fDeltaY);
                    const float fAlong = std::fabs(fDeltaX * fAxisX + fDeltaY + fAxisY);
                    const float fDistance = fAcross + fAlong;

                    if (bPrune && m_fMinNeighbourDistance > fDistance) {
                        freeParticle(nCurrent);
                        continue;
                    }
                    const float fMagnitude = forceMagnitude(m_aForceProfiles[0], fDistance);
                    const float fForceX = (fDeltaX / fDistance) * fMagnitude;
                    const float fForceY = (fDeltaY / fDistance) * fMagnitude;
                    particle.m_fForceX = particle.m_fForceX + fForceX;
                    particle.m_fForceY = particle.m_fForceY + fForceY;
                    other.m_fForceX = other.m_fForceX - fForceX;
                    other.m_fForceY = other.m_fForceY - fForceY;
                }
            }
        }

        const int nCellX = static_cast<int>(particle.m_fX - m_fMinX);
        const int nCellY = static_cast<int>(particle.m_fY - m_fMinY);
        if (bKillOnMask && m_abBlockedMask[nCellY * 128 + nCellX] != 0) {
            freeParticle(nIndex);
            continue;
        }
        const int nCellX0 = nCellX - m_nMaskRadius < 0 ? 0 : nCellX - m_nMaskRadius;
        const int nCellY0 = nCellY - m_nMaskRadius < 0 ? 0 : nCellY - m_nMaskRadius;
        const int nCellXEnd = nCellX + m_nMaskRadius + 1 >= 0x80 ? 0x7f : nCellX + m_nMaskRadius + 1;
        const int nCellYEnd = nCellY + m_nMaskRadius + 1 >= 0x80 ? 0x7f : nCellY + m_nMaskRadius + 1;
        for (int nRow = nCellY0; nRow < nCellYEnd; ++nRow) {
            for (int nCol = nCellX0; nCol < nCellXEnd; ++nCol) {
                const unsigned int nClass = m_abBlockedMask[nRow * 128 + nCol];
                if (nClass == 0) {
                    continue;
                }
                const float fDeltaX = (particle.m_fX - m_fMinX) - static_cast<float>(nCol);
                const float fDeltaY = (particle.m_fY - m_fMinY) - static_cast<float>(nRow);
                const float fDistance = sqrtf(fDeltaY * fDeltaY + fDeltaX * fDeltaX);
                const float fMagnitude = forceMagnitude(m_aForceProfiles[nClass], fDistance);
                particle.m_fForceX = (fDeltaX / fDistance) * fMagnitude + particle.m_fForceX;
                particle.m_fForceY = (fDeltaY / fDistance) * fMagnitude + particle.m_fForceY;
            }
        }
    }

    // Integrate the accumulated forces and rebuild the hash; the newest particle heads its bin.
    std::memset(m_anHashHeads, 0xff, sizeof(m_anHashHeads));
    for (unsigned int nIndex = 0; nIndex != nCount; ++nIndex) {
        if (nIndex >= m_Particles.m_nParticleCount) {
            continue;
        }
        PlacementParticle& particle = m_Particles.m_pParticles[nIndex];
        if (particle.m_bFree) {
            continue;
        }
        particle.m_fX = particle.m_fForceX + particle.m_fX;
        particle.m_fY = particle.m_fForceY + particle.m_fY;
        particle.m_fForceX = 0.0f;
        particle.m_fForceY = 0.0f;
        int nBinX = static_cast<int>((particle.m_fX - m_fMinX) * 0.25f);
        nBinX = nBinX > 0x1f ? 0x1f : (nBinX < 0 ? 0 : nBinX);
        int nBinY = static_cast<int>((particle.m_fY - m_fMinY) * 0.25f);
        nBinY = nBinY > 0x1f ? 0x1f : (nBinY < 0 ? 0 : nBinY);
        particle.m_nHashNext = m_anHashHeads[nBinY][nBinX];
        m_anHashHeads[nBinY][nBinX] = static_cast<unsigned short>(nIndex);
    }
}

/// 0x08a036f0
UNK_RET cvCityBuildingLayoutGen::SolveForFinalPlacements()
{
    // Tick flags: bit0 prunes close pairs, bit1 kills particles on blocked cells; each is off on every 5th / 10th tick
    for (unsigned int nTick = 0; nTick != 40; ++nTick) {
        unsigned char nFlags = 0;
        if (nTick % 5 != 0) {
            nFlags |= 1;
        }
        if (nTick % 10 != 0) {
            nFlags |= 2;
        }
        Tick(nFlags);
    }
}

/// 0x08a040c6
UNK_RET cvCityBuildingLayoutGen::FinalizePlacementSet()
{
    const float kTensorScale = 1.0f / 128.0f;

    // Free list insert: a lower index becomes the head, otherwise it follows the head.
    auto freeParticle = [this](unsigned int nKill) {
        PlacementParticle* pParticles = m_Particles.m_pParticles;
        pParticles[nKill].m_bFree = 1;
        if (nKill < m_Particles.m_nFreeHead) {
            pParticles[nKill].m_nNextFree = m_Particles.m_nFreeHead;
            m_Particles.m_nFreeHead = nKill;
        } else {
            pParticles[nKill].m_nNextFree = pParticles[m_Particles.m_nFreeHead].m_nNextFree;
            pParticles[m_Particles.m_nFreeHead].m_nNextFree = nKill;
        }
        --m_Particles.m_nLiveCount;
    };

    // Each channel: two truncations to a signed byte, then the vertical weighting.
    auto interpolateChannel = [](signed char c00, signed char c01, signed char c10, signed char c11,
                                 float fx, float fy) -> signed char {
        float fTop = static_cast<float>(static_cast<signed char>(static_cast<int>(c00 * (1.0f - fx) + c01 * fx)));
        float fBottom = static_cast<float>(static_cast<signed char>(static_cast<int>(c10 * (1.0f - fx) + c11 * fx)));
        return static_cast<signed char>(static_cast<int>(fTop * (1.0f - fy) + fBottom * fy));
    };

    for (unsigned int nIndex = 0; nIndex < m_Particles.m_nParticleCount; ++nIndex) {
        PlacementParticle& particle = m_Particles.m_pParticles[nIndex];
        if (particle.m_bFree) {
            continue;
        }
        float x = particle.m_fX;
        float y = particle.m_fY;
        bool bKeep = m_fMinX <= x && x < m_fMaxX && m_fMinY <= y && y < m_fMaxY;
        if (bKeep) {
            while (m_fWrapMin > x) {
                x += m_fWrapWidth;
            }
            while (x > m_fWrapMax) {
                x -= m_fWrapWidth;
            }
            // The frame sits right after m_Data inside the field object.
            const FieldFrame& frame = *reinterpret_cast<const FieldFrame*>(
                reinterpret_cast<const unsigned char*>(m_pTensorField) + sizeof(m_pTensorField->m_Data));
            bKeep = frame.m_fMinX <= x && x < frame.m_fMaxX && frame.m_fMinY <= y && y < frame.m_fMaxY;
        }
        if (!bKeep) {
            freeParticle(nIndex);
            continue;
        }

        const FieldFrame& frame = *reinterpret_cast<const FieldFrame*>(
            reinterpret_cast<const unsigned char*>(m_pTensorField) + sizeof(m_pTensorField->m_Data));
        const int nRow = static_cast<int>((y - frame.m_fMinY) * frame.m_fScaleY);
        const int nCol = static_cast<int>((x - frame.m_fMinX) * frame.m_fScaleX);
        const float fx = x - std::floor(x);
        const float fy = y - std::floor(y);
        const FTensor::TensorC* cells = &m_pTensorField->m_Data.m_aCells[0][0];
        const int nStride = frame.m_nWidth;
        const FTensor::TensorC& c00 = cells[nRow * nStride + nCol];
        const FTensor::TensorC& c01 = cells[nRow * nStride + nCol + 1];
        const FTensor::TensorC& c10 = cells[(nRow + 1) * nStride + nCol];
        const FTensor::TensorC& c11 = cells[(nRow + 1) * nStride + nCol + 1];
        const signed char nDirX = interpolateChannel(c00.m_cX, c01.m_cX, c10.m_cX, c11.m_cX, fx, fy);
        const signed char nDirY = interpolateChannel(c00.m_cY, c01.m_cY, c10.m_cY, c11.m_cY, fx, fy);
        particle.m_fTensorX = nDirX * kTensorScale;
        particle.m_fTensorY = nDirY * kTensorScale;
    }
    m_pTensorField = nullptr;
}
