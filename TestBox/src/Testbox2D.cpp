#include "Testbox2D.h"
#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Testbox2D::Testbox2D()
		: Layer("Testbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Testbox2D::OnAttach()
{
	AK_PROFILE_FUNCTION();

	m_CheckerboardTexture = Arklumos::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Testbox2D::OnDetach()
{
	AK_PROFILE_FUNCTION();
}

void Testbox2D::OnUpdate(Arklumos::Timestep ts)
{
	AK_PROFILE_FUNCTION();

	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	{
		AK_PROFILE_SCOPE("Renderer Prep");
		Arklumos::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
		Arklumos::RenderCommand::Clear();
	}

	{
		AK_PROFILE_SCOPE("Renderer Draw");
		Arklumos::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Arklumos::Renderer2D::DrawQuad({-1.0f, 0.0f}, {0.8f, 0.8f}, {0.8f, 0.2f, 0.3f, 1.0f});
		Arklumos::Renderer2D::DrawQuad({0.5f, -0.5f}, {0.5f, 0.75f}, {0.2f, 0.3f, 0.8f, 1.0f});
		Arklumos::Renderer2D::DrawQuad({0.0f, 0.0f, -0.1f}, {10.0f, 10.0f}, m_CheckerboardTexture);
		Arklumos::Renderer2D::EndScene();
	}
}

void Testbox2D::OnImGuiRender()
{
	AK_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::End();
}

void Testbox2D::OnEvent(Arklumos::Event &e)
{
	m_CameraController.OnEvent(e);
}