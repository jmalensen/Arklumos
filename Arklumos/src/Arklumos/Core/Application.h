#pragma once

#include "Core.h"
#include "Window.h"
#include "Arklumos/Core/LayerStack.h"
#include "Arklumos/Events/Event.h"
#include "Arklumos/Events/ApplicationEvent.h"

#include "Arklumos/Core/Timestep.h"

#include "Arklumos/ImGui/ImGuiLayer.h"

#include "Arklumos/Renderer/OrthographicCamera.h"

namespace Arklumos
{

	class Application
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
		bool OnWindowResize(WindowResizeEvent &e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer *m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;

		float m_LastFrameTime = 0.0f;

		static Application *s_Instance;
	};

	// To be defined in CLIENT
	Application *CreateApplication();
}