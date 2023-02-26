#include "Application.h"

namespace Arklumos
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application()
	{
		this->m_Window = std::unique_ptr<Window>(Window::Create());
		this->m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(Event &e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		AK_CORE_TRACE("{0}", e);
	}

	void Application::Run()
	{
		// Create WindowResizeEvent
		// WindowResizeEvent e(1280, 720);
		// if (e.IsInCategory(EventCategoryApplication))
		// {
		// 	AK_TRACE(e);
		// }
		// if (e.IsInCategory(EventCategoryInput))
		// {
		// 	AK_TRACE(e);
		// }

		while (this->m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			this->m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent &e)
	{
		this->m_Running = false;
		return true;
	}

}