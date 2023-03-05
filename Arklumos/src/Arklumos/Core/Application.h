#pragma once

#include "Arklumos/Core/Base.h"
#include "Arklumos/Core/Window.h"
#include "Arklumos/Core/LayerStack.h"
#include "Arklumos/Events/Event.h"
#include "Arklumos/Events/ApplicationEvent.h"

#include "Arklumos/Core/Timestep.h"

#include "Arklumos/ImGui/ImGuiLayer.h"

int main(int argc, char **argv);

namespace Arklumos
{

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void OnEvent(Event &e);

		void PushLayer(Layer *layer);
		void PushOverlay(Layer *layer);

		Window &GetWindow() { return *m_Window; }

		void Close();

		static Application &Get() { return *s_Instance; }

	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent &e);
		bool OnWindowResize(WindowResizeEvent &e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer *m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;

		float m_LastFrameTime = 0.0f;

		static Application *s_Instance;
		friend int ::main(int argc, char **argv);
	};

	// To be defined in CLIENT
	Application *CreateApplication();
}