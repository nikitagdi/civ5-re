/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFastVector.h"
#include <granny.h>

class FGlobalDefineMapperXML;
class FXmlDocument;
class FXmlElement;
class PredicateFXmlElemWithAttr;
template <class T0, class T1> class FXMLAccumulator;
template <class T0, class T1> class FXMLExecuteIf;

/// size ?
class FGlobalDefines {
public:
    class FGlobalDefineValue;
    /// 0x08cc038c
    FGlobalDefines();
    /// 0x08cc03e8
    UNK_RET ReadValues(char const* a1);
    /// 0x08cc0490
    UNK_RET GetDefine(char const* a1);
    /// 0x08cc0520
    UNK_RET GetDefine(char const* a1, FGlobalDefines::FGlobalDefineValue* a2);
    /// 0x08cc0580
    UNK_RET SetDefine(char const* a1, FGlobalDefines::FGlobalDefineValue const& a2);
    /// 0x08cc05d4
    UNK_RET GetStringPalette();
};

/// 0x08cc05dc
UNK_RET GLOBAL_INT(char const* a1);
/// 0x08cc063f
UNK_RET GLOBAL_INT(char const* a1, int a2);
/// 0x08cc06bc
UNK_RET GLOBAL_UINT(char const* a1);
/// 0x08cc071f
UNK_RET GLOBAL_UINT(char const* a1, unsigned int a2);
/// 0x08cc079c
UNK_RET GLOBAL_STRING(char const* a1);
/// 0x08cc07ff
UNK_RET GLOBAL_FLOAT(char const* a1);
/// 0x08cc0862
UNK_RET GLOBAL_FLOAT(char const* a1, float a2);
/// 0x08cc0a15 ForEachFXmlNode<FXMLExecuteIf<PredicateFXmlElemWithAttr, FGlobalDefineMapperXML> >
template <class T0>
void ForEachFXmlNode(FXmlDocument& a1, T0& a2);
/// 0x08cd4f1b ForEachFXmlNode<FXMLAccumulator<PredicateFXmlElemWithAttr, FStaticVector<FXmlElement, 512u, false, 27u, 0u> > >
/// 0x08cc0bd0 ForEachFXmlNode<FXMLExecuteIf<PredicateFXmlElemWithAttr, FGlobalDefineMapperXML> >
template <class T0>
void ForEachFXmlNode(FXmlElement& a1, T0& a2);
/// 0x08cc121c
UNK_RET GrannyMakeIdentity(float (*a1)[4][4]);
/// 0x08cc12a4
UNK_RET FGrannyCloseFileReader(granny_file_reader* a1);
/// 0x08cc130c
UNK_RET FGrannyReadAtMost(granny_file_reader* a1, int a2, int a3, void* a4);
/// 0x08cc135e
UNK_RET FGrannyGetReaderSize(granny_file_reader* a1, int* a2);
/// 0x08cc1397
UNK_RET FGrannyOpenFileReader(char const* a1);
/// 0x08cc14ad
UNK_RET GrannyError(granny_log_message_type a1, granny_log_message_origin a2, char const* a3, int a4, char const* a5, void* a6);
