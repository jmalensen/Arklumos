#pragma once

#include <glm/glm.hpp>
#include "Arklumos/Core/KeyCodes.h"
#include "Arklumos/Core/MouseCodes.h"

namespace Arklumos
{

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};

}