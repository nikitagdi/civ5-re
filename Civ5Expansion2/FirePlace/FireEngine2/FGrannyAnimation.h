/// origin guess
#pragma once
#include <d3d9.h>
#include <granny.h>

class FFileManager;
class FGrannyAssetInstance;
class IDirect3DVertexDeclaration9;
namespace FGranny {
class TrackMaskReporter;
}
namespace FGrannyDAG2ScalarCallbacks {
enum ECallbackFunction : int;
}
namespace FireGrafix {
class FGXVertexElement;
enum _FGX_VERTEXFORMATS : int;
enum FGX_VERTEXELEMENTFORMATS : int;
enum FGX_VERTEXELEMENTTYPES : int;
}

/// size ?
class FGrannyAnimation {
public:
    /// 0x08cc15a8
    FGrannyAnimation(granny_animation* a1, char const* a2);
    /// 0x08cc160a
    UNK_RET SetName(char const* a1);
    /// 0x08cc1658
    UNK_RET SetTrackMaskName(char const* a1);
};

/// 0x08cc169f
UNK_RET GrannyAssertHandler(char const* a1, char const* a2, int a3, char const* a4, bool* a5, void* a6);
/// 0x08cc1fcf
UNK_RET GetGrannyInputNameFromFGXName(FireGrafix::FGX_VERTEXELEMENTTYPES a1);
/// 0x08cc1fef
UNK_RET GetGrannyInputTypeFromFGXFormat(FireGrafix::FGX_VERTEXELEMENTFORMATS a1);
/// 0x08cc2012
UNK_RET GetGrannyInputElementsFromFGXFormat(FireGrafix::FGX_VERTEXELEMENTFORMATS a1);
/// 0x08cc2962
UNK_RET GetGrannyInputNameFromD3DUsage(unsigned char a1, unsigned char a2);
/// 0x08cc29dc
UNK_RET GetGrannyInputTypeFromD3DType(unsigned long a1);
/// 0x08cc29ff
UNK_RET GetGrannyInputElementsFromD3DType(unsigned long a1);

namespace FGranny {
/// 0x08cc16a0
UNK_RET Init();
/// 0x08cc1738
UNK_RET FGrannyAllocate(char const* a1, int a2, int a3, int a4);
/// 0x08cc1774
UNK_RET FGrannyDeallocate(char const* a1, int a2, void* a3);
/// 0x08cc1796
UNK_RET Shutdown();
/// 0x08cc179c
UNK_RET HookAllocator(void* (*a1)(char const*, int, int, int), void (*a2)(char const*, int, void*));
/// 0x08cc17b0
UNK_RET UnhookAllocator();
/// 0x08cc17de
UNK_RET RemapFileStrings(granny_file* a1);
/// 0x08cc190e
UNK_RET ReadEntireFileFromMemoryMT(int a1, void* a2, bool a3, bool* a4, char const* a5);
/// 0x08cc1956
UNK_RET ReadEntireFile(char const* a1, bool a2, bool* a3);
/// 0x08cc19da
UNK_RET GetDagPoseCache();
/// 0x08cc1a1c
UNK_RET GetLocalPoseCache();
/// 0x08cc1a6a
UNK_RET GetWorldPoseCache();
/// 0x08cc1ab8
UNK_RET CreateBoneUsageMask(granny_model const* a1);
/// 0x08cc1cf2
UNK_RET IsBoneCamera(granny_bone const* a1);
/// 0x08cc1d48
UNK_RET IsBoneLight(granny_bone const* a1);
/// 0x08cc1d9e
UNK_RET IsBoneAncestor(granny_skeleton const* a1, int a2, int a3);
/// 0x08cc1dec
UNK_RET NextChildBone(granny_skeleton const* a1, int a2, int a3);
/// 0x08cc1e40
UNK_RET ExtractUniqueMaterialsFromModel(granny_model const* a1, granny_material const** a2, int a3, int a4);
/// 0x08cc1f08
UNK_RET GrannyCompareVersionTo(int a1, int a2, int a3, int a4);
/// 0x08cc1f9a
UNK_RET FileIsDAG2(granny_file const* a1);
/// 0x08cc1fbc
UNK_RET GetGrannyLock();
/// 0x08cc2032
UNK_RET GetGrannyInputFormatFromFGXFormat(FireGrafix::_FGX_VERTEXFORMATS a1);
/// 0x08cc20a8
UNK_RET GetGrannyInputFormatFromFGXElems(FireGrafix::FGXVertexElement const* a1, unsigned int a2, granny_data_type_definition* a3);
/// 0x08cc213a
UNK_RET StringsAreEqualCallback(char const* a1, char const* a2);
/// 0x08cc26c0
UNK_RET SetStringDatabase(granny_string_database* a1);
/// 0x08cc26fc
UNK_RET LoadStringDatabase(char const* a1);
/// 0x08cc2746
UNK_RET LoadStringDatabaseToMaster(char const* a1);
/// 0x08cc279c
UNK_RET LoadStringDatabaseToMaster(granny_string_database* a1);
/// 0x08cc27ee
UNK_RET LoadStringDatabases(FFileManager* a1);
/// 0x08cc2a20
UNK_RET GetGrannyInputFormatFromD3D9Decls(IDirect3DVertexDeclaration9* a1, granny_data_type_definition* a2);
/// 0x08cc2a96
UNK_RET GetGrannyInputFormatFromD3D9Elems(_D3DVERTEXELEMENT9* a1, unsigned int a2, granny_data_type_definition* a3);
/// 0x08cc2b96
UNK_RET ExtractTrackMaskList(granny_model const* a1, FGranny::TrackMaskReporter& a2);
/// 0x08cc2ea2
UNK_RET ValidateGrannyFileInfo(granny_file_info const* a1);
/// 0x08cc325e
UNK_RET CurveEndpointsAreEquivalent(granny_curve2 const& a1, float a2);
}

namespace FGrannyDAG2ScalarCallbacks {
/// 0x08cc215c
UNK_RET GetFunctionName(FGrannyDAG2ScalarCallbacks::ECallbackFunction a1);
/// 0x08cc217c
UNK_RET GetFunctionDescription(FGrannyDAG2ScalarCallbacks::ECallbackFunction a1);
/// 0x08cc219c
UNK_RET GetFunctionParameterName(FGrannyDAG2ScalarCallbacks::ECallbackFunction a1, int a2);
/// 0x08cc2204
UNK_RET GetMaxParameters(FGrannyDAG2ScalarCallbacks::ECallbackFunction a1);
/// 0x08cc2224
UNK_RET IsFunctionParameterRequired(FGrannyDAG2ScalarCallbacks::ECallbackFunction a1, int a2);
/// 0x08cc2502
UNK_RET GetFunction(FGrannyDAG2ScalarCallbacks::ECallbackFunction a1);
/// 0x08cc2522
UNK_RET BindCallbackFunctionsToGrannyInstance(FGrannyAssetInstance* a1, granny_dag2_instance* a2, unsigned int a3);
}
