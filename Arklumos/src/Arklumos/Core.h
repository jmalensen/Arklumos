#pragma once

#ifdef AK_PLATFORM_WINDOWS
#ifdef AK_BUILD_DLL
#define ARKLUMOS_API __declspec(dllexport)
#else
#define ARKLUMOS_API __declspec(dllimport)
#endif
#else
// #error Arklumos only supports Windows!
#endif

// Allow us to use BIT(1), BIT(2)
// Differents events can go in differents categories
#define BIT(x) (1 << x)