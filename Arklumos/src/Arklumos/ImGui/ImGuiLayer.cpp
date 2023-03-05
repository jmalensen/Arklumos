#include "akpch.h"
#include "Arklumos/ImGui/ImGuiLayer.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.cpp>
#include <backends/imgui_impl_opengl3.cpp>

#include "Arklumos/Core/Application.h"

// TODO: Temp ?
#include <GLFW/glfw3.h>
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
		// AK_PROFILE_FUNCTION();

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		// Gets a reference to the ImGuiIO object for the current ImGui context using ImGui::GetIO()
		ImGuiIO &io = ImGui::GetIO();

		// Suppress the "unused variable" warning that may be generated by the compiler when a variable is declared but not used
		(void)io;

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		// Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
		// io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		// io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		// ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		/*
			Retrieves the current ImGui style using ImGui::GetStyle(), which is a reference to a ImGuiStyle struct.
			Then, it checks if the ImGuiConfigFlags_ViewportsEnable configuration flag is set in the io ImGuiIO object.

			If the flag is set, it sets the WindowRounding style property to 0.0f, which means that windows will have no rounded corners.
			It also sets the alpha channel of the ImGuiCol_WindowBg color to 1.0f, which makes window backgrounds fully opaque.
			Viewports are a feature in ImGui that allows for multiple independent windows, each with their own viewport and rendering context.
			When this feature is enabled, the style changes are necessary to prevent issues with overlapping or semi-transparent windows.
		*/
		ImGuiStyle &style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		/*
			Initializes ImGui for use with GLFW and OpenGL. It starts by obtaining a reference to the application object (which should contain the GLFW window) using the Application::Get() static method.
			It then gets a pointer to the GLFW window object using the GetNativeWindow() method of the application's window object.
		*/
		Application &app = Application::Get();
		GLFWwindow *window = static_cast<GLFWwindow *>(app.GetWindow().GetNativeWindow());

		/*
			Setup Platform/Renderer bindings

			Initializes the platform and renderer bindings using the ImGui_ImplGlfw_InitForOpenGL() and ImGui_ImplOpenGL3_Init() functions from the ImGui library.
			These functions take as input the GLFW window pointer and the version of OpenGL being used respectively.

			By calling these initialization functions, ImGui is set up to work with the GLFW window and OpenGL context, allowing the application to use the ImGui UI library to render GUI elements within the application.
		*/
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	/*
		OnDetach gets called when the ImGuiLayer is detached from the application. It is responsible for shutting down the ImGui context, GLFW bindings, and OpenGL bindings.

		Here is what each line does:

		ImGui_ImplOpenGL3_Shutdown() - This shuts down the ImGui OpenGL3 renderer.
		ImGui_ImplGlfw_Shutdown() - This shuts down the ImGui GLFW bindings.
		ImGui::DestroyContext() - This destroys the ImGui context that was created during initialization.
	*/
	void ImGuiLayer::OnDetach()
	{
		// AK_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnEvent(Event &e)
	{
		if (m_BlockEvents)
		{
			ImGuiIO &io = ImGui::GetIO();
			e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	/*
		Begin is a method of the ImGuiLayer class. This method is called at the beginning of each frame to initialize the ImGui interface.

		ImGui_ImplOpenGL3_NewFrame() initializes a new frame for rendering using the OpenGL backend.
		ImGui_ImplGlfw_NewFrame() initializes a new frame for the GLFW windowing system.
		ImGui::NewFrame() initializes a new ImGui frame.

		Together, these three calls set up the ImGui interface for rendering and prepare it to receive input events. The interface is then drawn in the End() method, which is called at the end of each frame.
	*/
	void ImGuiLayer::Begin()
	{
		// AK_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	/*
		Method to end the ImGui frame, performing ImGui rendering, and updating platform-specific windows if viewports are enabled.
	*/
	void ImGuiLayer::End()
	{
		// AK_PROFILE_FUNCTION();

		// Get a reference to the ImGuiIO object for the current ImGui context
		ImGuiIO &io = ImGui::GetIO();

		// Get a reference to the current Application instance
		Application &app = Application::Get();

		// Set the display size of ImGui to match the size of the application window
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		// Render the ImGui draw data using the OpenGL3 renderer
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Check if viewports are enabled
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			// Get a handle to the current OpenGL context
			GLFWwindow *backup_current_context = glfwGetCurrentContext();

			// Update platform-specific windows
			ImGui::UpdatePlatformWindows();

			// Render platform-specific windows using the default implementation
			ImGui::RenderPlatformWindowsDefault();

			// Make the current context the previously backed up context
			glfwMakeContextCurrent(backup_current_context);
		}
	}
}