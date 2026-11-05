/// origin binary
#pragma once

/// size ?
/// vtable 0x0a305768
class FDateTime {
public:
    /// 0x08d7da4c
    FDateTime();
    /// 0x08d7daba
    /// deleting 0x08d7daa6
    virtual ~FDateTime();
    /// 0x08d7da84
    UNK_RET Reset();
    /// 0x08d7dabc
    UNK_RET GetFSystemTime();
    /// 0x08d7db4c
    UNK_RET GetFLocalTime();
    /// 0x08d7dbdc
    UNK_RET GetPastDateTime(unsigned short a1, unsigned short a2);
    /// 0x08d7dd20
    UNK_RET GetDaysInMonth() const;
    /// 0x08d7ddac
    UNK_RET GenerateDailySeed() const;
    /// 0x08d7ddc8
    UNK_RET GetDaysInMonth(unsigned int a1) const;
    /// 0x08d7de52
    UNK_RET GetLeapYear() const;
    /// 0x08d7dea4
    UNK_RET GetElapsedDays() const;
    /// 0x08d7e012
    UNK_RET SetDayOfWeek();
    /// 0x08d7e14c
    UNK_RET GetDateString() const;
    /// 0x08d7e35a
    UNK_RET GetDayOfWeekString() const;
    /// 0x08d7e4e6
    UNK_RET GetMonthString() const;
    /// 0x08d7e736
    UNK_RET GetCompactDateString() const;
    /// 0x08d7e7d0
    UNK_RET GetTimeString() const;
};
