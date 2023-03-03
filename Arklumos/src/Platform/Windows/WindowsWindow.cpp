#include "akpch.h"
#include "WindowsWindow.h"

#include "Arklumos/Events/ApplicationEvent.h"
#include "Arklumos/Events/MouseEvent.h"
#include "Arklumos/Events/KeyEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace Arklumos
{

	static uint8_t s_GLFWWindowCount = 0;

	// Static function which is used as a callback function to handle GLFW errors
	static void GLFWErrorCallback(int error, const char *description)
	{
		AK_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window *Window::Create(const WindowProps &props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps &props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps &props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		AK_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (s_GLFWWindowCount == 0)
		{
			AK_CORE_INFO("Initializing GLFW");
			int success = glfwInit();
			AK_CORE_ASSERT(success, "Could not intialize GLFW!");

			glfwSetErrorCallback(GLFWErrorCallback);
		}

		/*
			GLFW window is created with the specified properties.
			The context is then made current, and the window user pointer is set to the data associated with the window.
			The vertical sync is enabled by calling the SetVSync function.
		*/
		m_p_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		++s_GLFWWindowCount;

		m_p_Context = CreateScope<OpenGLContext>(m_p_Window);
		m_p_Context->Init();

		glfwSetWindowUserPointer(m_p_Window, &m_Data);
		SetVSync(true);

		/// Set GLFW callbacks
		// Listen event window size
		// function sets the callback for resizing the window, which updates the WindowData struct associated with the window and calls the WindowResizeEvent event
		glfwSetWindowSizeCallback(m_p_Window, [](GLFWwindow *window, int width, int height)
															{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event); });

		// Listen event window close
		// function sets the callback for closing the window, which creates a WindowCloseEvent event
		glfwSetWindowCloseCallback(m_p_Window, [](GLFWwindow *window)
															 {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event); });

		// Listen event keypress / keyrelease
		// function sets the callback for key presses and releases, which creates either a KeyPressedEvent or a KeyReleasedEvent event depending on the action
		glfwSetKeyCallback(m_p_Window, [](GLFWwindow *window, int key, int scancode, int action, int mods)
											 {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
			} });

		// function sets the callback to be called when the user types a character while the window has focus. m_p_Window is a pointer to the GLFW window that the callback will be set on
		glfwSetCharCallback(m_p_Window, [](GLFWwindow *window, unsigned int keycode)
												{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.EventCallback(event); });

		// Listen event mousebutton pressed / released
		// function sets the callback for mouse button presses and releases, which creates either a MouseButtonPressedEvent or a MouseButtonReleasedEvent event depending on the action
		glfwSetMouseButtonCallback(m_p_Window, [](GLFWwindow *window, int button, int action, int mods)
															 {

			//Retrieves a reference to the WindowData struct associated with the window, which was previously set using glfwSetWindowUserPointer().
			//This allows the code to access any data associated with the window, such as event callbacks
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			} });

		// Listen event mouse scroll
		// function sets the callback for mouse scrolling, which creates a MouseScrolledEvent event
		glfwSetScrollCallback(m_p_Window, [](GLFWwindow *window, double xOffset, double yOffset)
													{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event); });

		// Listen event mouse movement
		// function sets the callback for mouse movement, which creates a MouseMovedEvent event
		glfwSetCursorPosCallback(m_p_Window, [](GLFWwindow *window, double xPos, double yPos)
														 {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event); });

		// All these events are passed to the EventCallback function in the WindowData struct, which is responsible for handling these events.
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_p_Window);
		if (--s_GLFWWindowCount == 0)
		{
			AK_CORE_INFO("Terminating GLFW");
			glfwTerminate();
		}
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		m_p_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

}
