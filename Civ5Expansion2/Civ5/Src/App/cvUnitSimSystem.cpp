/// origin guess
#include "Civ5/Src/App/cvUnitSimSystem.h"
#include "Civ5/Src/App/EventTemplate.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/cvUnitFormationLib.h"
#include <string.h>

/// 0x08802b8a
UNK_RET cvUnitSimSystem::OnUnitSetDamage(GameCoreEventTemplate<Event_Int2Type<72>, UnitSetDamageData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1) {
    if (m_ucFlags & 1) {
        struct {
            cvUnitSimSystem* m_pSystem;
            UNK_RET (cvUnitSimSystem::*m_pHandler)(GameCoreEventTemplate<Event_Int2Type<72>, UnitSetDamageData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>&);
        } deferred = { this, &cvUnitSimSystem::OnUnitSetDamage };
        Fun_088086cc(unk_114, &a1, &deferred, 0);
        return;
    }
    UnitSetDamageData& data = reinterpret_cast<UnitSetDamageData&>(a1);
    cvUnitSimHandle handle = GetUnitHandle(data.m_ePlayer, data.m_iUnitID);
    if (handle.m_uiIndex < m_uiNumSims && m_pSims[handle.m_uiIndex].m_iLiveTag < 0) {
        cvUnitSim_SetHitPct hit;
        hit.m_iPct = data.m_iDamage * 100 / data.m_iMaxHitPoints;
        UpdateFromHandle(handle, hit);
    }
}

/// 0x08804b2c
UNK_RET cvUnitSimSystem::UpdateFromHandle(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> a1, cvUnitSim_SetHitPct const& a2) {
    if (a1.m_uiIndex >= m_uiNumSims) {
        return;
    }
    cvUnitSim& sim = m_pSims[a1.m_uiIndex];
    if (sim.m_iLiveTag >= 0) {
        return;
    }
    float blendTime = 0.5f;
    Fun_08808a8c(sim.unk_2d8, &blendTime, 0);

    int numFigures = sim.m_iNumFigures;
    cvUnitVis_DamageState state;
    state.m_ucNumFigures = (unsigned char)numFigures;
    state.m_uiReviveMaskHi = 0;
    state.m_uiReviveMaskLo = 0;
    memset(state.m_aucFigureState, 0, sizeof(state.m_aucFigureState));

    int pct = a2.m_iPct;
    int targetDead = (pct * numFigures + 50) / 100;
    if (!(pct == 100 || targetDead < numFigures)) {
        targetDead = numFigures - 1;
    }
    int numDead = 0;
    for (int i = 0; i < numFigures; ++i) {
        numDead += (sim.m_pFigures[i].m_uiHealthPct == 0);
    }
    int resultDead = numDead;
    if (targetDead < numDead) {
        for (int i = 0; i < numFigures; ++i) {
            cvUnitSimFigure& figure = sim.m_pFigures[i];
            if (figure.m_uiHealthPct == 0) {
                figure.m_uiHealthPct = 100;
                figure.m_uiFigureTag = sim.m_uiFigureTag;
                --numDead;
                if (i & 0x20) {
                    state.m_uiReviveMaskHi |= 1u << (i & 0x1f);
                } else {
                    state.m_uiReviveMaskLo |= 1u << (i & 0x1f);
                }
                state.m_afReviveTag[i] = (float)sim.m_uiFigureTag;
            }
            resultDead = targetDead;
            if (numDead == targetDead) {
                break;
            }
            resultDead = numDead;
        }
    } else if (numDead < targetDead) {
        for (int i = 0; i < numFigures; ++i) {
            cvUnitSimFigure& figure = sim.m_pFigures[numFigures - 1 - i];
            if (figure.m_uiHealthPct != 0) {
                figure.m_uiHealthPct = 0;
                ++numDead;
            }
            resultDead = targetDead;
            if (numDead == targetDead) {
                break;
            }
            resultDead = numDead;
        }
    }
    for (int i = 0; i < numFigures; ++i) {
        cvUnitSimFigure& figure = sim.m_pFigures[i];
        unsigned int healthPct = 0;
        if (figure.m_uiHealthPct != 0) {
            figure.m_uiHealthPct = 100u - pct;
            healthPct = figure.m_uiHealthPct;
        }
        unsigned char figureState = 1;
        if (healthPct <= 100 && sim.m_uiDamageStep <= 100) {
            if (healthPct == 0) {
                figureState = 0;
            } else {
                figureState = (unsigned char)(1 << (((100 - healthPct) / sim.m_uiDamageStep) & 0x1f));
            }
        }
        state.m_aucFigureState[i] = figureState;
    }
    Fun_08808f6c(sim.unk_2d8, &state, 0);

    cvUnitSim_ResetFormation reset;
    reset.m_uiFormationCrc = 0;
    reset.m_uiFormationCrc2 = 0;
    reset.m_bSetHeading = (resultDead != targetDead);
    cvUnitSimHandle handle;
    handle.field_0 = a1.field_0;
    handle.m_uiIndex = a1.m_uiIndex;
    UpdateFromHandle(handle, reset);
}

