#include "akpch.h"
#include "Arklumos/Renderer/OrthographicCameraController.h"

#include "Arklumos/Core/Input.h"
#include "Arklumos/Core/KeyCodes.h"

namespace Arklumos
{

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
			: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{
	}

	/*
		OnUpdate function for the OrthographicCameraController. This class allows for the camera to be controlled by user input, such as WASD keys to move in different directions, and QE keys to rotate the camera.

		The OnUpdate function takes a timestep (ts) as input, which is used to scale the movement and rotation based on the elapsed time since the last update.

		The first two if statements check whether the A or D keys are pressed, and adjust the camera's position left or right accordingly.
		The camera's x and y positions are updated based on the cosine and sine of the camera's rotation, respectively.

		The next two if statements check whether the W or S keys are pressed, and adjust the camera's position up or down accordingly.
		The camera's x and y positions are updated based on the negative sine and cosine of the camera's rotation, respectively.

		The m_Rotation flag is used to determine whether the camera is able to rotate or not.
		If this flag is set to true, the next block of code checks whether the Q or E keys are pressed, and adjusts the camera's rotation angle based on the elapsed time since the last update.
		The camera's rotation angle is then clamped between -180 and 180 degrees, and the camera's rotation is updated accordingly.

		Finally, the camera's position is set to the new position calculated above, and the camera's translation speed is set to the current zoom level.
	*/
	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		// AK_PROFILE_FUNCTION();

		if (Input::IsKeyPressed(Key::A))
		{
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(Key::D))
		{
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}

		if (Input::IsKeyPressed(Key::W))
		{
			m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(Key::S))
		{
			m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(Key::Q))
			{
				m_CameraRotation += m_CameraRotationSpeed * ts;
			}
			if (Input::IsKeyPressed(Key::E))
			{
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			}

			if (m_CameraRotation > 180.0f)
			{
				m_CameraRotation -= 360.0f;
			}
			else if (m_CameraRotation <= -180.0f)
			{
				m_CameraRotation += 360.0f;
			}

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	/*
		OnEvent handles incoming events by using an EventDispatcher object to dispatch specific events to their corresponding event handling functions.

		The OnEvent function takes an Event reference as its parameter. Inside the function, an EventDispatcher object is created with the given Event.
		The EventDispatcher then dispatches the event to its corresponding event handling function by calling the Dispatch function with the type of event and the function to handle that event as the argument.

		In this implementation, the OrthographicCameraController class only handles two types of events: MouseScrolledEvent and WindowResizeEvent.
		The Dispatch function is called twice with these event types and the corresponding functions to handle those events, OnMouseScrolled and OnWindowResized, respectively, passed as the argument.

		The AK_BIND_EVENT_FN macro is used to bind the event handling functions to their corresponding member functions.
		It essentially creates a lambda function that calls the corresponding member function with the current OrthographicCameraController object as its argument.
	*/
	void OrthographicCameraController::OnEvent(Event &e)
	{
		// AK_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(AK_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(AK_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	/*
		OnResize handles when the window is resized and takes in two arguments width and height that represent the new dimensions of the window.

		The method first calculates the new aspect ratio of the window by dividing the width by the height.
		This aspect ratio is then used to update the orthographic projection matrix of the camera.
		The camera's projection matrix is updated using the SetProjection method of the camera object, passing in the new bounds of the projection frustum, which are calculated as:

				Left: -aspectRatio * zoomLevel
				Right: aspectRatio * zoomLevel
				Bottom: -zoomLevel
				Top: zoomLevel

		The m_ZoomLevel member variable is used to control the zoom of the camera.
	*/
	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	/*
		OnMouseScrolled handles when the user scrolls the mouse wheel.

		The method takes in a MouseScrolledEvent reference e as input, which contains information about the amount and direction of scrolling.

		The method then updates the m_ZoomLevel member variable of the OrthographicCameraController object based on the amount of scrolling.
		It multiplies the vertical offset of the scroll by 0.25 and subtracts it from the m_ZoomLevel variable. It then ensures that the zoom level is not less than 0.25, which is the minimum zoom level.

		Finally, the method updates the projection matrix of the OrthographicCamera object associated with the OrthographicCameraController object using the new m_ZoomLevel value and the aspect ratio of the viewport, which is represented by the m_AspectRatio member variable. The left and right coordinates of the orthographic projection are calculated as -m_AspectRatio * m_ZoomLevel and m_AspectRatio * m_ZoomLevel, respectively, and the top and bottom coordinates are calculated as -m_ZoomLevel and m_ZoomLevel, respectively.
	*/
	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent &e)
	{
		// AK_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent &e)
	{
		// AK_PROFILE_FUNCTION();

		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}

}