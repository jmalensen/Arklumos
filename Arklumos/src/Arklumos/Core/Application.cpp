#include "akpch.h"
#include "Arklumos/Core/Application.h"

#include "Arklumos/Core/Log.h"

#include "Arklumos/Renderer/Renderer.h"

#include "Arklumos/Core/Input.h"

#include <GLFW/glfw3.h>
namespace Arklumos
{

	Application *Application::s_Instance = nullptr;

	Application::Application(const std::string &name)
	{
		// AK_PROFILE_FUNCTION();

		AK_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = Window::Create(WindowProps(name));
		m_Window->SetEventCallback(AK_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		/*
			Creates a new ImGuiLayer object and assigns it to the m_ImGuiLayer pointer variable of the current object.
			Then, it calls the PushOverlay function of the current object, passing the m_ImGuiLayer pointer as an argument.
		*/
		m_ImGuiLayer = new ImGuiLayer();

		/*
			The PushOverlay function is used to add the m_ImGuiLayer as an overlay to the rendering pipeline.
			In other words, it will ensure that the ImGuiLayer is rendered on top of all other layers or objects in the scene.
		*/
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		// AK_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::PushLayer(Layer *layer)
	{
		// AK_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer *layer)
	{
		// AK_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::OnEvent(Event &e)
	{
		// AK_PROFILE_FUNCTION();

		/*
			The EventDispatcher object is constructed with the Event object and provides a Dispatch function that is used to invoke a callback function for the specific event type.
			In this case, the WindowCloseEvent type is checked and if it matches the event type, the OnWindowClose function is invoked using a macro called BIND_EVENT_FN.
		*/
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(AK_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(AK_BIND_EVENT_FN(Application::OnWindowResize));

		/*
			The function then iterates through the m_LayerStack vector from back to front, calling the OnEvent function of each layer in turn.
			The OnEvent function of each layer is responsible for handling the event if it is applicable.
			If the event is marked as "handled" by one of the layers, the iteration is stopped using the break keyword.
		*/
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled)
			{
				break;
			}
			(*it)->OnEvent(e);
		}
	}

	void Application::Run()
	{
		// AK_PROFILE_FUNCTION();

		while (m_Running)
		{
			// AK_PROFILE_SCOPE("RunLoop");

			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			// Update for each layer
			if (!m_Minimized)
			{
				{
					// AK_PROFILE_SCOPE("LayerStack OnUpdate");

					for (Layer *layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}

				// Starts the ImGui rendering process
				// Begin() is a method provided by the ImGuiLayer class that initializes the rendering context
				m_ImGuiLayer->Begin();
				{
					// AK_PROFILE_SCOPE("LayerStack OnImGuiRender");

					// Iterates over all the layers in the m_LayerStack and calls their OnImGuiRender() method
					for (Layer *layer : m_LayerStack)
					{
						layer->OnImGuiRender();
					}
				}
				// Marks the end of the ImGui rendering process
				m_ImGuiLayer->End();
			}

			// Updates the application window with the rendered ImGui elements and performs any other necessary updates
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent &e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent &e)
	{
		// AK_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}