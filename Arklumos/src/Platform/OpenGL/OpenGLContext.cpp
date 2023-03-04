#include "akpch.h"
#include "Platform/OpenGL/OpenGLContext.h"

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
		AK_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		AK_CORE_ASSERT(status, "Failed to initialize Glad!");

		AK_CORE_INFO("OpenGL Info:");
		AK_CORE_INFO("Vendor: {}", reinterpret_cast<const char *>(glGetString(GL_VENDOR)));
		AK_CORE_INFO("Vendor: {}", reinterpret_cast<const char *>(glGetString(GL_RENDERER)));
		AK_CORE_INFO("Vendor: {}", reinterpret_cast<const char *>(glGetString(GL_VERSION)));

#ifdef AK_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		AK_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Arklumos requires at least OpenGL version 4.5!");
#endif
	}

	void OpenGLContext::SwapBuffers()
	{
		AK_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}

}