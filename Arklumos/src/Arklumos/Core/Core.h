#pragma once

#include <memory>

// Platform detection using predefined macros
#ifdef _WIN32
/* Windows x64/x86 */
#ifdef _WIN64
/* Windows x64  */
#define AK_PLATFORM_WINDOWS
#else
/* Windows x86 */
#error "x86 Builds are not supported!"
#endif
#elif defined(__APPLE__) || defined(__MACH__)
#include <TargetConditionals.h>
/* TARGET_OS_MAC exists on all the platforms
 * so we must check all of them (in this order)
 * to ensure that we're running on MAC
 * and not some other Apple platform */
#if TARGET_IPHONE_SIMULATOR == 1
#error "IOS simulator is not supported!"
#elif TARGET_OS_IPHONE == 1
#define AK_PLATFORM_IOS
#error "IOS is not supported!"
#elif TARGET_OS_MAC == 1
#define AK_PLATFORM_MACOS
#error "MacOS is not supported!"
#else
#error "Unknown Apple platform!"
#endif
/* We also have to check __ANDROID__ before __linux__
 * since android is based on the linux kernel
 * it has __linux__ defined */
#elif defined(__ANDROID__)
#define AK_PLATFORM_ANDROID
#error "Android is not supported!"
#elif defined(__linux__)
#define AK_PLATFORM_LINUX
#else
/* Unknown compiler/platform */
#error "Unknown platform!"
#endif // End of platform detection

#ifdef AK_PLATFORM_WINDOWS
#if AK_DYNAMIC_LINK

#ifdef AK_BUILD_DLL
#define ARKLUMOS_API __declspec(dllexport)
#else
#define ARKLUMOS_API __declspec(dllimport)
#endif

#else
#define ARKLUMOS_API
#endif
#endif

// #ifdef AK_PLATFORM_LINUX
// // Assuming GCC compiler is used on GNU/Linux
// // #define ARKLUMOS_API __attribute__ ((visibility ("default")))
// #endif

///
// Define ARKLUMOS_API for any platform
#if defined _WIN32 || defined __CYGWIN__

// #ifdef AK_BUILD_DLL
// // Exporting...
// #ifdef __GNUC__
// #define ARKLUMOS_API __attribute__((dllexport))
// #else
// #define ARKLUMOS_API __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
// #endif
// #else
// #ifdef __GNUC__
// #define ARKLUMOS_API __attribute__((dllimport))
// #else
// #define ARKLUMOS_API __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
// #endif
// #endif

// NOT defined _WIN32 || defined __CYGWIN__ (Linux)
#else

#if __GNUC__ >= 4
#define ARKLUMOS_API __attribute__((visibility("default")))
// #define NOT_EXPORTED  __attribute__ ((visibility ("hidden")))
#else
// #define ARKLUMOS_API
#endif

#endif
///

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

namespace Arklumos
{

	template <typename T>
	using Scope = std::unique_ptr<T>;

	template <typename T>
	using Ref = std::shared_ptr<T>;

}