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

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdateRuntime(Timestep ts)
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
		/*
			Search for a primary camera in a registry of entities that have both a TransformComponent and a CameraComponent.

			After this code runs, mainCamera will point to the primary camera object in the registry (if one was found), and cameraTransform will point to the transform matrix associated with that camera.
			These pointers can then be used elsewhere in the code to access and manipulate the camera's properties.
		*/
		Camera *mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			// Creates a view of the entities in the registry that have both TransformComponent and CameraComponent attached to them. A view is an object that provides an efficient way to iterate over entities that meet certain criteria.
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				// Retrieves the TransformComponent and CameraComponent attached to the current entity in the loop, using structured binding syntax. The get method of the view takes an entity ID and a list of component types and returns references to the corresponding components.
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					// If the current camera component is marked as the primary camera, this line assigns the address of its Camera member to the mainCamera pointer.
					mainCamera = &camera.Camera;
					// Assigns the address of its Transform member to the cameraTransform pointer
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		if (mainCamera)
		{
			// Begins a new rendering scene using the Renderer2D class, passing in the camera and camera transform matrix that were found earlier.
			// BeginScene sets up the rendering environment with the appropriate view and projection matrices based on the camera properties.
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			// Creates a group of entities in the registry that have both TransformComponent and SpriteRendererComponent attached to them. A group is a view that provides a way to iterate over entities that have specific combinations of components.
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				// Retrieves the TransformComponent and SpriteRendererComponent attached to the current entity in the loop, using structured binding syntax. The get method of the group takes an entity ID and a list of component types and returns references to the corresponding components.
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
			}

			// Ends the current rendering scene
			Renderer2D::EndScene();
		}
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera &camera)
	{
		Renderer2D::BeginScene(camera);

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
		}

		Renderer2D::EndScene();
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		// Creates a view of all entities in the registry that have a CameraComponent. A view is a lightweight object that allows iteration over entities with specific component types
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			// Retrieves the CameraComponent attached to the current entity in the loop using a reference. The get method of the view takes an entity ID and a component type and returns a reference to the corresponding component
			auto &cameraComponent = view.get<CameraComponent>(entity);

			// Checks if the camera has a fixed aspect ratio. If it does not, we need to adjust its viewport size to match the new screen dimensions
			if (!cameraComponent.FixedAspectRatio)
			{
				// Sets the viewport size of the camera to the new dimensions (specified by width and height)
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto &camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
			{
				return Entity{entity, this};
			}
		}
		return {};
	}

	template <typename T>
	void Scene::OnComponentAdded(Entity entity, T &component)
	{
		// static_assert(false);
	}

	template <>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent &component)
	{
	}

	template <>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent &component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template <>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent &component)
	{
	}

	template <>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent &component)
	{
	}

	template <>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent &component)
	{
	}

}