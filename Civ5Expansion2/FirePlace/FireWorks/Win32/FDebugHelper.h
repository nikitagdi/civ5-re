/// origin binary
#ifndef FDEBUGHELPER_H
#define FDEBUGHELPER_H

#include <Windows.h>
#if defined(_MSC_VER)
#include <dbghelp.h>
#endif

// If in debug, use the semi-undocumented RtlCaptureStackBackTrace function.  It is exposed in Vista, but it is part of XP too.
// There are limitations to it though, the code cannot be compiled with FPO (Frame Pointer Optimizations) on.
//#ifdef _DEBUG
#define USE_RTL_CAPTURE_STACK
//#endif

#ifdef USE_RTL_CAPTURE_STACK
// Access the semi-undocumented stack trace function
typedef USHORT (WINAPI *CaptureStackBackTraceFuncType)(__in ULONG, __in ULONG, __out PVOID*, __out_opt PULONG);
#endif

// FILL_CONTEXT is a macro because it needs to get the stack, base and instruction registers at the
// exact point of interest.
// Note that currently it is only defined for X86 (i.e. IA32 machines), but the FDebugHelper class is designed to work
// with all supported machine types (IA64, Alpha, AMD64, etc.), so it would be pretty easy to make this
// class work with other architectures by appropriately defining FILL_CONTEXT
#ifdef _M_IX86
	#define FILL_CONTEXT( context ) \
		{ \
		__asm{ mov context.Esp, esp } \
		__asm{ mov context.Ebp, ebp } \
		__asm{ xor eax, eax } \
		__asm{ call jumpl } \
	jumpl: \
		__asm{ mov eax, [esp] } \
		__asm{ mov context.Eip, eax } \
		__asm{ pop eax } \
		}
#elif defined(_M_X64)
	// This might be slow, but, it's better than nothing.
	#define FILL_CONTEXT( context ) RtlCaptureContext( &context )
#else
	#define FILL_CONTEXT( context ) 
#endif

// FDebugHelper is a singleton class that contains some helper functions for walking the call stack
// and retrieving symbol information.
// 
// Basically, this is a pretty straightforward wrapper around the dbghelp library provided by Microsoft.
// For more info, do a help search on "dbghelp".
//
// In order for any of the functions to return meaningful information, FDebugHelper must be able to find
// the .pdb files that were created when the executable and libraries were built.
//

class FCallStack;
class FMiniDumper;

/// size 0xc
class FDebugHelper
{
public:
	/// 0x08dd0cde
	static FDebugHelper& GetInstance();
	/// inline
	static FDebugHelper* GetCurrentInstance() { return pInstance; };

	//	Load in the symbols for the current process.
	/// 0x08dd16f8
	bool LoadSymbols();
	//	Load in the symbols for the specified module.  Usually called for a delay-loaded DLL.
	/// 0x08dd1830
	bool LoadSymbols(HMODULE hModule);
	/// 0x08dd17c8
	void UnloadSymbols(HMODULE hModule);

	/// 0x08dd0dba
	bool GetSymbolName( DWORD64 dwAddress, TCHAR* name, unsigned int maxNameLength );
	/// 0x08dd196a
	bool GetSymbolName( const CONTEXT& context, TCHAR* name, unsigned int maxNameLength );

	/// 0x08dd0e6e
	bool GetSymbolFileLine( DWORD64 dwAddress, TCHAR* name, DWORD* pLineNumber, unsigned int maxNameLength );
	/// 0x08dd19b0
	bool GetSymbolFileLine( const CONTEXT& context, TCHAR* name, DWORD* pLineNumber, unsigned int maxNameLength );

	/// 0x08dd0d36
	bool GetModuleName( DWORD64 dwAddress, TCHAR* name, unsigned int maxNameLength );

	/**	Get the call stack, this will not include the call to this method.
		@param	iStrip		number of levels to strip off the top of the stack.  This is
							used to remove entries to the debugging point so that only relevant
							entries are displayed or tracked.
		@param	iDepth		limit the call stack depth, used to cut down on memory usage during
							debug sessions.  0 = full call stack. */
	/// 0x08dd1a06
	bool GetCallStack(FCallStack *pCallStack, int iStrip = 0, int iDepth = 0);

	/// inline
	template< class Function > void WalkStack( const CONTEXT& context, Function Func );

	// This function is registered with atexit to destroy our singleton
	/// 0x08dd1634
	static void DestroyDebugHelper();	// allow manual usage as public fxn

	typedef void (*ExceptCallback)(TCHAR* szExceptionText);
	/// inline
	void SetExceptionTextCB(ExceptCallback pCB) { m_pExceptionTextCB = pCB; }
	/// inline
	const ExceptCallback GetExceptionTextCB() const { return m_pExceptionTextCB; } 

	// Get our instance of the mini dumper.  Can be NULL before initialization of FDebugHelper.
#if !defined( FINAL_RELEASE )
	static FMiniDumper*	GetMiniDumper();
#endif

	// Waits for a debugger to be attached until execution (on this thread) continues.
	/// 0x08dd1c7e
	static void WaitForDebuggerAttachment(bool bShowWindow = true);

private:
	// private/disabled constructors / destructors / assignment
	/// 0x08dd1670
	FDebugHelper();
	/// 0x08dd16cc
	~FDebugHelper();
	/// absent
	FDebugHelper( const FDebugHelper& );
	/// absent
	const FDebugHelper& operator=( const FDebugHelper& );

	/// 0x100b231c
	static FDebugHelper* pInstance;

	HANDLE hCurrentProcess; ///< +0x0
	// optional callback function on exception catching (see FDebugHelperNS)
	ExceptCallback m_pExceptionTextCB; ///< +0x4

#ifdef USE_RTL_CAPTURE_STACK
	CaptureStackBackTraceFuncType m_pkCaptureStackBackTrace; ///< +0x8
#endif

};

#include "FirePlace/FireWorks/Win32/FDebugHelper.inl"

#endif // FDEBUGHELPER_H
