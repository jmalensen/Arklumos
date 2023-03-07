#include "akpch.h"
#include "Arklumos/Renderer/Framebuffer.h"

#include "Arklumos/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Arklumos
{

	/*
		Create a Framebuffer object based on the current graphics API being used by the engine.

		The FramebufferSpecification parameter defines the specifications of the framebuffer to be created, such as its width, height, and attachment types.
		The method begins by checking the current graphics API using Renderer::GetAPI().
		If the API is RendererAPI::API::None, it means that the engine is not currently using any graphics API, so the method returns nullptr.
		If the API is RendererAPI::API::OpenGL, the method creates a new OpenGLFramebuffer object using the provided spec and returns it wrapped in a Ref smart pointer.
		If the API is unknown, the method throws an assertion error using AK_CORE_ASSERT and also returns nullptr.

		Finally, the method returns the created Ref<Framebuffer> object.
	*/
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification &spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			AK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLFramebuffer>(spec);
		}

		AK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}