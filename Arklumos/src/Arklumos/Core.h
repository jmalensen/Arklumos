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

#ifdef AK_ENABLE_ASSERTS
#define AK_ASSERT(x, ...)                             \
	{                                                   \
		if (!(x))                                         \
		{                                                 \
			AK_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
			__debugbreak();                                 \
		}                                                 \
	}
#define AK_CORE_ASSERT(x, ...)                             \
	{                                                        \
		if (!(x))                                              \
		{                                                      \
			AK_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
			__debugbreak();                                      \
		}                                                      \
	}
#else
#define AK_ASSERT(x, ...)
#define AK_CORE_ASSERT(x, ...)
#endif

// Allow us to use BIT(1), BIT(2)
// Differents events can go in differents categories
#define BIT(x) (1 << x)