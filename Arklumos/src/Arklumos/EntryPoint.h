#pragma once

#include "Application.h"

// #ifdef AK_PLATFORM_WINDOWS

extern Arklumos::Application *Arklumos::CreateApplication();

int main(int argc, char **argv)
{
	auto app = Arklumos::CreateApplication();
	app->Run();
	delete app;
}

// #endif