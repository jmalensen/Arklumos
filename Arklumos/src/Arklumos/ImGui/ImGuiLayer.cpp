#include "ImGuiLayer.h"

namespace Arklumos
{

	ImGuiLayer::ImGuiLayer()
			: Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		// Gets a reference to the ImGuiIO object for the current ImGui context
		ImGuiIO &io = ImGui::GetIO();

		// Sets a flag indicating that the backend (such as OpenGL or DirectX) has support for custom mouse cursors
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;

		// Sets a flag indicating that the backend has support for setting the mouse position programmatically
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// TEMPORARY: should use Arklumos key codes
		// GLFW key codes, used by the backend to handle keyboard input
		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		// Initializes the OpenGL backend for ImGui, with a specified version of OpenGL
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
	}

	void ImGuiLayer::OnUpdate()
	{
		// Gets a reference to the ImGuiIO object for the current ImGui context
		ImGuiIO &io = ImGui::GetIO();

		// Gets a reference to the global instance of the Application class
		Application &app = Application::Get();

		// Sets the display size for ImGui to match the size of the application window
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		// Gets the current time using the GLFW library
		float time = (float)glfwGetTime();

		// Calculates the time elapsed since the last frame, and sets the DeltaTime property of the ImGuiIO object.
		// If this is the first frame, it sets DeltaTime to a default value of 1/60 seconds
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;

		// Starts a new frame for the OpenGL backend
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		// Displays a demo window for ImGui, which showcases some of its features.
		// This line can be removed or commented out once the developer is familiar with how to use ImGui
		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		// Renders the ImGui draw data for the current frame
		ImGui::Render();

		// Renders the ImGui draw data using the OpenGL backend
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event &event)
	{
	}

}