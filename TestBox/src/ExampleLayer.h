#pragma once

#include "Arklumos.h"

class ExampleLayer : public Arklumos::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Arklumos::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Arklumos::Event &e) override;

private:
	Arklumos::ShaderLibrary m_ShaderLibrary;
	Arklumos::Ref<Arklumos::Shader> m_Shader;
	Arklumos::Ref<Arklumos::VertexArray> m_VertexArray;

	Arklumos::Ref<Arklumos::Shader> m_FlatColorShader;
	Arklumos::Ref<Arklumos::VertexArray> m_SquareVA;

	Arklumos::Ref<Arklumos::Texture2D> m_Texture, m_ArklumosLogoTexture;

	Arklumos::OrthographicCameraController m_CameraController;
	glm::vec3 m_SquareColor = {0.2f, 0.3f, 0.8f};
};
