#include "akpch.h"
#include "Arklumos/Core/Window.h"

#ifdef AK_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#elif defined(AK_PLATFORM_LINUX)
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Arklumos
{

	Scope<Window> Window::Create(const WindowProps &props)
	{
		/*
			Uses preprocessor directives (AK_PLATFORM_WINDOWS or AK_PLATFORM_LINUX) to determine which platform the code is being compiled for and then calls the appropriate CreateScope function to create a new Window object.
			If the platform is not recognized, "Unknown platform!" is shown and nullptr is returned
		*/
#ifdef AK_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
#elif defined(AK_PLATFORM_LINUX)
		return CreateScope<WindowsWindow>(props);
#else
		AK_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}

}