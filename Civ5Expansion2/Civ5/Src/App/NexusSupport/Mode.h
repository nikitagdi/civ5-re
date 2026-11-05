/// origin guess
#pragma once

namespace Nexus {
class DataInStream;
class DataOutStream;
}

namespace Nexus {
/// size ?
/// vtable 0x0a300630
class Mode {
public:
    /// 0x08df6432
    Mode();
    /// 0x08b5880e
    /// deleting 0x08b5883e
    virtual ~Mode();
    /// pure
    virtual UNK_RET Startup() = 0;
    /// pure
    virtual UNK_RET Shutdown() = 0;
    /// pure
    virtual UNK_RET Update() = 0;
    /// 0x08df6470
    UNK_RET Call(Nexus::DataInStream& a1);
    /// 0x08df64ec
    UNK_RET GetMessageTypes(Nexus::DataOutStream& a1);
};
}
