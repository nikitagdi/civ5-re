/// origin guess
#pragma once

namespace FireGrafix {
class FGXModelInstance;
class FGXModelNode;
class FGXModelTemplate;
}

namespace FireGrafix {
/// size ?
class FGXModelNodeStateSorter {
public:
    enum eDesiredStateFlags : int;
    /// 0x08cf4450
    FGXModelNodeStateSorter();
    /// 0x08cf44a2
    UNK_RET Clear();
    /// 0x08cf44de
    UNK_RET operator()(FireGrafix::FGXModelNode const& a1, FireGrafix::FGXModelNode const& a2) const;
    /// 0x08cf4532
    UNK_RET IsInCorrectState(FireGrafix::FGXModelNode const& a1) const;
    /// 0x08cf4554
    UNK_RET SortInstance(FireGrafix::FGXModelInstance& a1, unsigned int a2);
    /// 0x08cf4606
    UNK_RET FindLastValidNode(FireGrafix::FGXModelNode const* a1, unsigned int a2);
    /// 0x08cf466a
    UNK_RET SortTemplate(FireGrafix::FGXModelTemplate& a1);
    /// 0x08cf4762
    UNK_RET SetDesiredState(unsigned char a1);
    /// 0x08cf478a
    UNK_RET SetDesiredState(FireGrafix::FGXModelNodeStateSorter::eDesiredStateFlags const& a1, unsigned int a2);
    /// 0x08cf47a6
    UNK_RET SetDesiredState(unsigned char a1, unsigned int a2);
    /// 0x08cf47c4
    UNK_RET SetExternalTreeIndices(unsigned int** a1, unsigned int a2);
};
}
