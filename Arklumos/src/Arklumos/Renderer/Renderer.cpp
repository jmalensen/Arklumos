#include "akpch.h"

#include "Arklumos/Renderer/Renderer.h"
#include "Arklumos/Renderer/Renderer2D.h"

namespace Arklumos
{

	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init()
	{
		// AK_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera &camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	/*
		Handle submitting a render command to the graphics card to draw a vertex array using a shader and a transformation matrix.
		The function takes in a shared pointer to a shader, a shared pointer to a vertex array, and a 4x4 transformation matrix as arguments.

		First, the shader is bound using its Bind method. This ensures that any subsequent draw calls will use this shader for rendering.

		Next, the ViewProjectionMatrix member variable of the s_SceneData struct (which is a static member of the Renderer class) is passed to the shader using the SetMat4 method with the name "u_ViewProjection".
		This is the combined view and projection matrix for the camera used to view the scene.

		The transformation matrix passed to the function is also passed to the shader using the SetMat4 method with the name "u_Transform".
		This matrix is typically used to position and orient objects in the scene.

		The vertex array is then bound using its Bind method.

		Finally, the DrawIndexed method of the RenderCommand class is called with the vertex array as an argument.
		This method tells the graphics card to draw the vertex array using the currently bound shader and transformation matrix
	*/
	void Renderer::Submit(const Ref<Shader> &shader, const Ref<VertexArray> &vertexArray, const glm::mat4 &transform)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}