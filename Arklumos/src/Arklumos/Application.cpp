#include "akpch.h"
#include "Application.h"

#include "Log.h"

#include <glad/glad.h>

#include "Input.h"
namespace Arklumos
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application *Application::s_Instance = nullptr;

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Arklumos::ShaderDataType::Float:
			return GL_FLOAT;
		case Arklumos::ShaderDataType::Float2:
			return GL_FLOAT;
		case Arklumos::ShaderDataType::Float3:
			return GL_FLOAT;
		case Arklumos::ShaderDataType::Float4:
			return GL_FLOAT;
		case Arklumos::ShaderDataType::Mat3:
			return GL_FLOAT;
		case Arklumos::ShaderDataType::Mat4:
			return GL_FLOAT;
		case Arklumos::ShaderDataType::Int:
			return GL_INT;
		case Arklumos::ShaderDataType::Int2:
			return GL_INT;
		case Arklumos::ShaderDataType::Int3:
			return GL_INT;
		case Arklumos::ShaderDataType::Int4:
			return GL_INT;
		case Arklumos::ShaderDataType::Bool:
			return GL_BOOL;
		}

		AK_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

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

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float vertices[3 * 7] = {
				-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
				0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
				0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			BufferLayout layout = {
					{ShaderDataType::Float3, "a_Position"},
					{ShaderDataType::Float4, "a_Color"}};

			m_VertexBuffer->SetLayout(layout);
		}

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		uint32_t index = 0;
		const auto &layout = m_VertexBuffer->GetLayout();
		for (const auto &element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
														element.GetComponentCount(),
														ShaderDataTypeToOpenGLBaseType(element.Type),
														element.Normalized ? GL_TRUE : GL_FALSE,
														layout.GetStride(),
														(const void *)(uintptr_t)element.Offset);
			index++;
		}

		uint32_t indices[3] = {0, 1, 2};
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			out vec3 v_Position;
			out vec4 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		this->m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
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
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			this->m_Shader->Bind();

			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

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