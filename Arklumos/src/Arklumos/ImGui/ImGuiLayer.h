#pragma once

#include "Arklumos/Core/Layer.h"

#include "Arklumos/Events/ApplicationEvent.h"
#include "Arklumos/Events/KeyEvent.h"
#include "Arklumos/Events/MouseEvent.h"

namespace Arklumos
{

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event &e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }

	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};

}