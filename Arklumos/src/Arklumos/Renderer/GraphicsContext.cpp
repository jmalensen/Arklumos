#include "akpch.h"
#include "Arklumos/Renderer/GraphicsContext.h"

#include "Arklumos/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Arklumos
{

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