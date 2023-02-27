#include "WindowsWindow.h"

namespace Arklumos
{

	static bool s_GLFWInitialized = false;

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
		this->m_Data.Title = props.Title;
		this->m_Data.Width = props.Width;
		this->m_Data.Height = props.Height;

		AK_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			AK_CORE_ASSERT(success, "Could not intialize GLFW!");

			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		/*
			GLFW window is created with the specified properties.
			The context is then made current, and the window user pointer is set to the data associated with the window.
			The vertical sync is enabled by calling the SetVSync function.
		*/
		this->m_p_Window = glfwCreateWindow((int)props.Width, (int)props.Height, this->m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(this->m_p_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		AK_CORE_ASSERT(status, "Failed to initialize Glad!");
		glfwSetWindowUserPointer(this->m_p_Window, &this->m_Data);
		SetVSync(true);

		/// Set GLFW callbacks
		// Listen event window size
		// function sets the callback for resizing the window, which updates the WindowData struct associated with the window and calls the WindowResizeEvent event
		glfwSetWindowSizeCallback(this->m_p_Window, [](GLFWwindow *window, int width, int height)
															{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event); });

		// Listen event window close
		// function sets the callback for closing the window, which creates a WindowCloseEvent event
		glfwSetWindowCloseCallback(this->m_p_Window, [](GLFWwindow *window)
															 {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event); });

		// Listen event keypress / keyrelease
		// function sets the callback for key presses and releases, which creates either a KeyPressedEvent or a KeyReleasedEvent event depending on the action
		glfwSetKeyCallback(this->m_p_Window, [](GLFWwindow *window, int key, int scancode, int action, int mods)
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

		// Listen event mousebutton pressed / released
		// function sets the callback for mouse button presses and releases, which creates either a MouseButtonPressedEvent or a MouseButtonReleasedEvent event depending on the action
		glfwSetMouseButtonCallback(this->m_p_Window, [](GLFWwindow *window, int button, int action, int mods)
															 {
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
		glfwSetScrollCallback(this->m_p_Window, [](GLFWwindow *window, double xOffset, double yOffset)
													{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event); });

		// Listen event mouse movement
		// function sets the callback for mouse movement, which creates a MouseMovedEvent event
		glfwSetCursorPosCallback(this->m_p_Window, [](GLFWwindow *window, double xPos, double yPos)
														 {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event); });

		// All these events are passed to the EventCallback function in the WindowData struct, which is responsible for handling these events.
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(this->m_p_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(this->m_p_Window);
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

		this->m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return this->m_Data.VSync;
	}

}
