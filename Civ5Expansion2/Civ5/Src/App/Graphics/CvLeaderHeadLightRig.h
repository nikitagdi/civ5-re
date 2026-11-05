/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFastVector.h"
#include <granny.h>

class FXmlTraverser;

/// size ?
class CvLeaderHeadLightRig {
public:
    class DirectionalLight;
    class LightList;
    class PointLight;
    class SpotLight;
    /// 0x08a30824
    CvLeaderHeadLightRig();
    /// 0x08a3093e
    UNK_RET EnforceLightLimit(unsigned int a1, unsigned int a2, unsigned int a3);
    /// 0x08a30aa6
    UNK_RET Clear();
    /// 0x08a30afa
    UNK_RET GetLights(CvLeaderHeadLightRig::LightList* a1, bool a2, bool a3) const;
    /// 0x08a30b9a
    UNK_RET Load(char const* a1);
    /// 0x08a30c7a
    UNK_RET InitFromFile(char const* a1);
    /// 0x08a3148a
    UNK_RET Update(float a1);
    /// 0x08a316c0
    UNK_RET ComputeLightTints();
    /// 0x08a3198c
    UNK_RET SetFSMPathAction(float a1, bool a2);
    /// 0x08a319ec
    UNK_RET ResetFSM();
    /// 0x08a31a42
    UNK_RET ParseLineLightSettings(FXmlTraverser& a1);
    /// 0x08a31e64
    UNK_RET ParseHackSpecSettings(FXmlTraverser& a1);
    /// 0x08a324a0
    UNK_RET FindExistingLight(granny_bone const& a1, unsigned int& a2) const;
    /// 0x08a325ac
    UNK_RET FindLightByID(unsigned int a1);
    /// 0x08a3266a
    UNK_RET AddLightToList(CvLeaderHeadLightRig::PointLight const& a1, unsigned int a2, CvLeaderHeadLightRig::LightList* a3) const;
    /// 0x08a3290e
    UNK_RET AddLightToList(CvLeaderHeadLightRig::DirectionalLight const& a1, unsigned int a2, CvLeaderHeadLightRig::LightList* a3) const;
    /// 0x08a32b9e
    UNK_RET AddLightToList(CvLeaderHeadLightRig::SpotLight const& a1, unsigned int a2, CvLeaderHeadLightRig::LightList* a3) const;
    /// 0x08a32ed4
    UNK_RET SampleVectorTracks(unsigned int a1, granny_animation const* a2, granny_sample_context const* a3);
    /// 0x08a331e2 CopyLights<FStaticVector<CvLeaderHeadLightRig::DirectionalLight, 4u, true, 46u, 0u> >
    /// 0x08a33154 CopyLights<FStaticVector<CvLeaderHeadLightRig::PointLight, 4u, true, 46u, 0u> >
    /// 0x08a3326a CopyLights<FStaticVector<CvLeaderHeadLightRig::SpotLight, 4u, true, 46u, 0u> >
    template <class T0>
    unsigned int CopyLights(bool a1, bool a2, CvLeaderHeadLightRig::LightList* a3, T0 const& a4) const;
};
