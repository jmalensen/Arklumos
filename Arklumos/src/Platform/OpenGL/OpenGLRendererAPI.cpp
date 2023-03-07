#include "akpch.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Arklumos
{
	void OpenGLMessageCallback(
			unsigned source,
			unsigned type,
			unsigned id,
			unsigned severity,
			int length,
			const char *message,
			const void *userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:
			AK_CORE_CRITICAL(message);
			return;
		case GL_DEBUG_SEVERITY_MEDIUM:
			AK_CORE_ERROR(message);
			return;
		case GL_DEBUG_SEVERITY_LOW:
			AK_CORE_WARN(message);
			return;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			AK_CORE_TRACE(message);
			return;
		}

		AK_CORE_ASSERT(false, "Unknown severity level!");
	}

	/*
		Initializes the OpenGL renderer API. Here's what each line does:

			#ifdef AK_DEBUG: This is a preprocessor directive that checks if the AK_DEBUG macro is defined. If it is, the code inside the block will be compiled, otherwise it will be ignored.
			This is a common technique to enable/disable certain features during development and production builds.

			glEnable(GL_DEBUG_OUTPUT): This enables OpenGL debug output. When an error or warning occurs during rendering, OpenGL will output a message to the debug console.

			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS): This makes OpenGL wait for the debug callback function to complete before returning control to the application.
			This ensures that the debug message will be printed immediately after it is generated, which makes it easier to debug issues.

			glDebugMessageCallback(OpenGLMessageCallback, nullptr): This sets the debug message callback function to OpenGLMessageCallback.
			Whenever a debug message is generated, OpenGL will call this function and pass the message as an argument.
			The second argument is a user-defined context pointer that is passed to the callback function.

			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE): This configures the debug message control.
			The first two arguments specify which messages to enable or disable (in this case, all messages are enabled).
			The third argument specifies the minimum severity level of messages to enable (in this case, only notifications are enabled).
			The last three arguments are not used in this case.

			glEnable(GL_BLEND): This enables alpha blending, which allows transparency in rendered objects.

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA): This sets the blend function to use for alpha blending.
			In this case, it uses the common blend function that blends the source alpha value with the destination color value using one minus the source alpha value as the blending factor.

			glEnable(GL_DEPTH_TEST): This enables depth testing, which ensures that objects closer to the camera are rendered in front of objects further away.
	*/
	void OpenGLRendererAPI::Init()
	{
		// AK_PROFILE_FUNCTION();

#ifdef AK_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4 &color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	/*
		Draws the geometry contained in a vertex array using indexed rendering

		The function takes two arguments:

				vertexArray: a reference to a vertex array that contains the geometry to be drawn.
				indexCount: an optional parameter that specifies the number of indices to be rendered. If this parameter is zero (or not provided), the entire index buffer of the vertex array is rendered.

		The function starts by calculating the number of indices to be rendered based on the indexCount parameter or the index count of the vertex array's index buffer.
		It then calls glDrawElements with the GL_TRIANGLES mode and the number of indices to be rendered.
		The last parameter is set to nullptr because we are assuming that the index data is stored in the index buffer of the vertex array.

		Finally, the function calls glBindTexture to unbind any currently bound texture.
		This is done as a good practice to ensure that the state of the graphics pipeline is not affected by any previous drawing operations
	*/
	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray> &vertexArray, uint32_t indexCount)
	{
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}