#include "Testbox2D.h"
#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Testbox2D::Testbox2D()
		: Layer("Testbox2D"), m_CameraController(1280.0f / 720.0f), m_SquareColor({0.2f, 0.3f, 0.8f, 1.0f})
{
}

void Testbox2D::OnAttach()
{
	// AK_PROFILE_FUNCTION();

	m_CheckerboardTexture = Arklumos::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Testbox2D::OnDetach()
{
	// AK_PROFILE_FUNCTION();
}

void Testbox2D::OnUpdate(Arklumos::Timestep ts)
{
	// AK_PROFILE_FUNCTION();

	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	Arklumos::Renderer2D::ResetStats();
	{
		// AK_PROFILE_SCOPE("Renderer Prep");
		Arklumos::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
		Arklumos::RenderCommand::Clear();
	}

	{
		static float rotation = 0.0f;
		rotation += ts * 50.0f;

		// AK_PROFILE_SCOPE("Renderer Draw");
		Arklumos::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Arklumos::Renderer2D::DrawRotatedQuad({1.0f, 0.0f}, {0.8f, 0.8f}, -45.0f, {0.8f, 0.2f, 0.3f, 1.0f});
		Arklumos::Renderer2D::DrawQuad({-1.0f, 0.0f}, {0.8f, 0.8f}, {0.8f, 0.2f, 0.3f, 1.0f});
		Arklumos::Renderer2D::DrawQuad({0.5f, -0.5f}, {0.5f, 0.75f}, m_SquareColor);
		Arklumos::Renderer2D::DrawQuad({0.0f, 0.0f, -0.1f}, {20.0f, 20.0f}, m_CheckerboardTexture, 10.0f);
		Arklumos::Renderer2D::DrawRotatedQuad({-2.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, rotation, m_CheckerboardTexture, 20.0f);
		Arklumos::Renderer2D::EndScene();

		Arklumos::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = {(x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f};
				Arklumos::Renderer2D::DrawQuad({x, y}, {0.45f, 0.45f}, color);
			}
		}
		Arklumos::Renderer2D::EndScene();
	}
}

void Testbox2D::OnImGuiRender()
{
	// AK_PROFILE_FUNCTION();

	ImGui::Begin("Settings");

	auto stats = Arklumos::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Testbox2D::OnEvent(Arklumos::Event &e)
{
	m_CameraController.OnEvent(e);
}