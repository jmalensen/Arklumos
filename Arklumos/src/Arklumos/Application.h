#pragma once

#include "akpch.h"

#include "Core.h"
#include "Arklumos/LayerStack.h"
#include "Arklumos/Events/Event.h"
#include "Arklumos/Events/ApplicationEvent.h"
#include "Log.h"
#include "Arklumos/Window.h"

#include <glad/glad.h>

#include "Input.h"

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

		inline Window &GetWindow() { return *m_Window; }

		inline static Application &Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent &e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

		static Application *s_Instance;
	};

	// To be defined in CLIENT
	Application *CreateApplication();

}