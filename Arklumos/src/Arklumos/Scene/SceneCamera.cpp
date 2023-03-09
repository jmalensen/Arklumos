#include "akpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Arklumos
{

	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	// Sets the properties of a camera with a perspective projection.
	void SceneCamera::SetPerspective(float verticalFOV, float nearClip, float farClip)
	{
		// Sets the camera's projection type to perspective. In a game engine, a camera can have either a perspective or orthographic projection, which affect how objects are rendered on the screen
		m_ProjectionType = ProjectionType::Perspective;
		// Sets the vertical field of view of the camera's perspective projection. The field of view determines how much of the scene is visible to the camera, and is typically measured in degrees
		m_PerspectiveFOV = verticalFOV;
		// Sets the near clipping plane of the camera's perspective projection. The near clipping plane is the distance from the camera at which objects start to be clipped (i.e., not visible)
		m_PerspectiveNear = nearClip;
		// Sets the far clipping plane of the camera's perspective projection. The far clipping plane is the distance from the camera at which objects become too small to be visible
		m_PerspectiveFar = farClip;
		RecalculateProjection();
	}

	// Sets the properties of a camera with a orthographic projection.
	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		// Sets the camera's projection type to orthographic. In a game engine, a camera can have either a perspective or orthographic projection, which affect how objects are rendered on the screen
		m_ProjectionType = ProjectionType::Orthographic;
		// Sets the size of the camera's orthographic projection. In an orthographic projection, objects maintain their size on screen regardless of their distance from the camera. The size parameter specifies the height of the view volume in world units
		m_OrthographicSize = size;
		// Sets the near clipping plane of the camera's orthographic projection. The near clipping plane is the distance from the camera at which objects start to be clipped (i.e., not visible)
		m_OrthographicNear = nearClip;
		// Sets the far clipping plane of the camera's orthographic projection. The far clipping plane is the distance from the camera at which objects become too small to be visible
		m_OrthographicFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		AK_CORE_ASSERT(width > 0 && height > 0);
		m_AspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		// Checks if the camera's projection type is perspective. If it is, the function calculates the projection matrix using the glm::perspective function
		if (m_ProjectionType == ProjectionType::Perspective)
		{
			// Calculates the perspective projection matrix using the camera's vertical field of view (m_PerspectiveFOV), aspect ratio (m_AspectRatio), near clipping plane (m_PerspectiveNear), and far clipping plane (m_PerspectiveFar) properties. The resulting projection matrix is assigned to the m_Projection member variable
			m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}
		else
		{
			// Calculate the dimensions of the orthographic view volume based on the camera's size (m_OrthographicSize) and aspect ratio (m_AspectRatio). The orthoLeft, orthoRight, orthoBottom, and orthoTop variables represent the left, right, bottom, and top edges of the view volume, respectively
			float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoBottom = -m_OrthographicSize * 0.5f;
			float orthoTop = m_OrthographicSize * 0.5f;

			// Calculates the orthographic projection matrix using the glm::ortho function, which takes as input the dimensions of the view volume, as well as the near and far clipping planes (m_OrthographicNear and m_OrthographicFar, respectively). The resulting projection matrix is assigned to the m_Projection member variable
			m_Projection = glm::ortho(orthoLeft, orthoRight,
																orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
		}
	}

}