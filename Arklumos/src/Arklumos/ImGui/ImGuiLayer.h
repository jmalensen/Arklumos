#pragma once

#include "akpch.h"
#include "Arklumos/Layer.h"

#include "Arklumos/Events/ApplicationEvent.h"
#include "Arklumos/Events/KeyEvent.h"
#include "Arklumos/Events/MouseEvent.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.cpp"
#include "backends/imgui_impl_opengl3.cpp"

#include "Arklumos/Application.h"

// TODO: Temp ?
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Arklumos
{

	class ARKLUMOS_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};

}