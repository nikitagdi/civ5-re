// What the Win32 SDK users get from the game's precompiled header (CvGameCoreDLLPCH.h of the
// Lekmod DLL, lines 15-74 and the project defines) before any SDK header is parsed.
// Used by test_msvc.py: every SDK header must compile after this prelude with MSVC's own headers.
#pragma once

#pragma warning( disable : 4800 )
#pragma warning( disable : 4355 )

#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING
#define _SILENCE_CXX17_ADAPTOR_TYPEDEFS_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS

#define DEBUG_VARIABLE(x) (void)(sizeof(x))

#include "Civ5/Src/App/CvGameCoreDLLUtil/CvGameCoreDLLUtil_Win32Headers.h"
#include <algorithm>
#include <vector>
#include <list>
#include <math.h>
#include <tchar.h>
#include <assert.h>
#include <map>
#include <hash_map>
#include <limits>
#include <unordered_set>

#define DllExport   __declspec( dllexport )

typedef unsigned char    byte;
typedef unsigned int     uint;
typedef wchar_t          wchar;

#define LIMIT_RANGE(low, value, high) value = (value < low ? low : (value > high ? high : value));
#define M_PI       3.14159265358979323846
#define fM_PI		3.141592654f

#define STDEXT stdext
#define MAX(a, b) std::max(a, b)
#define MIN(a, b) std::min(a, b)

#define AI_PERF(perfFileName, baseStringName) ((void)0)
#define AI_PERF_FORMAT(perfFileName, FormatValue) ((void)0)
#define AI_PERF_FORMAT_NESTED(perfFileName, FormatValue) ((void)0)
