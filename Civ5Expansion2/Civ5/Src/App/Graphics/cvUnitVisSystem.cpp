/// origin binary
#include <new>
#include <string.h>
#include "Civ5/Src/App/Graphics/cvUnitVisSystem.h"
#include "Civ5/Src/App/Graphics/UnitGlobalData.h"
#include "Civ5/Src/App/Graphics/cvUnitLibrary.h"

static const unsigned int kVisSlotSize = 0x380;
static const int kJobSetOffsetInUnitC90 = 0x6e10;

/// 0x0893170c
cvUnitVisSystem::cvUnitVisSystem() {
    field_0 = 8;
    Fun_08935b6a(unk_4);
    field_e8 = 0xfffffff;
    field_ec = 0;
    memset(unk_f0, 0, sizeof(unk_f0));
    Fun_08935bf6(unk_fc, 1, &field_e8);
    new (unk_10c) UnitGlobalData();
    memset(&m_kFrameGlobals.m_avWorldTopLeftVisible, 0, 0x140);
    field_b6c = 0;
    field_bf0.field_0 = 0xfffffff;
    field_bf0.field_4 = 0;
    field_bf0.field_8 = 0;
    field_bf0.field_c = 0;
    field_bf0.field_10 = 0;
    Fun_08935c96(unk_c04, 1, &field_bf0);
    new (unk_c20) cvUnitLibrary();
    Fun_08935d36(unk_c90);
    m_fTime = 0.0f;
    m_fDeltaTime = 0.0f;
    field_13ab4 = 10.116732f;
    m_eAnimationSamplingType = static_cast<AnimationSamplingChangedData::AnimationSampleTypes>(0);
    m_bDebugInfoDisplay = false;
    field_13ac0 = 0;
    // Six TextControl objects (stride 0x178) are constructed at unk_13ac4 here; TextControl is not declared yet.
    field_14394 = 100;
    field_143a8 = 0xfffffff;
    field_143ac = 0;
    field_143bc = 0;
    field_143c0 = 0xfffffff;
    field_143c4 = 0xfffffff;
}

/// 0x08931b0c
cvUnitVisSystem::~cvUnitVisSystem() {
    field_143a8 = 0xfffffff;
    field_143ac = 0;
    Fun_0893bbc8(unk_143b0);
    // The six TextControl objects at unk_13ac4 are destroyed here, last to first.
    reinterpret_cast<FJobSet*>(unk_c90 + kJobSetOffsetInUnitC90)->~FJobSet();
    reinterpret_cast<cvUnitLibrary*>(unk_c20)->~cvUnitLibrary();
    Fun_0893b7ac(unk_c04);
    Fun_0893b7d8(&field_bf0.field_8);
    Fun_0893b80e(unk_fc);
    Fun_0893b83a(unk_f0);
    Fun_08935dfa(unk_4);
}

/// 0x08932892
void cvUnitVisSystem::ShutdownGame() {
    Fun_0893b5ca(unk_4);
    Fun_0893b636(unk_4 + 0x10);
    Fun_0893b5ca(unk_4 + 0xd4);
    DestroyVises();
}

/// 0x08932bf6
void cvUnitVisSystem::OnToggleDisplayUnits(NonSerialEvent<13, NullEventClass>& a1) {
    m_bDisplayUnits = !m_bDisplayUnits;
}

/// 0x08932ef2
void cvUnitVisSystem::OnToggleDebugInfoDisplay(NonSerialEvent<19, NullEventClass>& a1) {
    m_bDebugInfoDisplay = !m_bDebugInfoDisplay;
    SetUnitCount(field_14394, field_14398);
}

/// 0x089326c4
void cvUnitVisSystem::SetAnimationSamplingType(AnimationSamplingChangedData::AnimationSampleTypes eType) {
    m_eAnimationSamplingType = eType;
    UpdateAnimationText();
}

// Percent, 100 from Startup, steps of 5 in [10, 500]; scales only the root node matrix of each unit model.
/// 0x08933ac2
float cvUnitVisSystem::GetGlobalUnitScale() const {
    return static_cast<float>(m_uiGlobalUnitScale) / 100.0f;
}

/// 0x08933fe2
void cvUnitVisSystem::Update(float fTime) {
    m_fDeltaTime = fTime - m_fTime;
    m_fTime = fTime;
    Fun_0893b5ca(unk_4);
    Fun_0893b636(unk_4 + 0x10);
    Fun_0893b5ca(unk_4 + 0xd4);
}

/// 0x08934e9a
void cvUnitVisSystem::InitHandle(cvUnitVisHandle& h, const cvUnitVis_Init& init) {
    const unsigned int index = h.m_uiIndex;
    const VisSlotArray* pSlots = field_c10;
    if (index < pSlots->field_c && *reinterpret_cast<const int*>(pSlots->field_8 + index * kVisSlotSize + 4) < 0)
        return;

    int emptyEntry[2] = {0, 0};
    char* pEntries = *reinterpret_cast<char**>(unk_fc);
    unsigned int newIndex = Fun_08937312(*reinterpret_cast<void**>(pEntries), emptyEntry);
    Fun_0893737e(pEntries, newIndex);
    h.m_uiIndex = newIndex;

    cvUnitVisHandle key = h;
    struct {
        cvUnitVisSystem* pObject;
        void (cvUnitVisSystem::*pCallback)(cvUnitVisHandle, const cvUnitVis_Init&);
    } onCreate = {this, &cvUnitVisSystem::UpdateFromHandle};
    Fun_08936be2(unk_4, &key, &init, &onCreate, 0, 0);
}
