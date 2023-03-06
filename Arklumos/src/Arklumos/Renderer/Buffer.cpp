#include "akpch.h"
#include "Arklumos/Renderer/Buffer.h"

#include "Arklumos/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Arklumos
{

	/*
		Implements a static function named Create that takes a uint32_t parameter named size and returns a Ref<VertexBuffer> object.

		This switch statement checks which graphics API is currently being used by the renderer.
		If the API is set to RendererAPI::API::None, the function will assert and return nullptr.
		If the API is set to RendererAPI::API::OpenGL, it will create a new OpenGLVertexBuffer object using the CreateRef function and return it.

		If the API is not set to either RendererAPI::API::None or RendererAPI::API::OpenGL, the function will assert and return nullptr.
		This ensures that the Create function is only called with a supported API.
	*/
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			AK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(size);
		}

		AK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	/*
		Implements a static method for creating vertex buffers.
		A vertex buffer is a region of memory used to store vertex data, which is used by the graphics processing unit (GPU) to render objects on the screen.

		The method takes in a pointer to an array of vertices and the size of that array as parameters.
		It first checks the current renderer API using the Renderer::GetAPI() method, which returns an enum value indicating the current graphics API being used by the application.

		If the API is set to RendererAPI::None, the method throws an assertion error since no graphics API is currently supported. Otherwise, if the API is set to RendererAPI::OpenGL, the method returns a new instance of the OpenGLVertexBuffer class, which is a subclass of VertexBuffer.
		If the API is unknown, the method throws another assertion error.
	*/
	Ref<VertexBuffer> VertexBuffer::Create(float *vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			AK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		AK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	/*
		Defines a static method Create of the IndexBuffer class, which returns a pointer to an instance of an index buffer.
		The method takes two arguments: indices, which is a pointer to an array of 32-bit unsigned integers representing the indices of the vertices in the buffer, and size, which is the size of the buffer in bytes.

		The method first calls the Renderer::GetAPI() method, which returns an enumeration value representing the currently active rendering API.
		It then uses a switch statement to determine which rendering API is being used and creates an instance of the appropriate index buffer implementation (in this case, OpenGLIndexBuffer).
		If the rendering API is not recognized, an assertion is raised with an error message, and the method returns nullptr.
	*/
	Ref<IndexBuffer> IndexBuffer::Create(uint32_t *indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			AK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLIndexBuffer>(indices, size);
		}

		AK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}