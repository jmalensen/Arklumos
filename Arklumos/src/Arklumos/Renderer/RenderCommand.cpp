#include "akpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Arklumos
{

	RendererAPI *RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}