#pragma once

#include "akpch.h"
#include "Application.h"
#include "Log.h"

#ifdef AK_PLATFORM_WINDOWS

extern Arklumos::Application *Arklumos::CreateApplication();

int main(int argc, char **argv)
{
	Arklumos::Log::Init();
	AK_CORE_WARN("Initialized Windows Log For engine!");
	int a = 5;
	AK_INFO("Hello! Var={0}", a);

	auto app = Arklumos::CreateApplication();
	app->Run();
	delete app;
}

#elif defined(AK_PLATFORM_LINUX)

extern Arklumos::Application *Arklumos::CreateApplication();

int main(int argc, char **argv)
{
	Arklumos::Log::Init();
	AK_CORE_WARN("Initialized GNU/Linux Log For engine!");
	int a = 5;
	AK_INFO("Hello! Var={0}", a);

	auto app = Arklumos::CreateApplication();
	app->Run();
	delete app;
}

#endif