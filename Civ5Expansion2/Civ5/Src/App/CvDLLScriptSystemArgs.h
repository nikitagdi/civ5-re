/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/ICvDLLScriptSystem.h"

/// size ?
/// vtable 0x0a2ffdc0
class CvDLLScriptSystemArgs : public ICvEngineScriptSystemArgs1 {
public:
    /// 0x08a027ba
    CvDLLScriptSystemArgs(unsigned int a1);
    /// 0x08a02740
    /// deleting 0x08a02718
    virtual ~CvDLLScriptSystemArgs();
    /// 0x08a0299c
    virtual unsigned int Count();
    /// 0x08a02a06
    virtual bool GetBool(unsigned int a1, bool& a2);
    /// 0x08a02a68
    virtual bool GetFloat(unsigned int a1, float& a2);
    /// 0x08a02a9a
    virtual bool GetInt(unsigned int a1, int& a2);
    /// 0x08a02a38
    virtual bool GetString(unsigned int a1, char*& a2);
    /// 0x08a029a4
    virtual ICvEngineScriptSystemArgs1::ArgType GetType(unsigned int a1);
    /// 0x08a0281c
    virtual unsigned int Push(bool a1);
    /// 0x08a02860
    virtual unsigned int Push(char const* a1, unsigned int a2);
    /// 0x08a028da
    virtual unsigned int Push(int a1);
    /// 0x08a0291c
    virtual unsigned int Push(float a1);
    /// 0x08a02962
    virtual unsigned int PushNULL();
};
