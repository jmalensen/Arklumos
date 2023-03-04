#include "akpch.h"
#include "Arklumos/Core/Input.h"

#ifdef AK_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsInput.h"
#elif defined(AK_PLATFORM_LINUX)
#include "Platform/Windows/WindowsInput.h"
#endif

namespace Arklumos
{

	Scope<Input> Input::s_Instance = Input::Create();

	Scope<Input> Input::Create()
	{
#ifdef AK_PLATFORM_WINDOWS
		return CreateScope<WindowsInput>();
#elif defined(AK_PLATFORM_LINUX)
		return CreateScope<WindowsInput>();
#else
		AK_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}
}