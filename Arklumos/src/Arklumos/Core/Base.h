#pragma once

#include <memory>

#include "Arklumos/Core/PlatformDetection.h"

#ifdef AK_DEBUG
#if defined(AK_PLATFORM_WINDOWS)
#define AK_DEBUGBREAK() __debugbreak()
#elif defined(AK_PLATFORM_LINUX)
#include <signal.h>
#define AK_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif
#define AK_ENABLE_ASSERTS
#else
#define AK_DEBUGBREAK()
#endif

#define AK_EXPAND_MACRO(x) x
#define AK_STRINGIFY_MACRO(x) #x

/* Allow us to use Macro BIT for example: BIT(1), BIT(2)
	Differents events can go in differents categories

	Specifically, the macro evaluates to 1 left-shifted by the value of x.
	For example, BIT(0) evaluates to 1 << 0, which is 1. BIT(1) evaluates to 1 << 1, which is 2. BIT(2) evaluates to 1 << 2, which is 4, and so on.

	This macro is commonly used in code that deals with bit flags or bit masks, which are values that use binary digits to represent a set of binary options or attributes.
	By using the BIT macro, code can easily define and manipulate bit flags using a more readable and convenient syntax.
*/
#define BIT(x) (1 << x)

#define AK_BIND_EVENT_FN(fn) [this](auto &&...args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Arklumos
{

	template <typename T>
	using Scope = std::unique_ptr<T>;
	template <typename T, typename... Args>
	constexpr Scope<T> CreateScope(Args &&...args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using Ref = std::shared_ptr<T>;
	template <typename T, typename... Args>
	constexpr Ref<T> CreateRef(Args &&...args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}

#include "Arklumos/Core/Log.h"
#include "Arklumos/Core/Assert.h"