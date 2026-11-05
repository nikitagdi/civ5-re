/// origin binary
//-----------------------------------------------------------------------------
//
// Memory Allocation Replacement Routines
//
//-----------------------------------------------------------------------------
// Copyright (c) 2006 Firaxis Games, Inc. All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __FIRAXIS_MEM_HOOKS_H__
#define __FIRAXIS_MEM_HOOKS_H__

#include "FirePlace/FireWorks/FFireTypes.h"
#include <stddef.h>

//-----------------------------------------------------------------------------
// Pool Types (tags are pool type specific)
//-----------------------------------------------------------------------------
typedef enum eMPoolType
{
	c_eMPoolTypeContainer = 0,	     // container allocations that need to be refactored
	c_eMPoolTypeSBA,			     // internal small block allocator grabs
	c_eMPoolTypeFireWorks,		     // generic FireWorks allocations
	c_eMPoolTypeFString,		     // all FStrings, all the time
    c_eMPoolTypeStringPalette,       // The string palette 
    c_eMPoolTypeStringHashPalette,   // The string hash palette 
	c_eMPoolTypeTinyXML,		     // TinyXML allocations
	c_eMPoolTypeFXml,			     // FXml allocations
	c_eMPoolTypeFFileIO,		     // FFileIO specific allocations (part of FireWorks)
	c_eMPoolTypeFireGrafix,		     // generic FireGrafix stuff
	c_eMPoolTypeTextures,		     // texture allocations only
	c_eMPoolTypeGranny,			     // animation/model stuff in Granny
	c_eMPoolTypeFireSound,		     // generic FireSound allocations
	c_eMPoolTypeSamples,		     // FireSound samples/segments/data only
	c_eMPoolTypeFireTune,		     // FireTune allocations
	c_eMPoolTypeFireWire,		     // FireWire allocations
	c_eMPoolTypeFireEngine,		     // FireEngine allocations
	c_eMPoolTypeJobManager,		     // job manager allocations (in FireEngine)
	c_eMPoolTypeGFC,			     // generic UI allocations
	c_eMPoolTypeUI,				     // generic UI allocations
	c_eMPoolTypeLua,			     // allocations from Lua
	c_eMPoolTypeGame,			     // generic game allocations
	c_eMPoolTypeGamebryo,		     // gamebryo internal allocations
	c_eMPoolTypePhysics,		     // PhysX allocations
	c_eMPoolTypeForkParticles,		 // Fork Particles allocations
	c_eMPoolTypeSTL,			     // STL specific allocations
	c_eMPoolTypeTool,			     // Marker for a Tool allocation, to allow mixing and matching of tool libs with game code
    c_eMPoolTypeGrannyAsset,         // Granny Asset allocations
    c_eMPoolTypeGrannyAssetInstance, // Granny Asset Instance allocations
    c_eMPoolTypeFGXModelTemplate,    // allocations made by model templates
    c_eMPoolTypeFGXModelInstance,    // allocations made by instancing models
    c_eMPoolTypeEventSystem,         // 
    c_eMPoolTypeInstructionCache,    //
    c_eMPoolTypeResourceManager,     // the res mgr singleton
    c_eMPoolTypeTriggerSystem,       // trigger systems
    c_eMPoolTypeFileManager,         // FFileManager
    c_eMPoolTypeStateMachine,        // finite state machine
    c_eMPoolTypeFloatVarList,        // FloatVarList
	c_eMPoolTypeDebugMenu,			 // Data for the Lua Debug Menu (events, etc.)
	c_eMPoolTypeCurve,				 // curve resources
	c_eDebugSafetyNet,               // used for tracking debug info
	c_eMPoolTypeUserStart		     // user types should start here; FirePlace stuff should all come before
};

enum eMPoolTag
{
	c_eMPoolTagDefault,

	// For c_eMPoolTagFireWorks
	c_eMPoolTagFireWorksDefaultNew = 1
};

enum eMHeaps
{
	c_eMHeapMain,
	c_eMHeapSBA,
	c_eMHeapGPU,
};
//-----------------------------------------------------------------------------
// Expose The Original Memory Management Functions
//-----------------------------------------------------------------------------

	#include "FirePlace/FireWorks/FUndefNew.h"

//-----------------------------------------------------------------------------
// C Memory Management Hooks
//-----------------------------------------------------------------------------

// we technically don't need the file, line, or tag if we're not tracking.
// if passing those parameters becomes a performance concern, it can be 
// easily changed.
#if defined (FXS_MEMORY_TRACKER) || defined (FXS_MEMORY_MANAGER)
	void * FireMalloc(size_t nSize, const char * szFile, int nLine, int nPoolType, int nPoolTag );
	void * FireMallocAligned(size_t nSize, size_t nAlignment, const char * szFile, int nLine, int nPoolType, int nPoolTag );
	void * FireCalloc(size_t nItems, size_t nSize, const char * szFile, int nLine, int nPoolType, int nPoolTag );
	void * FireRealloc(void * pBlock, size_t nSize, const char * szFile, int nLine, int nPoolType, int nPoolTag );
	void * FireReallocAligned(void * pBlock, size_t nSize, size_t nAlignment, const char * szFile, int nLine, int nPoolType, int nPoolTag );

	void FireFree(void * pBlock);
	void FireFreeAligned(void * pBlock);

    void * FireGuardBuild( void *pBlock, unsigned int size );
    void * FireGuardCheck( void *pBlock );

	size_t FireMemSize(void * pBlock);
	bool Validate();
