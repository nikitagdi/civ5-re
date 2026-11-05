/// origin guess
#pragma once
#include "FirePlace/FireWire/FHttpRequest.inl"
#include "FirePlace/FireWorks/FIMemTracker.h"
#include <lua.h>

class cvLuaHttpRequest;
namespace cvLuaStdLibLibrary {
class StdLibs;
}

/// size ?
/// vtable 0x0a300524
class cvLuaHttpRequest : public FHttpRequest<cvLuaHttpRequest> {
public:
    /// 0x08b51c10
    cvLuaHttpRequest();
    /// 0x08b52850
    /// deleting 0x08b52888
    virtual ~cvLuaHttpRequest();
    /// 0x08b51ce0
    static int Create(lua_State* a1);
    /// 0x08b52372
    /// lua ?
    static int lDestroy(lua_State* a1);
    /// 0x08b523a4
    /// lua ?
    static int lPopReceivedData(lua_State* a1);
    /// 0x08b52466
    /// lua ?
    static int lFinished(lua_State* a1);
    /// 0x08b524b2
    UNK_RET Finished() const;
    /// 0x08b524be
    UNK_RET OnHeadersAvailable();
    /// 0x08b524c0
    UNK_RET OnReadComplete(char* a1, unsigned int a2);
    /// 0x08b525b4
    UNK_RET OnResponseComplete(long a1);
};

namespace CvLuaOnlyEvent {
/// 0x08b5445c
/// lua ?
int lCreate(lua_State* a1);
/// 0x08b5452a
UNK_RET RegisterMethods(lua_State* a1, int a2, int a3, int a4);
/// 0x08b5460a
int RegisterMetatable(lua_State* a1);
/// 0x08b54708
/// lua ?
int lAdd(lua_State* a1);
/// 0x08b5475e
/// lua ?
int lCount(lua_State* a1);
/// 0x08b5479a
/// lua ?
int lDispatch(lua_State* a1);
/// 0x08b54880
/// lua ?
int lRemove(lua_State* a1);
/// 0x08b54976
/// lua ?
int lRemoveAll(lua_State* a1);
/// 0x08b549d0
/// lua ?
int lMetaCall(lua_State* a1);
/// 0x08b54a2e
UNK_RET RegisterMethod(lua_State* a1, int (*a2)(lua_State*), char const* a3, int a4, int a5);
}

namespace cvLuaMemTrackerLibrary {
/// 0x08b5334c
UNK_RET Register(lua_State* a1, lua_State* a2);
/// 0x08b5338a
int pRegister(lua_State* a1);
/// 0x08b534fa
/// lua ?
int lIsEnabled(lua_State* a1);
/// 0x08b53544
/// lua ?
int lEnable(lua_State* a1);
/// 0x08b53582
/// lua ?
int lIncrementMarker(lua_State* a1);
/// 0x08b535da
/// lua ?
int lDumpAllocations(lua_State* a1);
/// 0x08b5363e
/// lua ?
int lReport(lua_State* a1);
/// 0x08b5398e
/// lua ?
int lDumpLuaAllocations(lua_State* a1);
/// 0x08b539d2
UNK_RET DumpLuaAllocations(char const* a1);
/// 0x08b53d44
UNK_RET RecordLuaAllocation(unsigned int a1, int a2, int a3, int a4, FIMemTracker::EMemType a5);
/// 0x08b53f04
UNK_RET RecordLuaDeallocation(unsigned int a1);
}

namespace cvLuaStdLibLibrary {
/// 0x08b54a98
UNK_RET Register(lua_State* a1, cvLuaStdLibLibrary::StdLibs a2);
/// 0x08b54afe
int pRegisterStdLibs(lua_State* a1);
/// 0x08b54bb8
int pRegisterSafeStdLibs(lua_State* a1);
/// 0x08b54e3e
int RefreshIncludeFileList(lua_State* a1);
/// 0x08b54e72
int pRefreshIncludeFileList(lua_State* a1);
/// 0x08b54ec2
int RegisterInclude(lua_State* a1);
/// 0x08b54fc2
int RegisterTraceback(lua_State* a1);
/// 0x08b5505e
UNK_RET CopyStdTable(lua_State* a1, int a2, char const* a3, char const** a4);
/// 0x08b550f2
/// lua ?
int lRefreshIncludeFileList(lua_State* a1);
/// 0x08b551ee
/// lua ?
int lInclude(lua_State* a1);
}
