/// origin guess
#pragma once
#include <Windows.h>
#include <granny.h>

class alloc_combiner_tree;
class alloc_sorter;
class alloc_sorter_tree;
class binding_cache;
class binding_cache_free_list;
class dag_pose_cache_entry;
class fixup_remap_container;
class model_instance_counter;
class model_instance_counter_tree;
class retarget_cache;
enum LZDDATA : int;
namespace Nexus {
class HView;
}

namespace Nexus {
/// size ?
class ViewManager {
public:
    class View;
    /// 0x08df65b8
    ViewManager();
    /// 0x08df65d6
    ~ViewManager();
    /// 0x08df6634
    UNK_RET DestroySwapChain(Nexus::ViewManager::View& a1);
    /// 0x08df66a6
    UNK_RET CreateNexusView(HWND__* a1, bool a2);
    /// 0x08df6764
    UNK_RET FindNexusView(HWND__* a1);
    /// 0x08df67aa
    UNK_RET CreateSwapChain(Nexus::ViewManager::View& a1);
    /// 0x08df6866
    UNK_RET DestroyNexusView(Nexus::HView a1);
    /// 0x08df6896
    UNK_RET NexusViewResized(Nexus::HView a1);
    /// 0x08df68d2
    UNK_RET Render(Nexus::HView a1);
    /// 0x08df6992
    UNK_RET Present(Nexus::HView a1);
    /// 0x08df6a50
    UNK_RET ResetDevice();
    /// 0x08df6adc
    UNK_RET CheckDevice();
};
}

