/// origin guess
#pragma once

class FTextFile;
namespace cvAutomation {
class DataOrientation;
class TableType;
}

/// size ?
class cvAutomationFPS {
public:
    /// 0x0898f964
    cvAutomationFPS();
    /// 0x0898f9d8
    UNK_RET Start();
    /// 0x0898fa08
    UNK_RET WriteTable(char const* a1, unsigned int a2, cvAutomation::TableType a3, cvAutomation::DataOrientation a4);
    /// 0x0898fbf8
    UNK_RET WriteColumnTable(FTextFile& a1, unsigned int a2, cvAutomation::TableType a3);
    /// 0x0899052e
    UNK_RET WriteRowTable(FTextFile& a1, unsigned int a2, cvAutomation::TableType a3);
    /// 0x08990a34
    UNK_RET Update();
};
