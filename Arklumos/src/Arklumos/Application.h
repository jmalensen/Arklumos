#pragma once

#include "akpch.h"

#include "Core.h"
#include "Arklumos/LayerStack.h"
#include "Arklumos/Events/Event.h"
#include "Arklumos/Events/ApplicationEvent.h"
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

		void PushLayer(Layer *layer);
		void PushOverlay(Layer *layer);

	private:
		bool OnWindowClose(WindowCloseEvent &e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	// To be defined in CLIENT
	Application *CreateApplication();

}