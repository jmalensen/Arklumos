#include "akpch.h"
#include "Entity.h"

namespace Arklumos
{

	Entity::Entity(entt::entity handle, Scene *scene)
			: m_EntityHandle(handle), m_Scene(scene)
	{
	}

}