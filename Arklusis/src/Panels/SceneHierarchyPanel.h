#pragma once

#include "Arklumos/Core/Base.h"
#include "Arklumos/Core/Log.h"
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

	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};

}