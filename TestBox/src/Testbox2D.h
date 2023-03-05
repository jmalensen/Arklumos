#pragma once

#include "Arklumos.h"

class Testbox2D : public Arklumos::Layer
{
public:
	Testbox2D();
	virtual ~Testbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Arklumos::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Arklumos::Event &e) override;

private:
	Arklumos::OrthographicCameraController m_CameraController;

	// Temp
	Arklumos::Ref<Arklumos::VertexArray> m_SquareVA;
	Arklumos::Ref<Arklumos::Shader> m_FlatColorShader;

	Arklumos::Ref<Arklumos::Texture2D> m_CheckerboardTexture;

	glm::vec4 m_SquareColor = {0.2f, 0.3f, 0.8f, 1.0f};
};