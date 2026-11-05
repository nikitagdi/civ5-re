/// origin guess
#pragma once

class ShaderPackageInstanceCreator;
namespace FireGrafix {
class FGXModelInstance;
class FGXModelTemplate;
}

namespace FireGrafix {
/// size ?
template <class T0>
class FGXModelInstanceBuilder {
public:
    /// 0x088676f0 FireGrafix::FGXModelInstanceBuilder<ShaderPackageInstanceCreator>
    UNK_RET InitializeFGXModelInstance(FireGrafix::FGXModelTemplate const& a1, FireGrafix::FGXModelInstance& a2, unsigned long a3, unsigned char* a4);
    /// 0x08867be2 FireGrafix::FGXModelInstanceBuilder<ShaderPackageInstanceCreator>
    UNK_RET CalcBlockAllocationSizeAndOffsets(FireGrafix::FGXModelTemplate const& a1, unsigned long a2);
};
}
