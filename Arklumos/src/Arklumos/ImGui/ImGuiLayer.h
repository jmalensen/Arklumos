#pragma once

#include "Arklumos/Layer.h"

#include "Arklumos/Events/ApplicationEvent.h"
#include "Arklumos/Events/KeyEvent.h"
#include "Arklumos/Events/MouseEvent.h"

namespace Arklumos
{

	class ARKLUMOS_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};

}