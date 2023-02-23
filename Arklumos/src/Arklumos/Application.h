#pragma once

#include "Core.h"
#include <iostream>
#include <sstream>
#include <fstream>

namespace Arklumos
{

	class ARKLUMOS_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in CLIENT
	Application *CreateApplication();

}