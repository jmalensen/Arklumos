#include "akpch.h"
#include "Scene.h"

#include "Components.h"
#include "Arklumos/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

#include "Entity.h"

namespace Arklumos
{

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string &name)
	{
		Entity entity = {m_Registry.create(), this};
		entity.AddComponent<TransformComponent>();
		auto &tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		// Update scripts
		{
			/*
				warning: implicit capture of 'this' via '[=]' is deprecated in C++20 [-Wdeprecated]
				m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto &nsc)

				The warning message you are seeing is indicating that the lambda function is capturing this implicitly through the use of the capture-all [=] capture clause.
				This means that the lambda function is capturing all variables by value, including this, which can lead to issues if the lifetime of the lambda outlasts the lifetime of the object that this refers to.

				To fix this warning, you can explicitly capture only the variables that are needed by the lambda function. In this case, you can capture nsc by reference using the capture clause &, like this: [&] in place of [=]
				This way, nsc will be captured by reference, but this will not be captured implicitly.
			*/
			m_Registry.view<NativeScriptComponent>().each([&](auto entity, auto &nsc)
																										{
																											// TODO: Move to Scene::OnScenePlay
																											if (!nsc.Instance)
																											{
																												nsc.Instance = nsc.InstantiateScript();
																												nsc.Instance->m_Entity = Entity{entity, this};

																												nsc.Instance->OnCreate();
																											}

																											nsc.Instance->OnUpdate(ts); });
		}

		// Render 2D
		Camera *mainCamera = nullptr;
		glm::mat4 *cameraTransform = nullptr;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = &transform.Transform;
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				// Need to add const otherwise we have this error:
				// cannot bind non-const lvalue reference of type 'std::tuple<Arklumos::TransformComponent&, Arklumos::SpriteRendererComponent&>&' to an rvalue of type 'std::tuple<Arklumos::TransformComponent&, Arklumos::SpriteRendererComponent&>'

				// Use a const lvalue reference
				//  const auto &[transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				// If you need to modify the values of the tuple elements, you can copy the tuple instead of binding a reference to it
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(transform, sprite.Color);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto &cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
			{
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}

}