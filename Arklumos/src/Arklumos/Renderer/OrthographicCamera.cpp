#include "akpch.h"
#include "Arklumos/Renderer/OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Arklumos
{

	/*
		Constructor which takes in four parameters left, right, bottom, and top, which define the 2D boundaries of the camera's view frustum in the x and y directions.

		Inside the constructor, the projection matrix is created using the glm::ortho function, which creates an orthographic projection matrix, and takes in the boundaries of the frustum as its parameters.
		The last two parameters specify the near and far planes of the frustum, which in this case are set to -1.0f and 1.0f respectively, indicating that all objects between these planes will be rendered.

		The view matrix is initialized to the identity matrix, which means the camera is positioned at the origin and looking down the negative z-axis.

		Finally, the view projection matrix is computed by multiplying the projection matrix with the view matrix. This matrix is used to transform objects from world space to screen space during rendering.
	*/
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
			: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		// AK_PROFILE_FUNCTION();

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	/*
		Updates the projection matrix of the camera with the specified left, right, bottom, and top coordinates.

		The function first sets the projection matrix using the glm::ortho function with the specified parameters.
		The glm::ortho function creates an orthographic projection matrix given the coordinates of the left, right, bottom, and top planes of the viewing frustum, as well as the distances to the near and far clipping planes (in this case, -1.0f and 1.0f, respectively).

		Next, the function updates the view-projection matrix by multiplying the projection matrix with the current view matrix.

		This function is useful when you want to change the projection of the camera, for example when the aspect ratio of the viewport changes.
		By updating the projection matrix, you can ensure that the geometry is still displayed correctly on the screen.
	*/
	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		// AK_PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	/*
		Recalculates the view matrix of the camera based on its position and rotation.

		The glm::translate function creates a translation matrix based on the camera's position, and the glm::rotate function creates a rotation matrix based on the camera's rotation around the z-axis.
		These matrices are then multiplied together to create a single transformation matrix.

		The glm::inverse function calculates the inverse of the transformation matrix, which effectively "undoes" the camera's transformation and puts the world back into its original position.
		This inverse matrix is used as the view matrix, which represents the camera's position and orientation in the world.

		Finally, the view-projection matrix is updated by multiplying the new view matrix with the existing projection matrix.
		The resulting matrix is used by the renderer to transform world coordinates into screen coordinates.
	*/
	void OrthographicCamera::RecalculateViewMatrix()
	{
		// AK_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
													glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}