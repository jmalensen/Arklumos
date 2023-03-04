#include "akpch.h"
#include "Arklumos/Core/Window.h"

#ifdef AK_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Arklumos
{

	Scope<Window> Window::Create(const WindowProps &props)
	{
#ifdef AK_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
#else
		AK_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}

}