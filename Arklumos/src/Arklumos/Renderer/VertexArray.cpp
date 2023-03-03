#include "akpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Arklumos
{

	/*
		Static method that creates a new instance of a vertex array object, depending on the current renderer API.
		The method begins by calling the static GetAPI method on the Renderer class to determine which renderer API is currently in use.

		If the renderer API is set to RendererAPI::None, an assertion failure is thrown with an error message indicating that this renderer API is not supported. In this case, the method returns a null pointer.
		If the renderer API is set to RendererAPI::OpenGL, the method creates a new instance of an OpenGLVertexArray object and returns it.
		If the renderer API is set to an unknown value, an assertion failure is thrown with an error message indicating that the renderer API is unknown. In this case, the method returns a null pointer.
		Overall, this code is a factory method that creates a new vertex array object, depending on the current renderer API.
	*/
	VertexArray *VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			AK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return new OpenGLVertexArray();
		}

		AK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}