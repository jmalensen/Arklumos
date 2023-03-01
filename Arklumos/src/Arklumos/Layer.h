#pragma once

#include "akpch.h"
#include "Arklumos/Core.h"
#include "Arklumos/Events/Event.h"

namespace Arklumos
{

	class ARKLUMOS_API Layer
	{
	public:
		Layer(const std::string &name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event &event) {}

		inline const std::string &GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};

}