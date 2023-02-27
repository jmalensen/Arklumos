#include "Application.h"

namespace Arklumos
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application *Application::s_Instance = nullptr;

	Application::Application()
	{
		AK_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		this->m_Window = std::unique_ptr<Window>(Window::Create());
		this->m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer *layer)
	{
		this->m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer *layer)
	{
		this->m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event &e)
	{
		/*
			The EventDispatcher object is constructed with the Event object and provides a Dispatch function that is used to invoke a callback function for the specific event type.
			In this case, the WindowCloseEvent type is checked and if it matches the event type, the OnWindowClose function is invoked using a macro called BIND_EVENT_FN.
		*/
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		// The AK_CORE_TRACE macro is then used to print out the event to the console for debugging purposes.
		AK_CORE_TRACE("{0}", e);

		/*
			The function then iterates through the m_LayerStack vector from back to front, calling the OnEvent function of each layer in turn.
			The OnEvent function of each layer is responsible for handling the event if it is applicable.
			If the event is marked as "handled" by one of the layers, the iteration is stopped using the break keyword.
		*/
		for (auto it = this->m_LayerStack.end(); it != this->m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
			{
				break;
			}
		}
	}

	void Application::Run()
	{
		while (this->m_Running)
		{
			glClearColor(0, 1, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			// Update for each layer
			for (Layer *layer : this->m_LayerStack)
			{
				layer->OnUpdate();
			}

			this->m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent &e)
	{
		this->m_Running = false;
		return true;
	}

}