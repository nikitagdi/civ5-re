/// origin guess
#pragma once

class FGXTransparencyMesh;
namespace FireGrafix {
class FGXModelNode;
enum FGX_SPIV_GENERIC : int;
}

/// size ?
class FGXTransparencyScene {
public:
    /// 0x0889e6c4
    UNK_RET AddMeshInstance(FGXTransparencyMesh const* a1, FireGrafix::FGXModelNode const* a2, void const* a3, FireGrafix::FGX_SPIV_GENERIC const* a4, unsigned int const* a5);
    /// 0x0889e744
    UNK_RET FinalizeInstances();
    /// 0x0889e790
    UNK_RET ClearInstances();
};
