/// origin guess
#pragma once

/// size ?
class FNetLinkFactoryENet {
public:
    /// 0x08d572f4
    FNetLinkFactoryENet();
    /// 0x08d5730c
    ~FNetLinkFactoryENet();
    /// 0x08d573ae
    UNK_RET CreateLink(long a1);
    /// 0x08d5752a
    UNK_RET GetLink(long a1) const;
    /// 0x08d57578
    UNK_RET ReleaseLink(long a1);
};