#if !defined (FINAL_RELEASE)
#define VALIDATE_MEMORY // Validate();
#else
#define VALIDATE_MEMORY
#endif //FINAL_RELEASE
#endif

#ifndef VALIDATE_MEMORY
#define VALIDATE_MEMORY
#endif

//-----------------------------------------------------------------------------
// C++ Memory Management Hooks
//-----------------------------------------------------------------------------

// we technically don't need the file, line, or tag if we're not tracking.
// if passing those parameters becomes a performance issue, it can be easily 
// changed.
#if defined (FXS_MEMORY_TRACKER) || defined (FXS_MEMORY_MANAGER)
	// these we tripmine since we can't easily #define around them.
	void * operator new  (size_t nSize);
	void * operator new[](size_t nSize);

	// these are the ones we should be using
	void * operator new  (size_t nSize, int nBlockType, const char * szFile, int nLine, int nPool, int nTag);
	void * operator new[](size_t nSize, int nBlockType, const char * szFile, int nLine, int nPool, int nTag);

	// Need to have a matching delete for every type of new.
	void operator delete  (void * pBlock);
	void operator delete[](void * pBlock);	
	void operator delete  (void * pBlock, int nBlockType, const char * szFile, int nLine, int nPool, int nTag );
	void operator delete[](void * pBlock, int nBlockType, const char * szFile, int nLine, int nPool, int nTag );
#else
	// there's a handful of useful stuff we need to keep around in non-tracking mode
	/// 0x08db3981
	void * FireMallocAlignedNoTracking(size_t nSize, size_t nAlignment, const char * szFile, int nLine);
	/// 0x08db3a94
	void FireFreeAlignedNoTracking(void * pBlock);
#endif // FXS_MEMORY_TRACKER or FXS_MEMORY_MANAGER



//-----------------------------------------------------------------------------
// Memory Management Hooks (Tracking Only)
//-----------------------------------------------------------------------------

//	const void * const TrackAllocate(const void * const pBlock, size_t nSize, const char * szFile, int nLine, int nPool, int nTag);
//	const void * const TrackReallocate(const void * const pOldBlock, const void * const pNewBlock, size_t nSize, const char * szFile, int nLine, int nPool, int nTag);
//	const void * const TrackFree(const void * const pBlock);

//-----------------------------------------------------------------------------
// If you really, really, *REALLY* need to let something use the non-tracked
// new operator, use these.  
//-----------------------------------------------------------------------------
namespace FMemHooks
{
	/// 0x08db3a4a
	void EnableDefaultNewTraps();
	/// 0x08db3a4b
	unsigned int DisableDefaultNewTraps();
	/// 0x08db3a4e
	unsigned int SetDefaultNewTrapsState(unsigned int uiState);
	/// 0x08db3a51
	unsigned int AreDefaultNewTrapsEnabled();
	/// 0x08db3a54
	void EnableSBATracking( bool bEnable );
	/// 0x08db3a55
	bool IsSBATrackingEnabled();
}


//-----------------------------------------------------------------------------
// Replace The Original Memory Management Functions
//-----------------------------------------------------------------------------

	#include "FirePlace/FireWorks/FDefNew.h"

//-----------------------------------------------------------------------------
// Turn off some exception warnings
//-----------------------------------------------------------------------------
#pragma warning( disable : 4291 )		// no delete found if initialization throws an exception


#define FMEMORY_VALIDATE_DEFAULT			0
#define FMEMORY_VALIDATE_CONTENTS			0x00000001
#define FMEMORY_VALIDATE_ON_ALLOCATION		0x00000002
#define FMEMORY_VALIDATE_ON_DEALLOCATION	0x00000004

/// 0x08db3acb
bool FMemoryManagerValidate(uint uiOptions = 0);
/// 0x08db3add
uint FGetMemoryManagerValidateOptions();
/// 0x08db3af2
uint FSetMemoryManagerValidateOptions(uint uiOptions);
/// 0x08db3b0d
uint FSetMemoryManagerValidateFrequency(uint uiValidateFrequency);


/// 0x0a312ee8
extern bool g_bDisableSmallObjectAllocator; 

#ifdef _WINPC
 #ifdef FXS_IS_DLL
  #define FSBAExport 
 #else
  #define FSBAExport __declspec(dllexport)
 #endif
#else
  #define FSBAExport 
#endif

class FSmallObjectAllocator;
class FSmallObjectAllocatorInitParams;

extern "C" 
{
	/// 0x08db37f4
	FSBAExport FSmallObjectAllocator * GetSmallObjectAllocator(bool bCreate = true);
	/// 0x086df75e
	FSBAExport FSmallObjectAllocatorInitParams * GetSmallObjectAllocatorInitParams();

	/// 0x08db3818
	FSBAExport void* FSBAAllocate(size_t nSize);
	/// 0x08db383c
	FSBAExport bool FSBADeallocate(void* pBlock);
	/// 0x08db3860
	FSBAExport void FSBADisable(bool bState);
	/// 0x08db3882
	FSBAExport bool FSBAIsDisabled();
	/// 0x08db38a9
	FSBAExport bool FSBAIsSmallBlock(void* pBlock, size_t& stSize);
	/// 0x08db38cd
	FSBAExport bool FSBAHasAllocatorForSize(size_t tSize);
	/// 0x08db38f1
	FSBAExport size_t FSBAGetMaxAllocatorSize();
	/// 0x08db3915
	FSBAExport bool FSBAPreventCreation(bool bState);
}


#endif // Include Guard
