#include "akpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/gl.h>

namespace Arklumos
{

	OpenGLContext::OpenGLContext(GLFWwindow *windowHandle)
			: m_WindowHandle(windowHandle)
	{
		AK_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		AK_CORE_ASSERT(status, "Failed to initialize Glad!");

		AK_CORE_INFO("OpenGL Info:");
		AK_CORE_INFO("Vendor: {}", reinterpret_cast<const char *>(glGetString(GL_VENDOR)));
		AK_CORE_INFO("Vendor: {}", reinterpret_cast<const char *>(glGetString(GL_RENDERER)));
		AK_CORE_INFO("Vendor: {}", reinterpret_cast<const char *>(glGetString(GL_VERSION)));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}