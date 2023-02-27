#pragma once

#include "akpch.h"
#include "Arklumos/Layer.h"

#include "imgui.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include <GLFW/glfw3.h>
#include "Arklumos/Application.h"

namespace Arklumos
{

	class ARKLUMOS_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event &event);

	private:
		float m_Time = 0.0f;
	};

}