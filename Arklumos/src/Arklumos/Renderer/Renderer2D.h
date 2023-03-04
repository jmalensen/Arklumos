#pragma once

#include "Arklumos/Renderer/OrthographicCamera.h"

#include "Arklumos/Renderer/Texture.h"

namespace Arklumos
{

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera &camera);
		static void EndScene();

		// Primitives
		static void DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color);
		static void DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color);
		static void DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Ref<Texture2D> &texture);
		static void DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const Ref<Texture2D> &texture);
	};

}