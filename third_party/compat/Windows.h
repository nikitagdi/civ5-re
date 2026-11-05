#pragma once
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <wchar.h>
#include <wctype.h>

typedef int BOOL;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef float FLOAT;
typedef double DOUBLE;
typedef char CHAR;
typedef short SHORT;
typedef long LONG;
typedef int INT;
typedef unsigned int UINT;
typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned long ULONG;
typedef wchar_t WCHAR;
typedef char TCHAR;
typedef signed char INT8;
typedef short INT16;
typedef int INT32;
typedef long long INT64;
typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned long long UINT64;
typedef long long LONGLONG;
typedef unsigned long long ULONGLONG;
typedef int INT_PTR;
typedef unsigned int UINT_PTR;
typedef long LONG_PTR;
typedef unsigned long ULONG_PTR;
typedef ULONG_PTR DWORD_PTR;
typedef ULONG_PTR SIZE_T;
typedef UINT_PTR WPARAM;
typedef long LPARAM;
typedef LONG_PTR LRESULT;
typedef long HRESULT;

typedef void* PVOID;
typedef void* LPVOID;
typedef const void* LPCVOID;
typedef void* HANDLE;
typedef const char* LPCSTR;
typedef char* LPSTR;
typedef const WCHAR* LPCWSTR;
typedef WCHAR* LPWSTR;
typedef const TCHAR* LPCTSTR;
typedef TCHAR* LPTSTR;
typedef BOOL* LPBOOL;
typedef DWORD* LPDWORD;
typedef BYTE* LPBYTE;

#define TRUE 1
#define FALSE 0
#define MAX_PATH 260
#define INFINITE 0xFFFFFFFF
#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)

struct HWND__ { int unused; };
typedef struct HWND__* HWND;
struct HINSTANCE__ { int unused; };
typedef struct HINSTANCE__* HINSTANCE;
typedef HINSTANCE HMODULE;
struct HICON__ { int unused; };
typedef struct HICON__* HICON;

typedef struct _GUID
{
    unsigned long Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char Data4[8];
} GUID;

typedef struct tagPOINT
{
    LONG x;
    LONG y;
} POINT;

typedef struct tagRECT
{
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
} RECT;

