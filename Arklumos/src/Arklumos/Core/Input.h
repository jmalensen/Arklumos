#pragma once

#include "Arklumos/Core/Base.h"
#include "Arklumos/Core/KeyCodes.h"
#include "Arklumos/Core/MouseCodes.h"

namespace Arklumos
{

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};

}