/// 0x08df7d91
UNK_RET GrannySampleDAG_PoseAnimSource(granny_dag2_node* a1, granny_dag2_nodedata_poseanimsource* a2, granny_dag2_instance* a3, int a4, float a5, granny_dag_pose_cache* a6);
/// 0x08df7e8b
UNK_RET GrannyFreezeDAGInstanceOffsets(granny_dag2* a1, granny_dag2_node* a2, int& a3, granny_model* a4, granny_model* a5);
/// 0x08dfd525
UNK_RET CurveFindFormatFromCurve(granny::curve2 const& a1);
/// 0x08e11acc
UNK_RET FreeLocalPoseChain(dag_pose_cache_entry* a1);
/// 0x08e14b20
UNK_RET PreserveFileSectionFormats(granny::file_data_tree_writer& a1, granny::file const* a2);
/// 0x08e1d42d
UNK_RET box_filter(float a1);
/// 0x08e1d465
UNK_RET linear_filter(float a1);
/// 0x08e1d499
UNK_RET mcubic_filter(float a1);
/// 0x08e23108
UNK_RET SetBlockToBAADF00D(int a1, void* a2);
/// 0x08e23138
UNK_RET BasicTrackedAllocate(char const* a1, int a2, int a3, int a4);
/// 0x08e2320a
UNK_RET BasicTrackedDeallocate(char const* a1, int a2, void* a3);
/// 0x08e24a6a
UNK_RET DeallocateSafeBuilder(granny::mesh_builder* a1);
/// 0x08e28704
UNK_RET LZ_decompress_alloc_size(unsigned int a1, unsigned int a2, unsigned int a3);
/// 0x08e2875e
UNK_RET LZ_decompress_open(void* a1, unsigned int a2, unsigned int a3, unsigned int a4);
/// 0x08e288b4
UNK_RET LZ_decompress_alloc_size_from_header(void* a1);
/// 0x08e28947
UNK_RET LZ_decompress_open_from_header(void* a1, void* a2);
/// 0x08e28b17
UNK_RET LZ_decompress(LZDDATA* a1, granny::_ARITHBITS* a2, unsigned char* a3);
/// 0x08e2a7f4
UNK_RET Initialize(fixup_remap_container* a1, int a2);
/// 0x08e2a80e
UNK_RET Add(fixup_remap_container* a1, granny::fixup_remap* a2);
/// 0x08e2aab7
UNK_RET Remove(fixup_remap_container* a1, granny::fixup_remap* a2);
/// 0x08e2ad9a
UNK_RET RemoveFirst(fixup_remap_container* a1);
/// 0x08e2adb8
UNK_RET RemoveLast(fixup_remap_container* a1);
/// 0x08e2add6
UNK_RET First(fixup_remap_container* a1);
/// 0x08e2adde
UNK_RET Next(fixup_remap_container* a1, granny::fixup_remap* a2);
/// 0x08e2ae43
UNK_RET Previous(fixup_remap_container* a1, granny::fixup_remap* a2);
/// 0x08e2aead
UNK_RET Last(fixup_remap_container* a1);
/// 0x08e2aeb5
UNK_RET Find(fixup_remap_container* a1, granny::grn_reference a2);
/// 0x08e2aeee
UNK_RET FindGT(fixup_remap_container* a1, granny::grn_reference a2);
/// 0x08e2af30
UNK_RET FindLT(fixup_remap_container* a1, granny::grn_reference a2);
/// 0x08e2af74
UNK_RET Clear(fixup_remap_container* a1);
/// 0x08e2ec20
UNK_RET Initialize(retarget_cache* a1, int a2);
/// 0x08e2ec3a
UNK_RET Add(retarget_cache* a1, granny::retargeter* a2);
/// 0x08e2eed3
UNK_RET Remove(retarget_cache* a1, granny::retargeter* a2);
/// 0x08e2f1be
UNK_RET RemoveFirst(retarget_cache* a1);
/// 0x08e2f1dc
UNK_RET RemoveLast(retarget_cache* a1);
/// 0x08e2f1fa
UNK_RET First(retarget_cache* a1);
/// 0x08e2f202
UNK_RET Next(retarget_cache* a1, granny::retargeter* a2);
/// 0x08e2f269
UNK_RET Previous(retarget_cache* a1, granny::retargeter* a2);
/// 0x08e2f2d3
UNK_RET Last(retarget_cache* a1);
/// 0x08e2f2db
UNK_RET Find(retarget_cache* a1, granny::retarget_identifier a2);
/// 0x08e2f31a
UNK_RET FindGT(retarget_cache* a1, granny::retarget_identifier a2);
/// 0x08e2f360
UNK_RET FindLT(retarget_cache* a1, granny::retarget_identifier a2);
/// 0x08e2f3a8
UNK_RET Clear(retarget_cache* a1);
/// 0x08e33076
UNK_RET s3tc_32bitValue(unsigned short* a1);
/// 0x08e3307d
UNK_RET s3tc_16bitValue(unsigned short* a1);
/// 0x08e33085
UNK_RET s3tc_OutputValue(unsigned int a1);
/// 0x08e33d64
UNK_RET find3from012(int a1, int a2, int a3, int a4);
/// 0x08e33e31
UNK_RET find0from123(int a1, int a2, int a3, int a4);
/// 0x08e35678
UNK_RET Initialize(model_instance_counter_tree* a1, int a2);
/// 0x08e35692
UNK_RET Add(model_instance_counter_tree* a1, model_instance_counter* a2);
/// 0x08e35905
UNK_RET Remove(model_instance_counter_tree* a1, model_instance_counter* a2);
/// 0x08e35be4
UNK_RET RemoveFirst(model_instance_counter_tree* a1);
/// 0x08e35c02
UNK_RET RemoveLast(model_instance_counter_tree* a1);
/// 0x08e35c20
UNK_RET First(model_instance_counter_tree* a1);
/// 0x08e35c28
UNK_RET Next(model_instance_counter_tree* a1, model_instance_counter* a2);
/// 0x08e35c83
UNK_RET Previous(model_instance_counter_tree* a1, model_instance_counter* a2);
/// 0x08e35ce5
UNK_RET Last(model_instance_counter_tree* a1);
/// 0x08e35ced
UNK_RET Find(model_instance_counter_tree* a1, granny::model* a2);
/// 0x08e35d1e
UNK_RET FindGT(model_instance_counter_tree* a1, granny::model* a2);
/// 0x08e35d53
UNK_RET FindLT(model_instance_counter_tree* a1, granny::model* a2);
/// 0x08e35d8e
UNK_RET Clear(model_instance_counter_tree* a1);
/// 0x08e35dae
UNK_RET Initialize(alloc_combiner_tree* a1, int a2);
/// 0x08e35dc8
UNK_RET Add(alloc_combiner_tree* a1, alloc_sorter* a2);
/// 0x08e360ad
UNK_RET Remove(alloc_combiner_tree* a1, alloc_sorter* a2);
/// 0x08e363a4
UNK_RET RemoveFirst(alloc_combiner_tree* a1);
/// 0x08e363c2
UNK_RET RemoveLast(alloc_combiner_tree* a1);
/// 0x08e363e0
UNK_RET First(alloc_combiner_tree* a1);
/// 0x08e363e8
UNK_RET Next(alloc_combiner_tree* a1, alloc_sorter* a2);
/// 0x08e3646b
UNK_RET Previous(alloc_combiner_tree* a1, alloc_sorter* a2);
/// 0x08e364f5
UNK_RET Last(alloc_combiner_tree* a1);
/// 0x08e364fd
UNK_RET Find(alloc_combiner_tree* a1, char const* a2, int a3);
/// 0x08e36554
UNK_RET FindGT(alloc_combiner_tree* a1, char const* a2, int a3);
/// 0x08e365bd
UNK_RET FindLT(alloc_combiner_tree* a1, char const* a2, int a3);
/// 0x08e36626
UNK_RET Clear(alloc_combiner_tree* a1);
/// 0x08e36646
UNK_RET Initialize(alloc_sorter_tree* a1, int a2);
/// 0x08e36660
UNK_RET Add(alloc_sorter_tree* a1, alloc_sorter* a2);
/// 0x08e36900
UNK_RET Remove(alloc_sorter_tree* a1, alloc_sorter* a2);
/// 0x08e36be5
UNK_RET RemoveFirst(alloc_sorter_tree* a1);
/// 0x08e36c03
UNK_RET RemoveLast(alloc_sorter_tree* a1);
/// 0x08e36c21
UNK_RET First(alloc_sorter_tree* a1);
/// 0x08e36c29
UNK_RET Next(alloc_sorter_tree* a1, alloc_sorter* a2);
/// 0x08e36c90
UNK_RET Previous(alloc_sorter_tree* a1, alloc_sorter* a2);
/// 0x08e36cfa
UNK_RET Last(alloc_sorter_tree* a1);
/// 0x08e36d02
UNK_RET Clear(alloc_sorter_tree* a1);
/// 0x08e3a7e8
UNK_RET PushLengthString(granny::string_table& a1, int a2, char const* a3);
/// 0x08e3a949
UNK_RET PushBoundedString(granny::string_table& a1, char const* a2, char const* a3);
/// 0x08e3a96e
UNK_RET PushZString(granny::string_table& a1, char const* a2);
/// 0x08e3a999
UNK_RET PushSize(granny::string_table& a1, int a2);
/// 0x08e415ad
UNK_RET FindSwizzlesFromLinear(float const* a1, int* a2, float* a3);
/// 0x08e49d78
UNK_RET AnsiDeleteFileWriter(granny::file_writer* a1);
/// 0x08e49dd5
UNK_RET AnsiSeekWriter(granny::file_writer& a1, int a2, granny::file_writer_seek_type a3);
/// 0x08e49e3b
UNK_RET AnsiWrite(granny::file_writer& a1, int a2, void const* a3);
/// 0x08e49eb8
UNK_RET AnsiBeginWriterCRC(granny::file_writer& a1);
/// 0x08e49ed2
UNK_RET AnsiEndWriterCRC(granny::file_writer& a1);
/// 0x08e4e81c
UNK_RET Initialize(binding_cache* a1, int a2);
/// 0x08e4e836
UNK_RET Add(binding_cache* a1, granny::animation_binding* a2);
/// 0x08e4eb2a
UNK_RET Remove(binding_cache* a1, granny::animation_binding* a2);
/// 0x08e4ee61
UNK_RET RemoveFirst(binding_cache* a1);
/// 0x08e4ee7f
UNK_RET RemoveLast(binding_cache* a1);
/// 0x08e4ee9d
UNK_RET First(binding_cache* a1);
/// 0x08e4eea5
UNK_RET Next(binding_cache* a1, granny::animation_binding* a2);
/// 0x08e4ef57
UNK_RET Previous(binding_cache* a1, granny::animation_binding* a2);
/// 0x08e4f013
UNK_RET Last(binding_cache* a1);
/// 0x08e4f01b
UNK_RET Find(binding_cache* a1, granny::animation_binding_identifier a2);
/// 0x08e4f0a6
UNK_RET FindGT(binding_cache* a1, granny::animation_binding_identifier a2);
/// 0x08e4f139
UNK_RET FindLT(binding_cache* a1, granny::animation_binding_identifier a2);
/// 0x08e4f1c8
UNK_RET Clear(binding_cache* a1);
/// 0x08e4f1e8
UNK_RET Initialize(binding_cache_free_list* a1, int a2);
/// 0x08e4f1fb
UNK_RET Add(binding_cache_free_list* a1, granny::animation_binding* a2);
/// 0x08e4f227
UNK_RET Remove(binding_cache_free_list* a1, granny::animation_binding* a2);
/// 0x08e4f252
UNK_RET RemoveFirst(binding_cache_free_list* a1);
/// 0x08e4f27e
UNK_RET RemoveLast(binding_cache_free_list* a1);
/// 0x08e4f2aa
UNK_RET First(binding_cache_free_list* a1);
/// 0x08e4f2b2
UNK_RET Next(binding_cache_free_list* a1, granny::animation_binding* a2);
/// 0x08e4f2ba
UNK_RET Previous(binding_cache_free_list* a1, granny::animation_binding* a2);
/// 0x08e4f2c2
UNK_RET Last(binding_cache_free_list* a1);
/// 0x08e4f2ca
UNK_RET Clear(binding_cache_free_list* a1);
/// 0x08e5200c
UNK_RET RGBtoYUV(short* a1, short* a2, short* a3, short* a4, int a5, int a6, void const* a7, int a8, int a9, int a10);
/// 0x08e522ee
UNK_RET RGBShiftUp(short* a1, short* a2, short* a3, short* a4, int a5, int a6, void const* a7, int a8, int a9, int a10);
/// 0x08e5258c
UNK_RET YUVtoRGB(void* a1, short const* a2, short const* a3, short const* a4, short const* a5, int a6, int a7, int a8);
/// 0x08e52718
UNK_RET RGBShiftDown(void* a1, short const* a2, short const* a3, short const* a4, short const* a5, int a6, int a7, int a8);
/// 0x08e52898
UNK_RET PtoRGB(unsigned int* a1, short const* a2, int a3, int a4);
/// 0x08e5c43f
UNK_RET AnimationGetControlledAnimation(granny::model_control_binding& a1);
/// 0x08e5c447
UNK_RET AnimationGetControlledPose(granny::model_control_binding& a1);
extern "C" {
    /// 0x08ff7a06
    UNK_RET _frkAnmNoteUpdateHead(UNK_PARAMS);
    /// 0x08ff7ac4
    UNK_RET _frkAnmNotesProcess(UNK_PARAMS);
    /// 0x08ff9c71
    UNK_RET _frkAnmParseImage(UNK_PARAMS);
    /// 0x08ff9ada
    UNK_RET _frkAnmSetTrk(UNK_PARAMS);
    /// 0x08ff986e
    UNK_RET _frkAnmUpdateHead(UNK_PARAMS);
    /// 0x0900440c
    UNK_RET _frkEEmitterDestroyCB(UNK_PARAMS);
    /// 0x0900452b
    UNK_RET _frkEmitterPathBezier(UNK_PARAMS);
    /// 0x0900080f
    UNK_RET _frkEmitterPathProcess(UNK_PARAMS);
    /// 0x08fff33e
    UNK_RET _frkLLSysClose(UNK_PARAMS);
    /// 0x08fff2b8
    UNK_RET _frkLLSysOpen(UNK_PARAMS);
    /// 0x08fff3ac
    UNK_RET _frkLLTracerPop(UNK_PARAMS);
    /// 0x08fff371
    UNK_RET _frkLLTracerPush(UNK_PARAMS);
    /// 0x08ff7758
    UNK_RET _frkMemSysClose(UNK_PARAMS);
    /// 0x08ff7720
    UNK_RET _frkMemSysOpen(UNK_PARAMS);
    /// 0x08ff9e9a
    UNK_RET _frkPAnmKeySetInterpData(UNK_PARAMS);
    /// 0x08ff9d0b
    UNK_RET _frkPAnmPrepare(UNK_PARAMS);
    /// 0x08ff9e87
    UNK_RET _frkPAnmTrkSetInterpFlags(UNK_PARAMS);
    /// 0x08ffea3d
    UNK_RET _frkPClose(UNK_PARAMS);
    /// 0x08ffa069
    UNK_RET _frkPCloseDefault(UNK_PARAMS);
    /// 0x090068b4
    UNK_RET _frkPEffectInfoInit(UNK_PARAMS);
    /// 0x08ffc33a
    UNK_RET _frkPEmitterBurstProcess(UNK_PARAMS);
    /// 0x09002203
    UNK_RET _frkPEmitterDefaultFullSim(UNK_PARAMS);
    /// 0x0900449f
    UNK_RET _frkPEmitterDefaultPhysics(UNK_PARAMS);
    /// 0x090037c4
    UNK_RET _frkPEmitterEmitProcess(UNK_PARAMS);
    /// 0x08ffff01
    UNK_RET _frkPEmitterInit(UNK_PARAMS);
    /// 0x09000e6a
    UNK_RET _frkPEmitterPreProcess(UNK_PARAMS);
    /// 0x08ffc56a
    UNK_RET _frkPEmitterPreStart(UNK_PARAMS);
    /// 0x09001241
    UNK_RET _frkPLODProcess(UNK_PARAMS);
    /// 0x08ffea03
    UNK_RET _frkPOpen(UNK_PARAMS);
    /// 0x08ff9f00
    UNK_RET _frkPOpenDefault(UNK_PARAMS);
    /// 0x0900685b
    UNK_RET _frkPSBAssociateAnimationAssets(UNK_PARAMS);
    /// 0x090067d8
    UNK_RET _frkPSBEmitterChunkGrab(UNK_PARAMS);
    /// 0x09001395
    UNK_RET _frkPSystemESortAscendFn(UNK_PARAMS);
    /// 0x09001365
    UNK_RET _frkPSystemESortDescendFn(UNK_PARAMS);
    /// 0x08fffe30
    UNK_RET _frkPSystemInit(UNK_PARAMS);
    /// 0x090013f3
    UNK_RET _frkPSystemPSortAscendFn(UNK_PARAMS);
    /// 0x090013c9
    UNK_RET _frkPSystemPSortDescendFn(UNK_PARAMS);
    /// 0x09003b7d
    UNK_RET _frkPSystemPostProcess(UNK_PARAMS);
    /// 0x09003726
    UNK_RET _frkPSystemPreProcess(UNK_PARAMS);
    /// 0x0900620d
    UNK_RET _frkParticleBirthArc(UNK_PARAMS);
    /// 0x090063fb
    UNK_RET _frkParticleBirthArcVol(UNK_PARAMS);
    /// 0x0900084f
    UNK_RET _frkParticleBirthBezier(UNK_PARAMS);
    /// 0x09005154
    UNK_RET _frkParticleBirthCone(UNK_PARAMS);
    /// 0x09005ad5
    UNK_RET _frkParticleBirthConeVol(UNK_PARAMS);
    /// 0x09005418
    UNK_RET _frkParticleBirthCylinder(UNK_PARAMS);
    /// 0x09005e6e
    UNK_RET _frkParticleBirthCylinderVol(UNK_PARAMS);
    /// 0x090003ce
    UNK_RET _frkParticleBirthPlane(UNK_PARAMS);
    /// 0x090055c2
    UNK_RET _frkParticleBirthPlaneVol(UNK_PARAMS);
    /// 0x09006090
    UNK_RET _frkParticleBirthPoint(UNK_PARAMS);
    /// 0x09004f4f
    UNK_RET _frkParticleBirthSphere(UNK_PARAMS);
    /// 0x0900585b
    UNK_RET _frkParticleBirthSphereVol(UNK_PARAMS);
    /// 0x09001421
    UNK_RET _frkParticleDefaultPhysics(UNK_PARAMS);
    /// 0x090026e6
    UNK_RET _frkParticlePathBezier(UNK_PARAMS);
    /// 0x09004c1a
    UNK_RET _frkParticlePathPhysics(UNK_PARAMS);
    /// 0x090030f3
    UNK_RET _frkParticleProcessFullSim(UNK_PARAMS);
    /// 0x09004c7c
    UNK_RET _frkPathCreate(UNK_PARAMS);
    /// 0x09004f3e
    UNK_RET _frkPathPrepare(UNK_PARAMS);
    /// 0x09004f2d
    UNK_RET _frkPathPrepareImported(UNK_PARAMS);
    /// 0x09003f8d
    UNK_RET _frkSEmitterAuxBirth(UNK_PARAMS);
    /// 0x090007d7
    UNK_RET _frkSEmitterAuxDeath(UNK_PARAMS);
    /// 0x09004441
    UNK_RET _frkSEmitterAuxDeathA(UNK_PARAMS);
    /// 0x09000667
    UNK_RET _frkSEmitterProcess(UNK_PARAMS);
    /// 0x09000748
    UNK_RET _frkSEmitterSubmit(UNK_PARAMS);
    /// 0x08ffe6d0
    UNK_RET _frkWin32MultiCoreFullSimThread(UNK_PARAMS);
    /// 0x08ffe847
    UNK_RET _frkWin32MultiCoreReg(UNK_PARAMS);
    /// 0x08ffe97f
    UNK_RET _frkWin32MultiCoreUnReg(UNK_PARAMS);
    /// 0x08ffe73a
    UNK_RET _frkWin32ParticleFullSimASync(UNK_PARAMS);
    /// 0x08ffe76b
    UNK_RET _frkWin32ParticleFullSimReadyWait(UNK_PARAMS);
    /// 0x08ff9ed4
    UNK_RET frkAnmGetNoteCBData(UNK_PARAMS);
    /// 0x08ff9eb3
    UNK_RET frkAnmSetNoteCB(UNK_PARAMS);
    /// 0x08fffa3b
    UNK_RET frkLLClean(UNK_PARAMS);
    /// 0x08fff3e6
    UNK_RET frkLLCreate(UNK_PARAMS);
    /// 0x08fff48b
    UNK_RET frkLLCreateEx(UNK_PARAMS);
    /// 0x08fff528
    UNK_RET frkLLDestroy(UNK_PARAMS);
    /// 0x08fff7c6
    UNK_RET frkLLDestroyItem(UNK_PARAMS);
    /// 0x08fff5c6
    UNK_RET frkLLDestroyNode(UNK_PARAMS);
    /// 0x08fff918
    UNK_RET frkLLGrabItem(UNK_PARAMS);
    /// 0x08fff812
    UNK_RET frkLLGrabNode(UNK_PARAMS);
    /// 0x08fff628
    UNK_RET frkLLInsertItem(UNK_PARAMS);
    /// 0x08fff6a8
    UNK_RET frkLLInsertNode(UNK_PARAMS);
    /// 0x08fff957
    UNK_RET frkLLRemoveNode(UNK_PARAMS);
    /// 0x08fffacf
    UNK_RET frkLLSetCompareCB(UNK_PARAMS);
    /// 0x08fff9cd
    UNK_RET frkLLSort(UNK_PARAMS);
    /// 0x08ff7787
    UNK_RET frkMemAlloc(UNK_PARAMS);
    /// 0x08ff78ea
    UNK_RET frkMemCompact(UNK_PARAMS);
    /// 0x08ff78c0
    UNK_RET frkMemFree(UNK_PARAMS);
    /// 0x08ff7772
    UNK_RET frkMemGetAlign(UNK_PARAMS);
    /// 0x08ff7947
    UNK_RET frkMemGetLargestBlock(UNK_PARAMS);
    /// 0x08ff798b
    UNK_RET frkMemGetTotalFreeMemory(UNK_PARAMS);
    /// 0x08ff7759
    UNK_RET frkMemSetAlign(UNK_PARAMS);
    /// 0x08ff9cbd
    UNK_RET frkPAnmGetInstMemSize(UNK_PARAMS);
    /// 0x08ff9e6e
    UNK_RET frkPAnmGetTrk(UNK_PARAMS);
    /// 0x08ff9dba
    UNK_RET frkPAnmGetTrkKeyTracer(UNK_PARAMS);
    /// 0x08ff9da5
    UNK_RET frkPAnmIncTrkKeyTracer(UNK_PARAMS);
    /// 0x08ff9cc3
    UNK_RET frkPAnmInit(UNK_PARAMS);
    /// 0x08ff9de3
    UNK_RET frkPAnmIsEnded(UNK_PARAMS);
    /// 0x08ff9ef6
    UNK_RET frkPAnmNoteGetData(UNK_PARAMS);
    /// 0x08ff9ee7
    UNK_RET frkPAnmNoteGetFrame(UNK_PARAMS);
    /// 0x08ff9eee
    UNK_RET frkPAnmNoteGetID(UNK_PARAMS);
    /// 0x08ff79cc
    UNK_RET frkPAnmNoteProcess(UNK_PARAMS);
    /// 0x08ff7b95
    UNK_RET frkPAnmProcess(UNK_PARAMS);
    /// 0x08ff9d8c
    UNK_RET frkPAnmResetTrkKeyTracer(UNK_PARAMS);
    /// 0x08ff9df6
    UNK_RET frkPAnmRestart(UNK_PARAMS);
    /// 0x08ff9b7a
    UNK_RET frkPAnmSet(UNK_PARAMS);
    /// 0x08ff9c52
    UNK_RET frkPAnmSetLoop(UNK_PARAMS);
    /// 0x08ff9c3c
    UNK_RET frkPAnmSetSpd(UNK_PARAMS);
    /// 0x08ff9ee2
    UNK_RET frkPAnmTrkGetName(UNK_PARAMS);
    /// 0x08ff9e7c
    UNK_RET frkPAnmTrkIsLoop(UNK_PARAMS);
    /// 0x08ffd2d5
    UNK_RET frkPAssetGetDataPointer(UNK_PARAMS);
    /// 0x08ffd280
    UNK_RET frkPAssetGetKey(UNK_PARAMS);
    /// 0x08ffd288
    UNK_RET frkPAssetGetKeyFileName(UNK_PARAMS);
    /// 0x08ffd279
    UNK_RET frkPAssetGetType(UNK_PARAMS);
    /// 0x08ffab80
    UNK_RET frkPAssetGetUVs(UNK_PARAMS);
    /// 0x08ffd2e0
    UNK_RET frkPAssetSetDataPointer(UNK_PARAMS);
    /// 0x08ffad28
    UNK_RET frkPBufferEmitterIncTracer(UNK_PARAMS);
    /// 0x08ffad32
    UNK_RET frkPBufferParticleIncTracer(UNK_PARAMS);
    /// 0x08ffa6cb
    UNK_RET frkPBufferProcess(UNK_PARAMS);
    /// 0x08ffad20
    UNK_RET frkPBufferResetTracer(UNK_PARAMS);
    /// 0x08ffabcb
    UNK_RET frkPBufferSet(UNK_PARAMS);
    /// 0x08ffd377
    UNK_RET frkPChunkGetData(UNK_PARAMS);
    /// 0x08ffd2ef
    UNK_RET frkPChunkGetKey(UNK_PARAMS);
    /// 0x08ffd2fc
    UNK_RET frkPChunkGetSize(UNK_PARAMS);
    /// 0x08ffd2f4
    UNK_RET frkPChunkGetType(UNK_PARAMS);
    /// 0x08ffd304
    UNK_RET frkPChunkGetVersion(UNK_PARAMS);
    /// 0x08ffd349
    UNK_RET frkPCustomDataGetTracerChunk(UNK_PARAMS);
    /// 0x08ffd32b
    UNK_RET frkPCustomDataIncChunkTracer(UNK_PARAMS);
    /// 0x08ffd30c
    UNK_RET frkPCustomDataResetChunkTracer(UNK_PARAMS);
    /// 0x08ffe475
    UNK_RET frkPEffectAnmSetNoteCB(UNK_PARAMS);
    /// 0x08ffd612
    UNK_RET frkPEffectCreate(UNK_PARAMS);
    /// 0x08ffd65b
    UNK_RET frkPEffectCreateEx(UNK_PARAMS);
    /// 0x08ffe3a9
    UNK_RET frkPEffectDestroy(UNK_PARAMS);
    /// 0x08ffe666
    UNK_RET frkPEffectEmitterGetTracerItem(UNK_PARAMS);
    /// 0x08ffe638
    UNK_RET frkPEffectEmitterIncTracer(UNK_PARAMS);
    /// 0x08ffe624
    UNK_RET frkPEffectEmitterResetTracer(UNK_PARAMS);
    /// 0x08ffe656
    UNK_RET frkPEffectGetEffectInfo(UNK_PARAMS);
    /// 0x08ffe59f
    UNK_RET frkPEffectGetEmitters(UNK_PARAMS);
    /// 0x08ffe64e
    UNK_RET frkPEffectGetEventAnmInst(UNK_PARAMS);
    /// 0x08ffe65e
    UNK_RET frkPEffectGetNamePtr(UNK_PARAMS);
    /// 0x08ffd4fb
    UNK_RET frkPEffectInfoCreate(UNK_PARAMS);
    /// 0x08ffd572
    UNK_RET frkPEffectInfoCreateEx(UNK_PARAMS);
    /// 0x08ffd5e5
    UNK_RET frkPEffectInfoDestroy(UNK_PARAMS);
    /// 0x08ffe60c
    UNK_RET frkPEffectInfoGetEmitterInfos(UNK_PARAMS);
    /// 0x08ffe060
    UNK_RET frkPEffectMatrixPtr(UNK_PARAMS);
    /// 0x08ffe323
    UNK_RET frkPEffectProcess(UNK_PARAMS);
    /// 0x08ffe17c
    UNK_RET frkPEffectReg(UNK_PARAMS);
    /// 0x08ffe2d2
    UNK_RET frkPEffectRestart(UNK_PARAMS);
    /// 0x08ffe065
    UNK_RET frkPEffectSetCallback(UNK_PARAMS);
    /// 0x08ffdc2c
    UNK_RET frkPEffectSetMatrix(UNK_PARAMS);
    /// 0x08ffe680
    UNK_RET frkPEffectSetScale(UNK_PARAMS);
    /// 0x08ffe222
    UNK_RET frkPEffectUnReg(UNK_PARAMS);
    /// 0x08ffbc2e
    UNK_RET frkPEmitterBirth(UNK_PARAMS);
    /// 0x08ffadca
    UNK_RET frkPEmitterCreate(UNK_PARAMS);
    /// 0x08ffae13
    UNK_RET frkPEmitterCreateEx(UNK_PARAMS);
    /// 0x08ffbc96
    UNK_RET frkPEmitterDeath(UNK_PARAMS);
    /// 0x08ffa27f
    UNK_RET frkPEmitterDestroy(UNK_PARAMS);
    /// 0x08ffb183
    UNK_RET frkPEmitterEmit(UNK_PARAMS);
    /// 0x08ffc94a
    UNK_RET frkPEmitterGetAnmInst(UNK_PARAMS);
    /// 0x08ffbf20
    UNK_RET frkPEmitterGetCallback(UNK_PARAMS);
    /// 0x08ffbdef
    UNK_RET frkPEmitterGetForce(UNK_PARAMS);
    /// 0x08ffc8a8
    UNK_RET frkPEmitterGetLife(UNK_PARAMS);
    /// 0x08ffc939
    UNK_RET frkPEmitterGetNumLive(UNK_PARAMS);
    /// 0x08ffbfc6
    UNK_RET frkPEmitterGetPropertyInfo(UNK_PARAMS);
    /// 0x08ffc8b3
    UNK_RET frkPEmitterGetTime(UNK_PARAMS);
    /// 0x08ffc884
    UNK_RET frkPEmitterGetUserData(UNK_PARAMS);
    /// 0x08ffbcd7
    UNK_RET frkPEmitterKillParticles(UNK_PARAMS);
    /// 0x08ffaba1
    UNK_RET frkPEmitterParticleGetTracerItem(UNK_PARAMS);
    /// 0x08ffabb2
    UNK_RET frkPEmitterParticleIncTracer(UNK_PARAMS);
    /// 0x08ffab8a
    UNK_RET frkPEmitterParticleResetTracer(UNK_PARAMS);
    /// 0x08ffbc6a
    UNK_RET frkPEmitterPhysics(UNK_PARAMS);
    /// 0x08ffb057
    UNK_RET frkPEmitterProcess(UNK_PARAMS);
    /// 0x08ffb00a
    UNK_RET frkPEmitterReg(UNK_PARAMS);
    /// 0x08ffc648
    UNK_RET frkPEmitterRestart(UNK_PARAMS);
    /// 0x08ffbfb7
    UNK_RET frkPEmitterSetCBData(UNK_PARAMS);
    /// 0x08ffbe6b
    UNK_RET frkPEmitterSetCallback(UNK_PARAMS);
    /// 0x08ffc913
    UNK_RET frkPEmitterSetEmissionInterpDistanceThreshold(UNK_PARAMS);
    /// 0x08ffc926
    UNK_RET frkPEmitterSetEmissionInterpTParamInterval(UNK_PARAMS);
    /// 0x08ffc900
    UNK_RET frkPEmitterSetEmissionRateScale(UNK_PARAMS);
    /// 0x08ffc8d1
    UNK_RET frkPEmitterSetEmissionVolumeScale(UNK_PARAMS);
    /// 0x08ffbe18
    UNK_RET frkPEmitterSetForce(UNK_PARAMS);
    /// 0x08ffc895
    UNK_RET frkPEmitterSetLife(UNK_PARAMS);
    /// 0x08ffc757
    UNK_RET frkPEmitterSetMatrix(UNK_PARAMS);
    /// 0x08ffbfd1
    UNK_RET frkPEmitterSetPropertyInfo(UNK_PARAMS);
    /// 0x08ffc8be
    UNK_RET frkPEmitterSetScale(UNK_PARAMS);
    /// 0x08ffbf01
    UNK_RET frkPEmitterSetState(UNK_PARAMS);
    /// 0x08ffc875
    UNK_RET frkPEmitterSetUserData(UNK_PARAMS);
    /// 0x08ffc48f
    UNK_RET frkPEmitterSimulate(UNK_PARAMS);
    /// 0x08ffafb1
    UNK_RET frkPEmitterUnReg(UNK_PARAMS);
    /// 0x08ffaf18
    UNK_RET frkPEmitterUnRegDestroyChildren(UNK_PARAMS);
    /// 0x08ffa5da
    UNK_RET frkPEnableEmitterParticlePools(UNK_PARAMS);
    /// 0x08ffe79d
    UNK_RET frkPEnableMultiCoreWin32(UNK_PARAMS);
    /// 0x08ffa664
    UNK_RET frkPGetAuxBuffer(UNK_PARAMS);
    /// 0x08ffa626
    UNK_RET frkPGetNumLive(UNK_PARAMS);
    /// 0x08ffadb1
    UNK_RET frkPLODEnable(UNK_PARAMS);
    /// 0x08ffad3a
    UNK_RET frkPLODSetParams(UNK_PARAMS);
    /// 0x08ffcea0
    UNK_RET frkPPropertyGetAnmTrk(UNK_PARAMS);
    /// 0x08ffceb6
    UNK_RET frkPPropertyGetAsset(UNK_PARAMS);
    /// 0x08ffce95
    UNK_RET frkPPropertyGetCurAsset(UNK_PARAMS);
    /// 0x08ffce48
    UNK_RET frkPPropertyGetCustomData(UNK_PARAMS);
    /// 0x08ffce53
    UNK_RET frkPPropertyGetCustomDataSize(UNK_PARAMS);
    /// 0x08ffcf22
    UNK_RET frkPPropertyGetEmissionAsset(UNK_PARAMS);
    /// 0x08ffd00e
    UNK_RET frkPPropertyGetForce(UNK_PARAMS);
    /// 0x08ffd0b4
    UNK_RET frkPPropertyGetGroup(UNK_PARAMS);
    /// 0x08ffd146
    UNK_RET frkPPropertyGetMatrix(UNK_PARAMS);
    /// 0x08ffd150
    UNK_RET frkPPropertyGetMaxEmitterLife(UNK_PARAMS);
    /// 0x08ffcfe2
    UNK_RET frkPPropertyGetNumAssets(UNK_PARAMS);
    /// 0x08ffcff8
    UNK_RET frkPPropertyGetNumEmissionAssets(UNK_PARAMS);
    /// 0x08ffd13b
    UNK_RET frkPPropertyGetNumForces(UNK_PARAMS);
    /// 0x08ffcfed
    UNK_RET frkPPropertyGetNumRefAssets(UNK_PARAMS);
    /// 0x08ffd003
    UNK_RET frkPPropertyGetNumShaderAssets(UNK_PARAMS);
    /// 0x08ffd130
    UNK_RET frkPPropertyGetOrder(UNK_PARAMS);
    /// 0x08ffceec
    UNK_RET frkPPropertyGetRefAsset(UNK_PARAMS);
    /// 0x08ffcf58
    UNK_RET frkPPropertyGetShaderAsset(UNK_PARAMS);
    /// 0x08ffc2ee
    UNK_RET frkPPropertyGetShape(UNK_PARAMS);
    /// 0x08ffc955
    UNK_RET frkPPropertyGetState(UNK_PARAMS);
    /// 0x08ffcb16
    UNK_RET frkPPropertyGetVolume(UNK_PARAMS);
    /// 0x08ffcf8e
    UNK_RET frkPPropertySetAsset(UNK_PARAMS);
    /// 0x08ffce5e
    UNK_RET frkPPropertySetCurAsset(UNK_PARAMS);
    /// 0x08ffd061
    UNK_RET frkPPropertySetForce(UNK_PARAMS);
    /// 0x08ffccef
    UNK_RET frkPPropertySetShape(UNK_PARAMS);
    /// 0x08ffcb22
    UNK_RET frkPPropertySetState(UNK_PARAMS);
    /// 0x08ffcce0
    UNK_RET frkPPropertySetVolume(UNK_PARAMS);
    /// 0x08ffa5b0
    UNK_RET frkPRegSubmitCB(UNK_PARAMS);
    /// 0x08ffe4ad
    UNK_RET frkPSBAssociateAssets(UNK_PARAMS);
    /// 0x09006709
    UNK_RET frkPSBAssociateRefAssets(UNK_PARAMS);
    /// 0x08ffd4ce
    UNK_RET frkPSBChunkGetData(UNK_PARAMS);
    /// 0x08ffd4ea
    UNK_RET frkPSBChunkGetDataSize(UNK_PARAMS);
    /// 0x08ffd4e5
    UNK_RET frkPSBChunkGetName(UNK_PARAMS);
    /// 0x08ffd4c9
    UNK_RET frkPSBChunkGetTracerItem(UNK_PARAMS);
    /// 0x08ffd4d9
    UNK_RET frkPSBChunkGetType(UNK_PARAMS);
    /// 0x08ffd4b1
    UNK_RET frkPSBChunkIncTracer(UNK_PARAMS);
    /// 0x08ffd4a9
    UNK_RET frkPSBChunkResetTracer(UNK_PARAMS);
    /// 0x08ffe50e
    UNK_RET frkPSBEmitterAssociateAssets(UNK_PARAMS);
    /// 0x090066f8
    UNK_RET frkPSBGetTracerChunkDataSize(UNK_PARAMS);
    /// 0x090066f3
    UNK_RET frkPSBGetTracerChunkName(UNK_PARAMS);
    /// 0x090066cc
    UNK_RET frkPSBGetTracerData(UNK_PARAMS);
    /// 0x090066d7
    UNK_RET frkPSBGetTracerType(UNK_PARAMS);
    /// 0x090066ad
    UNK_RET frkPSBIncTracer(UNK_PARAMS);
    /// 0x090066e3
    UNK_RET frkPSBIsTracerLastChunk(UNK_PARAMS);
    /// 0x08ffd456
    UNK_RET frkPSBPrepare(UNK_PARAMS);
    /// 0x08ffd37f
    UNK_RET frkPSBPrepareEData(UNK_PARAMS);
    /// 0x090066a5
    UNK_RET frkPSBResetTracer(UNK_PARAMS);
    /// 0x08ffa5f3
    UNK_RET frkPSetGravityDir(UNK_PARAMS);
    /// 0x08ffeae9
    UNK_RET frkPSystemASyncEnable(UNK_PARAMS);
    /// 0x08ffa127
    UNK_RET frkPSystemCreate(UNK_PARAMS);
    /// 0x08ffa166
    UNK_RET frkPSystemCreateEx(UNK_PARAMS);
    /// 0x08ffa3a5
    UNK_RET frkPSystemDestroy(UNK_PARAMS);
    /// 0x08ffa6a6
    UNK_RET frkPSystemGetAuxBuffer(UNK_PARAMS);
    /// 0x08ffa11c
    UNK_RET frkPSystemGetMaxParticleCount(UNK_PARAMS);
    /// 0x08ffa68f
    UNK_RET frkPSystemGetNumLive(UNK_PARAMS);
    /// 0x08ffa582
    UNK_RET frkPSystemGetState(UNK_PARAMS);
    /// 0x08ffa41e
    UNK_RET frkPSystemProcess(UNK_PARAMS);
    /// 0x08ffea51
    UNK_RET frkPSystemProcessASync(UNK_PARAMS);
    /// 0x08ffeaa4
    UNK_RET frkPSystemProcessASyncReady(UNK_PARAMS);
    /// 0x08ffa10d
    UNK_RET frkPSystemSetMaxParticleCount(UNK_PARAMS);
    /// 0x08ffa468
    UNK_RET frkPSystemSetState(UNK_PARAMS);
    /// 0x08ffbfe0
    UNK_RET frkParticleProcess(UNK_PARAMS);
    /// 0x08fffd4d
    UNK_RET frkPoolAlloc(UNK_PARAMS);
    /// 0x08fffdbc
    UNK_RET frkPoolAllocBatch(UNK_PARAMS);
    /// 0x08fffc71
    UNK_RET frkPoolCreate(UNK_PARAMS);
    /// 0x08fffbc2
    UNK_RET frkPoolCreateEx(UNK_PARAMS);
    /// 0x08fffd24
    UNK_RET frkPoolDestroy(UNK_PARAMS);
    /// 0x08fffd79
    UNK_RET frkPoolFree(UNK_PARAMS);
    /// 0x08fffb2e
    UNK_RET frkPoolInit(UNK_PARAMS);
    /// 0x08fffae8
    UNK_RET frkPoolPrep(UNK_PARAMS);
    /// 0x08ffcd8f
    UNK_RET frkPropertyCopy(UNK_PARAMS);
    /// 0x08ffcd4d
    UNK_RET frkPropertyGetSize(UNK_PARAMS);
    /// 0x08fff0f5
    UNK_RET frkSDKClose(UNK_PARAMS);
    /// 0x08fff149
    UNK_RET frkSDKGet(UNK_PARAMS);
    /// 0x08ffeee4
    UNK_RET frkSDKOpen(UNK_PARAMS);
    /// 0x08ffec28
    UNK_RET frkSDKPrep(UNK_PARAMS);
    /// 0x08ffeb50
    UNK_RET frkSDKPrepPBuffers(UNK_PARAMS);
    /// 0x08ffeb94
    UNK_RET frkSDKPrepTuner(UNK_PARAMS);
    /// 0x08fff185
    UNK_RET frkSDKSet(UNK_PARAMS);
    /// 0x08ff9d78
    UNK_RET frkTRKPrepare(UNK_PARAMS);
}
