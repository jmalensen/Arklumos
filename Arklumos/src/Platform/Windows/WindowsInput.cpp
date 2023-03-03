#include "akpch.h"
#include "WindowsInput.h"

#include "Arklumos/Core/Application.h"
#include <GLFW/glfw3.h>

namespace Arklumos
{

	Scope<Input> Input::s_Instance = CreateScope<WindowsInput>();

	/*
		Checks whether a key on the keyboard is currently pressed or not

		The function IsKeyPressedImpl takes an integer keycode as input, which represents the identifier for a particular key on the keyboard.
		It then uses the Application class to get a reference to the GLFW window that is currently active. The GetNativeWindow() function returns a pointer to the underlying GLFW window handle.
		The function then calls glfwGetKey with the window handle and the keycode as arguments to get the current state of the key. The glfwGetKey function returns GLFW_PRESS if the key is currently being pressed, and GLFW_RELEASE otherwise.
		The function checks if the state returned by glfwGetKey is either GLFW_PRESS or GLFW_REPEAT, which indicates that the key is currently being pressed.
		Finally, the function returns a boolean value indicating whether the key is currently pressed or not.
	*/
	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow *>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	/*
		Checks whether a mouse button is currently pressed or not

		The function takes an integer parameter button that represents the ID of the mouse button that needs to be checked.
		The function first gets the native window handle from the application's window using Application::Get().GetWindow().GetNativeWindow() and casts it to a GLFW window object using static_cast<GLFWwindow*>. This is required because GLFW is the underlying windowing library being used by the application.
		The function then calls glfwGetMouseButton function, passing in the GLFW window object and the button parameter. This function returns the current state of the specified mouse button (GLFW_PRESS or GLFW_RELEASE).
		Finally, the function checks if the returned state is GLFW_PRESS (button is currently pressed) and returns true, otherwise it returns false.
	*/
	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow *>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	/*
		Method GetMousePositionImpl of the WindowsInput class, which returns the current position of the mouse cursor within the window.

		The method starts by retrieving a pointer to the GLFW window associated with the application's window using Application::Get().GetWindow().GetNativeWindow(). Then, it calls the glfwGetCursorPos function, which retrieves the current position of the mouse cursor relative to the top-left corner of the window in screen coordinates. The function returns the position of the cursor as two double values, which are stored in the xpos and ypos variables.
		Finally, the method returns a std::pair object with the xpos and ypos values cast to float.
	*/
	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow *>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return {(float)xpos, (float)ypos};
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return y;
	}

}