/// 0x088059b6
UNK_RET cvUnitSimSystem::UpdateFromHandle(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> a1, cvUnitSim_ResetFormation const& a2) {
    if (a1.m_uiIndex >= m_uiNumSims) {
        return;
    }
    cvUnitSim& sim = m_pSims[a1.m_uiIndex];
    if (sim.m_iLiveTag >= 0) {
        return;
    }
    int numFigures = sim.m_iNumFigures;
    if (a2.m_uiFormationCrc != 0) {
        sim.m_uiFormationCrc = a2.m_uiFormationCrc;
    }
    if (a2.m_uiFormationCrc2 != 0) {
        sim.m_uiFormationSeed = a2.m_uiFormationCrc2;
    }
    if (numFigures > 0) {
        int numAlive = 0;
        for (int i = 0; i < numFigures; ++i) {
            numAlive += (sim.m_pFigures[i].m_uiHealthPct != 0);
        }
        if (numAlive != 0) {
            if (sim.m_uiFormationCrc == 0) {
                sim.m_uiFormationCrc = g_UnitFormationLib.CalcFormationCrc("Default");
            }
            cvUnitSim_SetTransformation transform;
            transform.m_vPos = sim.m_vPos;
            transform.m_fHeading = sim.m_fHeading;
            transform.m_uiFigure = 0xffffffff;
            transform.m_bSetHeading = a2.m_bSetHeading;
            cvUnitSimHandle handle;
            handle.field_0 = a1.field_0;
            handle.m_uiIndex = a1.m_uiIndex;
            UpdateFromHandle(handle, transform);
        }
    }
    PublishUnitActionStates(sim);
}

/// 0x08806394
UNK_RET cvUnitSimSystem::UpdateFromHandle(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> a1, cvUnitSim_DamageState const& a2) {
    if (a1.m_uiIndex >= m_uiNumSims) {
        return;
    }
    cvUnitSim& sim = m_pSims[a1.m_uiIndex];
    if (sim.m_iLiveTag >= 0) {
        return;
    }
    float blendTime = 0.5f;
    Fun_08808a8c(sim.unk_2d8, &blendTime, 0);

    int numFigures = sim.m_iNumFigures;
    cvUnitVis_DamageState state;
    state.m_ucNumFigures = (unsigned char)numFigures;
    state.m_uiReviveMaskHi = 0;
    state.m_uiReviveMaskLo = 0;
    memset(state.m_aucFigureState, 0, sizeof(state.m_aucFigureState));

    unsigned int figureIndex = a2.m_uiFigure;
    if (figureIndex == 0xffffffff) {
        return;
    }
    cvUnitSimFigure& target = sim.m_pFigures[figureIndex];
    if ((a2.m_uiHealthPct == 0) != (target.m_uiHealthPct == 0)) {
        unsigned int bit = (a2.m_uiHealthPct != 0) ? (1u << (figureIndex & 0x1f)) : 0;
        if (figureIndex & 0x20) {
            state.m_uiReviveMaskHi = bit;
        } else {
            state.m_uiReviveMaskLo = bit;
        }
        state.m_afReviveTag[figureIndex] = 0;
    }
    target.m_uiHealthPct = a2.m_uiHealthPct;
    for (int i = 0; i < numFigures; ++i) {
        unsigned int healthPct = sim.m_pFigures[i].m_uiHealthPct;
        unsigned char figureState = 1;
        if (healthPct <= 100 && sim.m_uiDamageStep <= 100) {
            if (healthPct == 0) {
                figureState = 0;
            } else {
                figureState = (unsigned char)(1 << (((100 - healthPct) / sim.m_uiDamageStep) & 0x1f));
            }
        }
        state.m_aucFigureState[i] = figureState;
    }
    Fun_08808f6c(sim.unk_2d8, &state, 0);
}
