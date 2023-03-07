#pragma once

#include "Arklumos/Core/Base.h"
#include "Arklumos/Core/Log.h"
#include <filesystem>

#ifdef AK_ENABLE_ASSERTS

// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
#define AK_INTERNAL_ASSERT_IMPL(type, check, msg, ...) \
	{                                                    \
		if (!(check))                                      \
		{                                                  \
			AK##type##ERROR(msg, __VA_ARGS__);               \
			AK_DEBUGBREAK();                                 \
		}                                                  \
	}
#define AK_INTERNAL_ASSERT_WITH_MSG(type, check, ...) AK_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define AK_INTERNAL_ASSERT_NO_MSG(type, check) AK_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", AK_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define AK_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define AK_INTERNAL_ASSERT_GET_MACRO(...) AK_EXPAND_MACRO(AK_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, AK_INTERNAL_ASSERT_WITH_MSG, AK_INTERNAL_ASSERT_NO_MSG))

// Currently accepts at least the condition and one additional parameter (the message) being optional
#define AK_ASSERT(...) AK_EXPAND_MACRO(AK_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
#define AK_CORE_ASSERT(...) AK_EXPAND_MACRO(AK_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__))
#else
#define AK_ASSERT(...)
#define AK_CORE_ASSERT(...)
#endif