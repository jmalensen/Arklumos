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

/* Allow us to use Macro BIT for example: BIT(1), BIT(2)
	Differents events can go in differents categories

	Specifically, the macro evaluates to 1 left-shifted by the value of x.
	For example, BIT(0) evaluates to 1 << 0, which is 1. BIT(1) evaluates to 1 << 1, which is 2. BIT(2) evaluates to 1 << 2, which is 4, and so on.

	This macro is commonly used in code that deals with bit flags or bit masks, which are values that use binary digits to represent a set of binary options or attributes.
	By using the BIT macro, code can easily define and manipulate bit flags using a more readable and convenient syntax.
*/
#define BIT(x) (1 << x)