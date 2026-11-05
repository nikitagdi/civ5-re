#pragma once

#define WIN32 1
#define _WINPC 1
#define FINAL_RELEASE 1
#define CV_WORLD_BUILDER_MAP_IN_GAME 1

#define __stdcall
#define __cdecl
#define __fastcall
#define __thiscall
#define WINAPI
#define CALLBACK

#define __forceinline inline __attribute__((always_inline))
#define __int64 long long
#define __int32 int
#define __int16 short
#define __int8 char
#define __w64

#define __declspec(x) __civ5re_declspec_##x
#define __civ5re_declspec_align(n) __attribute__((aligned(n)))
#define __civ5re_declspec_dllimport
#define __civ5re_declspec_dllexport
#define __civ5re_declspec_novtable
#define __civ5re_declspec_naked __attribute__((naked))
#define __civ5re_declspec_noinline __attribute__((noinline))
#define __civ5re_declspec_noreturn __attribute__((noreturn))
#define __civ5re_declspec_selectany __attribute__((weak))
#define __civ5re_declspec_thread __thread

#define _alloca __builtin_alloca
#define __debugbreak() __builtin_trap()
#define __assume(x) __builtin_assume(x)
#define __alignof alignof
#define _CRTIMP

#define _In_
#define _In_z_
#define _In_opt_
#define _In_opt_z_
#define _In_bytecount_(n)
#define _Out_
#define _Inout_
#define _Inout_z_cap_(n)
#define _Inout_z_cap_c_(n)
#define _Ret_z_
#define _Ret_opt_
#define _Ret_opt_z_
#define _Ret_maybenull_
#define _Check_return_
#define __in
#define __out
#define __out_opt
#define __checkReturn

typedef void UNK_RET;
typedef unsigned char UNK_DATA;
#define UNK_PARAMS ...
