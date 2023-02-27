#pragma once

#include "akpch.h"
#include "Arklumos/Layer.h"

#include "Arklumos/Events/ApplicationEvent.h"
#include "Arklumos/Events/KeyEvent.h"
#include "Arklumos/Events/MouseEvent.h"

#include "imgui.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"
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

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event &event);

	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent &e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent &e);
		bool OnMouseMovedEvent(MouseMovedEvent &e);
		bool OnMouseScrolledEvent(MouseScrolledEvent &e);
		bool OnKeyPressedEvent(KeyPressedEvent &e);
		bool OnKeyReleasedEvent(KeyReleasedEvent &e);
		bool OnKeyTypedEvent(KeyTypedEvent &e);
		bool OnWindowResizeEvent(WindowResizeEvent &e);

		float m_Time = 0.0f;
	};

}