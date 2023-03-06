#include "akpch.h"
#include "Arklumos/Renderer/GraphicsContext.h"

#include "Arklumos/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Arklumos
{

	/*
		Create a graphics context for a specific rendering API (in this case, OpenGL).

		The function first calls Renderer::GetAPI() to determine which rendering API is being used.
		If the API is None, it will return nullptr and assert an error.
		If the API is OpenGL, it will return a Scope smart pointer to an OpenGLContext object, passing the window pointer to its constructor.
		If the API is unknown, it will return nullptr and assert an error.
	*/
	Scope<GraphicsContext> GraphicsContext::Create(void *window)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			AK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateScope<OpenGLContext>(static_cast<GLFWwindow *>(window));
		}

		AK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}