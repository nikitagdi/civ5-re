/// origin guess
#pragma once

/// size ?
/// vtable 0x0a301e30
class FResourceHotLoader {
public:
    /// 0x08cdd3fc
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08cdd42c
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08cdcc10
    virtual UNK_RET IsAssetTypeRecognized(char const* a1);
    /// 0x08cdcdfc
    virtual UNK_RET LoadAsset(char const* a1);
    /// 0x08cdd08e
    virtual UNK_RET LoadAudioFile(char const* a1);
    /// 0x08cdd236
    virtual UNK_RET LoadTexture(char const* a1);
    /// 0x08cdd094
    virtual UNK_RET LoadXMLFile(char const* a1);
    /// 0x08cdcdf6
    virtual UNK_RET LoadCSVFile(char const* a1);
    /// 0x08cdcc40
    UNK_RET IsAssetExtensionInList(char const** a1, char const* a2);
    /// 0x08cdd23a
    UNK_RET RegisterXMLCallbackFunction(char const* a1, bool (*a2)(char const*));
};
