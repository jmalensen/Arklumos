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