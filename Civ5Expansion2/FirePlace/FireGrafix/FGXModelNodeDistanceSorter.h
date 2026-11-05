/// origin guess
#pragma once

namespace FireGrafix {
class FGXModelInstance;
class FGXModelNode;
class FGXModelTemplate;
}

namespace FireGrafix {
/// size ?
class FGXModelNodeDistanceSorter {
public:
    /// 0x08cf47d8
    UNK_RET operator()(FireGrafix::FGXModelNode const& a1, FireGrafix::FGXModelNode const& a2) const;
    /// 0x08cf4886
    UNK_RET SortInstance(FireGrafix::FGXModelInstance& a1, unsigned int a2);
    /// 0x08cf4938
    UNK_RET SortTemplate(FireGrafix::FGXModelTemplate& a1);
};
}
