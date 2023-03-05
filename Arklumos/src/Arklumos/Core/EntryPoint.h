#pragma once

#include "akpch.h"
#include "Arklumos/Core/Application.h"
#include "Arklumos/Core/Log.h"

#ifdef AK_PLATFORM_WINDOWS

extern Arklumos::Application *Arklumos::CreateApplication();

int main(int argc, char **argv)
{
	Arklumos::Log::Init();
	AK_CORE_WARN("Initialized Windows Log For engine!");

	// AK_PROFILE_BEGIN_SESSION("Startup", "ArklumosProfile-Startup.json");
	auto app = Arklumos::CreateApplication();
	// AK_PROFILE_END_SESSION();

	// AK_PROFILE_BEGIN_SESSION("Runtime", "ArklumosProfile-Runtime.json");
	app->Run();
	// AK_PROFILE_END_SESSION();

	// AK_PROFILE_BEGIN_SESSION("Shutdown", "ArklumosProfile-Shutdown.json");
	delete app;
	// AK_PROFILE_END_SESSION();
}

#elif defined(AK_PLATFORM_LINUX)

extern Arklumos::Application *Arklumos::CreateApplication();

int main(int argc, char **argv)
{
	Arklumos::Log::Init();
	AK_CORE_WARN("Initialized GNU/Linux Log For engine!");

	// AK_PROFILE_BEGIN_SESSION("Startup", "ArklumosProfile-Startup.json");
	auto app = Arklumos::CreateApplication();
	// AK_PROFILE_END_SESSION();

	// AK_PROFILE_BEGIN_SESSION("Runtime", "ArklumosProfile-Runtime.json");
	app->Run();
	// AK_PROFILE_END_SESSION();

	// AK_PROFILE_BEGIN_SESSION("Shutdown", "ArklumosProfile-Shutdown.json");
	delete app;
	// AK_PROFILE_END_SESSION();
}

#endif