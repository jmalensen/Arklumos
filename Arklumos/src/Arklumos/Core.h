#pragma once

#ifdef AK_PLATFORM_WINDOWS
#ifdef AK_BUILD_DLL
#define ARKLUMOS_API __declspec(dllexport)
#else
#define ARKLUMOS_API __declspec(dllimport)
#endif
#endif

// #ifdef AK_PLATFORM_LINUX
// // Assuming GCC compiler is used on GNU/Linux
// // #define ARKLUMOS_API __attribute__ ((visibility ("default")))
// #endif




// Define EXPORTED for any platform
#if defined _WIN32 || defined __CYGWIN__
//   #ifdef WIN_EXPORT
//     // Exporting...
//     #ifdef __GNUC__
//       #define EXPORTED __attribute__ ((dllexport))
//     #else
//       #define EXPORTED __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
//     #endif
//   #else
//     #ifdef __GNUC__
//       #define EXPORTED __attribute__ ((dllimport))
//     #else
//       #define EXPORTED __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
//     #endif
//   #endif
//   #define NOT_EXPORTED
#else
  #if __GNUC__ >= 4
    #define ARKLUMOS_API __attribute__ ((visibility ("default")))
    //#define NOT_EXPORTED  __attribute__ ((visibility ("hidden")))
  #else
    //#define ARKLUMOS_API
  #endif
#endif





#ifdef AK_DEBUG
#define AK_ENABLE_ASSERTS
#endif

#ifdef AK_ENABLE_ASSERTS
#define AK_ASSERT(x, ...)                             \
	{                                                   \
		if (!(x))                                         \
		{                                                 \
			AK_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
		}                                                 \
	}
#define AK_CORE_ASSERT(x, ...)                             \
	{                                                        \
		if (!(x))                                              \
		{                                                      \
			AK_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
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

#define AK_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)