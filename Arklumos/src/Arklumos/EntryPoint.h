#pragma once

#include "Application.h"
#include "Log.h"

#ifdef AK_PLATFORM_WINDOWS

extern Arklumos::Application *Arklumos::CreateApplication();

int main(int argc, char **argv)
{
	Arklumos::Log::Init();
	AK_CORE_WARN("Initialized Log For engine!");
	int a = 5;
	AK_INFO("Hello! Var={0}", a);

	auto app = Arklumos::CreateApplication();
	app->Run();
	delete app;
}

#endif