typedef struct _SYSTEMTIME
{
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME;

typedef struct _FILETIME
{
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME;

typedef union _LARGE_INTEGER
{
    struct
    {
        DWORD LowPart;
        LONG HighPart;
    };
    LONGLONG QuadPart;
} LARGE_INTEGER;

typedef struct _RTL_CRITICAL_SECTION
{
    void* DebugInfo;
    LONG LockCount;
    LONG RecursionCount;
    HANDLE OwningThread;
    HANDLE LockSemaphore;
    ULONG_PTR SpinCount;
} CRITICAL_SECTION;

struct _CONTEXT;
typedef struct _CONTEXT CONTEXT;

// Win32 takes volatile LONG*; Civ5 fields are int/unsigned, so any 4-byte integer type is accepted (same layout in the binary).
template <class T> struct civ5re_interlocked_ok { static_assert(sizeof(T) == sizeof(LONG), "Interlocked* needs a 4-byte integer"); typedef T type; };
template <class T> inline typename civ5re_interlocked_ok<T>::type InterlockedIncrement(volatile T* p) { return __sync_add_and_fetch(p, 1); }
template <class T> inline typename civ5re_interlocked_ok<T>::type InterlockedDecrement(volatile T* p) { return __sync_sub_and_fetch(p, 1); }
template <class T, class V> inline typename civ5re_interlocked_ok<T>::type InterlockedExchangeAdd(volatile T* p, V v) { return __sync_fetch_and_add(p, (T)v); }
template <class T, class V> inline typename civ5re_interlocked_ok<T>::type InterlockedExchange(volatile T* p, V v) { return __sync_lock_test_and_set(p, (T)v); }
template <class T, class E, class C> inline typename civ5re_interlocked_ok<T>::type InterlockedCompareExchange(volatile T* p, E e, C c) { return __sync_val_compare_and_swap(p, (T)c, (T)e); }
inline PVOID InterlockedCompareExchangePointer(volatile PVOID* p, PVOID e, PVOID c) { return __sync_val_compare_and_swap(p, c, e); }
template <class T, class V> inline typename civ5re_interlocked_ok<T>::type InterlockedAnd(volatile T* p, V v) { return __sync_fetch_and_and(p, (T)v); }
template <class T, class V> inline typename civ5re_interlocked_ok<T>::type InterlockedOr(volatile T* p, V v) { return __sync_fetch_and_or(p, (T)v); }

#define DECLSPEC_DEPRECATED
#define IMAGE_FILE_MACHINE_I386 0x014c

typedef unsigned long long DWORD64;
typedef ULONG* PULONG;

typedef enum { AddrMode1616, AddrMode1632, AddrModeReal, AddrModeFlat } ADDRESS_MODE;

typedef struct _tagADDRESS64
{
    DWORD64 Offset;
    WORD Segment;
    ADDRESS_MODE Mode;
} ADDRESS64;

typedef struct _tagSTACKFRAME64
{
    ADDRESS64 AddrPC;
    ADDRESS64 AddrReturn;
    ADDRESS64 AddrFrame;
    ADDRESS64 AddrStack;
    ADDRESS64 AddrBStore;
    PVOID FuncTableEntry;
    DWORD64 Params[4];
    BOOL Far;
    BOOL Virtual;
    DWORD64 Reserved[3];
} STACKFRAME64;

typedef BOOL (*PREAD_PROCESS_MEMORY_ROUTINE64)(HANDLE, DWORD64, PVOID, DWORD, LPDWORD);
typedef PVOID (*PFUNCTION_TABLE_ACCESS_ROUTINE64)(HANDLE, DWORD64);
typedef DWORD64 (*PGET_MODULE_BASE_ROUTINE64)(HANDLE, DWORD64);
typedef DWORD64 (*PTRANSLATE_ADDRESS_ROUTINE64)(HANDLE, HANDLE, ADDRESS64*);

HANDLE GetCurrentThread(void);
BOOL StackWalk64(DWORD, HANDLE, HANDLE, STACKFRAME64*, PVOID, PREAD_PROCESS_MEMORY_ROUTINE64,
                 PFUNCTION_TABLE_ACCESS_ROUTINE64, PGET_MODULE_BASE_ROUTINE64, PTRANSLATE_ADDRESS_ROUTINE64);
PVOID SymFunctionTableAccess64(HANDLE, DWORD64);
DWORD64 SymGetModuleBase64(HANDLE, DWORD64);
LONG CompareFileTime(const FILETIME*, const FILETIME*);

// Aspyr's Win32 emulation (binfacts group aspyr, C symbols); Win32 signatures.
typedef CRITICAL_SECTION* LPCRITICAL_SECTION;
typedef LARGE_INTEGER* PLARGE_INTEGER;
#define CP_ACP 0
#define CP_OEMCP 1
#define CP_UTF8 65001
#define MB_PRECOMPOSED 0x00000001
#define WC_ERR_INVALID_CHARS 0x00000080

extern "C" {
void InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
BOOL InitializeCriticalSectionAndSpinCount(LPCRITICAL_SECTION lpCriticalSection, DWORD dwSpinCount);
void DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
void EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
void LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
BOOL TryEnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
BOOL QueryPerformanceCounter(LARGE_INTEGER* lpPerformanceCount);
BOOL QueryPerformanceFrequency(LARGE_INTEGER* lpFrequency);
int MultiByteToWideChar(UINT CodePage, DWORD dwFlags, LPCSTR lpMultiByteStr, int cbMultiByte, LPWSTR lpWideCharStr, int cchWideChar);
int WideCharToMultiByte(UINT CodePage, DWORD dwFlags, LPCWSTR lpWideCharStr, int cchWideChar, LPSTR lpMultiByteStr, int cbMultiByte, LPCSTR lpDefaultChar, LPBOOL lpUsedDefaultChar);
void Sleep(DWORD dwMilliseconds);
DWORD GetTickCount(void);
DWORD GetLastError(void);
DWORD GetCurrentThreadId(void);
void OutputDebugStringA(LPCSTR lpOutputString);
void GetLocalTime(SYSTEMTIME* lpSystemTime);
void GetSystemTime(SYSTEMTIME* lpSystemTime);
BOOL CloseHandle(HANDLE hObject);
DWORD WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds);
DWORD TlsAlloc(void);
BOOL TlsFree(DWORD dwTlsIndex);
LPVOID TlsGetValue(DWORD dwTlsIndex);
BOOL TlsSetValue(DWORD dwTlsIndex, LPVOID lpTlsValue);
}

// Win32 file enumeration and message box (Aspyr emulation, C symbols).
#define FILE_ATTRIBUTE_READONLY 0x00000001
#define FILE_ATTRIBUTE_HIDDEN 0x00000002
#define FILE_ATTRIBUTE_SYSTEM 0x00000004
#define FILE_ATTRIBUTE_DIRECTORY 0x00000010
#define FILE_ATTRIBUTE_ARCHIVE 0x00000020
#define FILE_ATTRIBUTE_NORMAL 0x00000080
#define INVALID_FILE_ATTRIBUTES ((DWORD)0xFFFFFFFF)

typedef struct _WIN32_FIND_DATAA
{
    DWORD dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
    DWORD dwReserved0;
    DWORD dwReserved1;
    CHAR cFileName[MAX_PATH];
    CHAR cAlternateFileName[14];
} WIN32_FIND_DATAA;
typedef WIN32_FIND_DATAA* LPWIN32_FIND_DATAA;

#define MB_OK 0x00000000
#define MB_OKCANCEL 0x00000001
#define MB_ABORTRETRYIGNORE 0x00000002
#define MB_YESNOCANCEL 0x00000003
#define MB_YESNO 0x00000004
#define MB_RETRYCANCEL 0x00000005
#define MB_ICONHAND 0x00000010
#define MB_ICONERROR MB_ICONHAND
#define MB_ICONQUESTION 0x00000020
#define MB_ICONEXCLAMATION 0x00000030
#define MB_ICONWARNING MB_ICONEXCLAMATION
#define MB_ICONASTERISK 0x00000040
#define MB_ICONINFORMATION MB_ICONASTERISK
#define MB_SETFOREGROUND 0x00010000
#define MB_TOPMOST 0x00040000
#define IDOK 1
#define IDCANCEL 2
#define IDABORT 3
#define IDRETRY 4
#define IDIGNORE 5
#define IDYES 6
#define IDNO 7

#define WM_MOUSEMOVE 0x0200
#define WM_LBUTTONDOWN 0x0201
#define WM_LBUTTONUP 0x0202

extern "C" {
HANDLE FindFirstFileA(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData);
BOOL FindNextFileA(HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData);
BOOL FindClose(HANDLE hFindFile);
DWORD GetFileAttributesA(LPCSTR lpFileName);
int MessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
HWND SetCapture(HWND hWnd);
BOOL ReleaseCapture(void);
}

// D3DX9 math (d3dx9math.h layout); Aspyr exports the functions as C symbols.
typedef struct D3DXVECTOR2
{
    FLOAT x, y;
} D3DXVECTOR2;

typedef struct D3DXVECTOR3
{
    FLOAT x, y, z;
} D3DXVECTOR3;

typedef struct D3DXVECTOR4
{
    FLOAT x, y, z, w;
} D3DXVECTOR4;

typedef struct D3DXQUATERNION
{
    FLOAT x, y, z, w;
} D3DXQUATERNION;

typedef struct D3DXPLANE
{
    FLOAT a, b, c, d;
} D3DXPLANE;

typedef struct D3DXMATRIX
{
    union
    {
        struct
        {
            FLOAT _11, _12, _13, _14;
            FLOAT _21, _22, _23, _24;
            FLOAT _31, _32, _33, _34;
            FLOAT _41, _42, _43, _44;
        };
        FLOAT m[4][4];
    };
} D3DXMATRIX;

typedef struct D3DVIEWPORT9
{
    DWORD X, Y, Width, Height;
    FLOAT MinZ, MaxZ;
} D3DVIEWPORT9;

typedef struct D3DXFLOAT16
{
    WORD value;
} D3DXFLOAT16;

extern "C" {
D3DXVECTOR4* D3DXVec4Transform(D3DXVECTOR4* pOut, const D3DXVECTOR4* pV, const D3DXMATRIX* pM);
D3DXVECTOR2* D3DXVec2Normalize(D3DXVECTOR2* pOut, const D3DXVECTOR2* pV);
D3DXVECTOR3* D3DXVec3Normalize(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV);
D3DXVECTOR4* D3DXVec3Transform(D3DXVECTOR4* pOut, const D3DXVECTOR3* pV, const D3DXMATRIX* pM);
D3DXVECTOR3* D3DXVec3TransformNormal(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV, const D3DXMATRIX* pM);
D3DXVECTOR3* D3DXVec3TransformCoord(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV, const D3DXMATRIX* pM);
D3DXVECTOR3* D3DXVec3Unproject(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV, const D3DVIEWPORT9* pViewport,
                               const D3DXMATRIX* pProjection, const D3DXMATRIX* pView, const D3DXMATRIX* pWorld);
D3DXVECTOR3* D3DXPlaneIntersectLine(D3DXVECTOR3* pOut, const D3DXPLANE* pP, const D3DXVECTOR3* pV1, const D3DXVECTOR3* pV2);
D3DXMATRIX* D3DXMatrixMultiply(D3DXMATRIX* pOut, const D3DXMATRIX* pM1, const D3DXMATRIX* pM2);
D3DXMATRIX* D3DXMatrixScaling(D3DXMATRIX* pOut, FLOAT sx, FLOAT sy, FLOAT sz);
D3DXMATRIX* D3DXMatrixTranslation(D3DXMATRIX* pOut, FLOAT x, FLOAT y, FLOAT z);
D3DXMATRIX* D3DXMatrixOrthoOffCenterRH(D3DXMATRIX* pOut, FLOAT l, FLOAT r, FLOAT b, FLOAT t, FLOAT zn, FLOAT zf);
D3DXMATRIX* D3DXMatrixLookAtRH(D3DXMATRIX* pOut, const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt, const D3DXVECTOR3* pUp);
D3DXMATRIX* D3DXMatrixRotationX(D3DXMATRIX* pOut, FLOAT Angle);
D3DXMATRIX* D3DXMatrixRotationY(D3DXMATRIX* pOut, FLOAT Angle);
D3DXMATRIX* D3DXMatrixRotationZ(D3DXMATRIX* pOut, FLOAT Angle);
D3DXMATRIX* D3DXMatrixRotationAxis(D3DXMATRIX* pOut, const D3DXVECTOR3* pV, FLOAT Angle);
D3DXMATRIX* D3DXMatrixPerspectiveFovRH(D3DXMATRIX* pOut, FLOAT fovy, FLOAT Aspect, FLOAT zn, FLOAT zf);
D3DXMATRIX* D3DXMatrixTranspose(D3DXMATRIX* pOut, const D3DXMATRIX* pM);
D3DXPLANE* D3DXPlaneTransform(D3DXPLANE* pOut, const D3DXPLANE* pP, const D3DXMATRIX* pM);
D3DXMATRIX* D3DXMatrixInverse(D3DXMATRIX* pOut, FLOAT* pDeterminant, const D3DXMATRIX* pM);
BOOL D3DXIntersectTri(const D3DXVECTOR3* p0, const D3DXVECTOR3* p1, const D3DXVECTOR3* p2, const D3DXVECTOR3* pRayPos,
                      const D3DXVECTOR3* pRayDir, FLOAT* pU, FLOAT* pV, FLOAT* pDist);
D3DXFLOAT16* D3DXFloat32To16Array(D3DXFLOAT16* pOut, const FLOAT* pIn, UINT n);

// Aspyr CRT wrappers.
unsigned int rand_wincompatible(void);
void ASL_Msvc_qsort(void* base, size_t num, size_t width, int (*compare)(const void*, const void*));
}

// CRT secure functions, C++ symbols of the executable; the size is in elements, errno_t is int.
typedef int errno_t;
errno_t strcpy_s(char* dest, size_t destSize, const char* src);
errno_t strncpy_s(char* dest, size_t destSize, const char* src, size_t count);
errno_t strcat_s(char* dest, size_t destSize, const char* src);
int sprintf_s(char* buffer, size_t sizeOfBuffer, const char* format, ...);
int vsprintf_s(char* buffer, size_t sizeOfBuffer, const char* format, va_list argptr);
int _snprintf_s(char* buffer, size_t sizeOfBuffer, size_t count, const char* format, ...);
int _vsnprintf_s(char* buffer, size_t sizeOfBuffer, size_t count, const char* format, va_list argptr);
errno_t wcscpy_s(wchar_t* dest, size_t destSize, const wchar_t* src);
errno_t wcsncpy_s(wchar_t* dest, size_t destSize, const wchar_t* src, size_t count);
errno_t wcscat_s(wchar_t* dest, size_t destSize, const wchar_t* src);
int swprintf_s(wchar_t* buffer, size_t sizeOfBuffer, const wchar_t* format, ...);
errno_t memcpy_s(void* dest, size_t destSize, const void* src, size_t count);
errno_t memmove_s(void* dest, size_t destSize, const void* src, size_t count);
extern "C" int _snprintf(char* buffer, size_t count, const char* format, ...);
extern "C" int _vsnwprintf(wchar_t* buffer, size_t count, const wchar_t* format, va_list argptr);

inline int _stricmp(const char* a, const char* b) { return strcasecmp(a, b); }
inline int _strnicmp(const char* a, const char* b, size_t n) { return strncasecmp(a, b, n); }
inline int _stricoll(const char* a, const char* b) { return strcasecmp(a, b); }
inline int _wcsicmp(const wchar_t* a, const wchar_t* b) { return wcscasecmp(a, b); }
inline int _wcsnicmp(const wchar_t* a, const wchar_t* b, size_t n) { return wcsncasecmp(a, b, n); }
inline int _wcsicoll(const wchar_t* a, const wchar_t* b) { return wcscasecmp(a, b); }
inline int _vsnprintf(char* s, size_t n, const char* f, va_list v) { return vsnprintf(s, n, f, v); }
inline char* _strupr(char* s) { for (char* p = s; *p; ++p) *p = (char)toupper((unsigned char)*p); return s; }
inline char* _strlwr(char* s) { for (char* p = s; *p; ++p) *p = (char)tolower((unsigned char)*p); return s; }
inline char* _strrev(char* s) { size_t n = strlen(s); for (size_t i = 0; i < n / 2; ++i) { char c = s[i]; s[i] = s[n - 1 - i]; s[n - 1 - i] = c; } return s; }
inline char* _strnset(char* s, int c, size_t n) { for (char* p = s; *p && n; ++p, --n) *p = (char)c; return s; }
inline wchar_t* _wcsupr(wchar_t* s) { for (wchar_t* p = s; *p; ++p) *p = towupper(*p); return s; }
inline wchar_t* _wcslwr(wchar_t* s) { for (wchar_t* p = s; *p; ++p) *p = towlower(*p); return s; }
inline wchar_t* _wcsrev(wchar_t* s) { size_t n = wcslen(s); for (size_t i = 0; i < n / 2; ++i) { wchar_t c = s[i]; s[i] = s[n - 1 - i]; s[n - 1 - i] = c; } return s; }
inline wchar_t* _wcsnset(wchar_t* s, wchar_t c, size_t n) { for (wchar_t* p = s; *p && n; ++p, --n) *p = c; return s; }
inline size_t _msize(void* p) { return 0; }

#define _T(x) x
#define TEXT(x) x

#define fprintf_s fprintf
#define ZeroMemory(p, n) memset((p), 0, (n))
