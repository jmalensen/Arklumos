#pragma once

#include "Arklumos/Core/Base.h"
#include "Arklumos/Scene/Scene.h"
#include "Arklumos/Scene/Entity.h"

namespace Arklumos
{

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene> &scene);

		void SetContext(const Ref<Scene> &scene);

		void OnImGuiRender();

		Entity GetSelectedEntity() const { return m_SelectionContext; }

	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};

}