#include "akpch.h"
#include "Application.h"

#include "Log.h"

#include <glad/glad.h>

#include "Input.h"
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

		/*
			Creates a new ImGuiLayer object and assigns it to the m_ImGuiLayer pointer variable of the current object.
			Then, it calls the PushOverlay function of the current object, passing the m_ImGuiLayer pointer as an argument.

			The PushOverlay function is used to add the m_ImGuiLayer as an overlay to the rendering pipeline.
			In other words, it will ensure that the ImGuiLayer is rendered on top of all other layers or objects in the scene.
		*/
		this->m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(this->m_ImGuiLayer);
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

			// Starts the ImGui rendering process
			// Begin() is a method provided by the ImGuiLayer class that initializes the rendering context
			this->m_ImGuiLayer->Begin();

			// Iterates over all the layers in the m_LayerStack and calls their OnImGuiRender() method
			for (Layer *layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}

			// Marks the end of the ImGui rendering process
			this->m_ImGuiLayer->End();

			// Updates the application window with the rendered ImGui elements and performs any other necessary updates
			this->m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent &e)
	{
		this->m_Running = false;
		return true;
	}

}