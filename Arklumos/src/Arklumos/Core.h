#pragma once

// #ifdef AK_PLATFORM_WINDOWS
//  #ifdef AK_BUILD_DLL
#define EXP_ARKLUMOS_API __declspec(dllexport)
// #else
#define IMP_ARKLUMOS_API __declspec(dllimport)
// #endif
// #else
// #error Arklumos only supports Windows!
// #endif