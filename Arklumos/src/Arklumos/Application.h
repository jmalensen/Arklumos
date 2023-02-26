#pragma once

#include "akpch.h"

#include "Core.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Log.h"
#include "Window.h"

#include <GLFW/glfw3.h>

namespace Arklumos
{

	class ARKLUMOS_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event &e);

	private:
		bool OnWindowClose(WindowCloseEvent &e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// To be defined in CLIENT
	Application *CreateApplication();

}