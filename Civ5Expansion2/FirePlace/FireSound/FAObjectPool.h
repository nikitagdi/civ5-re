/// origin binary
#pragma once

class FSharedSoundData;
class FSound2D;
class FSound3D;

/// size ?
template <class T0>
class FAObjectPool {
public:
    /// 0x08d1f38a FAObjectPool<FSharedSoundData>
    /// 0x08d1fa5e FAObjectPool<FSound2D>
    /// 0x08d1f6ea FAObjectPool<FSound3D>
    UNK_RET CreateObject();
    /// 0x08d1f3d6 FAObjectPool<FSharedSoundData>
    /// 0x08d1faae FAObjectPool<FSound2D>
    /// 0x08d1f73a FAObjectPool<FSound3D>
    UNK_RET Reserve(unsigned int a1);
    /// 0x08d1ffae FAObjectPool<FSharedSoundData>
    /// 0x08d1ff46 FAObjectPool<FSound2D>
    /// 0x08d1fede FAObjectPool<FSound3D>
    UNK_RET DestroyObject(T0* a1);
    /// 0x08d20172 FAObjectPool<FSharedSoundData>
    /// 0x08d20264 FAObjectPool<FSound2D>
    /// 0x08d201f0 FAObjectPool<FSound3D>
    UNK_RET Term();
};
