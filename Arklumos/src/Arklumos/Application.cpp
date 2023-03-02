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

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

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

		/*
			Creates a new vertex array using a static factory method named Create() that is defined in the VertexArray class.
			The reset() method sets the m_VertexArray smart pointer to the newly created vertex array
		*/
		m_VertexArray.reset(VertexArray::Create());

		/*
			The float vertices[3 * 7] line initializes an array of float values representing the vertices.
			Each set of 7 values represents a single vertex with x, y, z coordinates and RGBA color components.

			Here is the breakdown of the array:
				Vertex 1: (-0.5, -0.5, 0.0) with color (0.8, 0.2, 0.8, 1.0)
				Vertex 2: (0.5, -0.5, 0.0) with color (0.2, 0.3, 0.8, 1.0)
				Vertex 3: (0.0, 0.5, 0.0) with color (0.8, 0.8, 0.2, 1.0)
		*/
		float vertices[3 * 7] = {
				-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
				0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
				0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f};

		/*
			Creates a shared pointer to a vertex buffer, by calling the static Create function of the VertexBuffer class and passing in the array of vertices and its size.
		*/
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		/*
			After that, a BufferLayout object is created, which is used to define the layout of the vertex buffer.
			In this case, it specifies that the vertex buffer contains 2 elements, the first is a vec3 for position with the name a_Position, and the second is a vec4 for color with the name a_Color.
		*/
		BufferLayout layout = {
				{ShaderDataType::Float3, "a_Position"},
				{ShaderDataType::Float4, "a_Color"}};
		// The SetLayout function of the vertex buffer object is then called, which sets the buffer layout according to the BufferLayout object created earlier.
		vertexBuffer->SetLayout(layout);
		// The vertex buffer is added to the vertex array object by calling the AddVertexBuffer function of the vertex array object and passing in the shared pointer to the vertex buffer.
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		/*
			Creates an index buffer with 3 indices and sets it as the index buffer of a vertex array object.
			First, it creates an array of 3 unsigned integers called "indices" and initializes it with the values 0, 1, and 2.
		*/
		uint32_t indices[3] = {0, 1, 2};
		// Creates a shared pointer to an IndexBuffer object using the static "Create" method of the IndexBuffer class, passing in the "indices" array and the size of the array in bytes divided by the size of an unsigned integer.
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		// Sets the created index buffer as the index buffer of the vertex array object "m_VertexArray" by calling the "SetIndexBuffer" method and passing in the index buffer shared pointer.
		m_VertexArray->SetIndexBuffer(indexBuffer);

		/*
			Creates a new vertex array using a static factory method named Create() that is defined in the VertexArray class.
			The reset() method sets the m_SquareVA smart pointer to the newly created vertex array
		*/
		m_SquareVA.reset(VertexArray::Create());

		/*
			Defines an array of 4 vertices that represent a square. The vertices are defined in a three-dimensional space using the X, Y, and Z coordinates.
			The first vertex is located at (-0.75, -0.75, 0.0), the second vertex is at (0.75, -0.75, 0.0), the third vertex is at (0.75, 0.75, 0.0), and the fourth vertex is at (-0.75, 0.75, 0.0).
			This array is often used as the vertex data to draw a square using a graphics API like OpenGL or DirectX.
		*/
		float squareVertices[3 * 4] = {
				-0.75f, -0.75f, 0.0f,
				0.75f, -0.75f, 0.0f,
				0.75f, 0.75f, 0.0f,
				-0.75f, 0.75f, 0.0f};

		/*
			Creates a shared pointer to a vertex buffer object for the square geometry.
			The vertex buffer is created by calling the static Create method of the VertexBuffer class, which takes in the squareVertices array and its size as arguments.
			Once the vertex buffer is created, its layout is set to only include the position attribute by calling the SetLayout method and passing in a BufferLayout object with a single BufferElement that describes the position attribute.
			Finally, the vertex buffer is added to the vertex array object for the square geometry (m_SquareVA) by calling its AddVertexBuffer method and passing in the squareVB shared pointer.
		*/
		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({{ShaderDataType::Float3, "a_Position"}});
		m_SquareVA->AddVertexBuffer(squareVB);

		/*
			Defines an array of 6 indices that form a rectangle made of two triangles. The indices are stored in an array of unsigned integers called squareIndices.
			Then, a new shared pointer squareIB is created and assigned to the result of calling IndexBuffer::Create() with squareIndices and the size of squareIndices divided by the size of an unsigned int (i.e., the number of elements in the squareIndices array).
			Finally, the index buffer is set on the square vertex array by calling m_SquareVA->SetIndexBuffer(squareIB), which associates the index buffer with the vertex array.
		*/
		uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		/*
			Defines a vertex shader in GLSL (OpenGL Shading Language).
			The shader takes two inputs, a position attribute (a_Position) and a color attribute (a_Color), both of which are assigned locations 0 and 1 respectively.
			The shader then outputs two variables, v_Position and v_Color, which are interpolated by the graphics pipeline and passed to the fragment shader for each pixel.
			Finally, the shader assigns the position to gl_Position, which is a built-in variable that represents the final vertex position after all transformations have been applied.
		*/
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

		/*
			Defines a fragment shader source code as a string literal using raw string literals (R"( and )") in C++.

			The shader code sets the output color of the fragment shader (color) to the input color (v_Color) and ignores the input position (v_Position).

			The input and output variables of the shader are specified using in and out qualifiers respectively.
			The input variables v_Position and v_Color are specified as in variables and are expected to be interpolated across the triangle fragments by the graphics pipeline.
			The output variable color is specified as an out variable and will be used to set the color of the pixel.

			The shader uses the GLSL function vec4 to create a 4-component vector, which is used to set the output color.
			The first line sets the color based on the input position by transforming it using a scale and offset (v_Position * 0.5 + 0.5).
			However, the second line overrides this color with the input color (v_Color). So, the output color will be the same as the input color.
		*/
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

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

		/*
			Defines a single input variable a_Position, which is a 3D vector representing the vertex position.
			The input variable is associated with a vertex buffer in the rendering pipeline.

			The shader also defines a single output variable v_Position, which is a 3D vector representing the vertex position.
			The output variable is passed to the fragment shader for further processing.

			The main function of the shader sets the output variable v_Position to the value of the input variable a_Position, and sets the special variable gl_Position to a 4D vector representing the final vertex position in clip space.
			In this case, gl_Position is set to vec4(a_Position, 1.0), which means that the vertex is not transformed by any matrices and is positioned at a_Position in world space.
		*/
		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		/*
			Defines the source code for a simple blue-colored shader program in GLSL.
			The vertex shader takes in a 3D position vector as input and outputs the same position to a varying output variable, v_Position, as well as setting the built-in gl_Position output variable to the position vector with a fourth component of 1.0.

			The fragment shader takes in the v_Position varying variable as input and sets the output color to a solid blue color with a value of (0.2, 0.3, 0.8, 1.0) using a vec4 vector.
			The color output variable is declared as an out variable, meaning that it will be written to the output color buffer by the GPU at the end of the shader pipeline.

			Together, the vertex and fragment shaders define a simple pipeline that takes in a 3D position vector and outputs a solid blue color to the screen for each fragment (pixel) of the primitive being drawn.
			This shader program can be used to draw simple objects with a solid blue color.
		*/
		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		m_BlueShader.reset(new Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer *layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer *layer)
	{
		m_LayerStack.PushOverlay(layer);
	}

	void Application::OnEvent(Event &e)
	{
		/*
			The EventDispatcher object is constructed with the Event object and provides a Dispatch function that is used to invoke a callback function for the specific event type.
			In this case, the WindowCloseEvent type is checked and if it matches the event type, the OnWindowClose function is invoked using a macro called BIND_EVENT_FN.
		*/
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		/*
			The function then iterates through the m_LayerStack vector from back to front, calling the OnEvent function of each layer in turn.
			The OnEvent function of each layer is responsible for handling the event if it is applicable.
			If the event is marked as "handled" by one of the layers, the iteration is stopped using the break keyword.
		*/
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
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
		while (m_Running)
		{
			// This sets the clear color for the color buffer. The arguments are the red, green, blue, and alpha values for the color, respectively
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			// This clears the color buffer. Since we just set the clear color in the previous line, this will fill the entire screen with a dark gray color
			glClear(GL_COLOR_BUFFER_BIT);

			// This sets the current shader program to m_BlueShader. A shader program consists of a vertex shader and a fragment shader, which together define how the vertices in the scene should be rendered
			m_BlueShader->Bind();
			// This sets the current vertex array to m_SquareVA. A vertex array encapsulates the vertex buffer and index buffer for a mesh, and defines the layout of the vertex data
			m_SquareVA->Bind();
			/*
				This tells OpenGL to render the vertices in the currently bound vertex array as triangles, using the currently bound shader program.
				The GL_TRIANGLES argument specifies the primitive type to use (triangles), and the m_SquareVA->GetIndexBuffer()->GetCount() argument specifies the number of indices to use for rendering.
				The GL_UNSIGNED_INT argument specifies the type of the indices in the index buffer, and the nullptr argument specifies the offset from the start of the index buffer.
			*/
			glDrawElements(GL_TRIANGLES, m_SquareVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			// This sets the current shader program to m_Shader.
			m_Shader->Bind();
			// This sets the current vertex array to m_VertexArray
			m_VertexArray->Bind();
			/*
				This tells OpenGL to render the vertices in the currently bound vertex array as triangles, using the currently bound shader program.
				The GL_TRIANGLES argument specifies the primitive type to use (triangles), and the m_VertexArray->GetIndexBuffer()->GetCount() argument specifies the number of indices to use for rendering.
				The GL_UNSIGNED_INT argument specifies the type of the indices in the index buffer, and the nullptr argument specifies the offset from the start of the index buffer.
			*/
			glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			// Update for each layer
			for (Layer *layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			// Starts the ImGui rendering process
			// Begin() is a method provided by the ImGuiLayer class that initializes the rendering context
			m_ImGuiLayer->Begin();

			// Iterates over all the layers in the m_LayerStack and calls their OnImGuiRender() method
			for (Layer *layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}

			// Marks the end of the ImGui rendering process
			m_ImGuiLayer->End();

			// Updates the application window with the rendered ImGui elements and performs any other necessary updates
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent &e)
	{
		m_Running = false;
		return true;
	}

}