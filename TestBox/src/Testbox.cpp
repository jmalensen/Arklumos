#include "../../Arklumos/src/Arklumos.h"
//#include <Arklumos.h>

class ExampleLayer : public Arklumos::Layer
{
public:
	ExampleLayer()
			: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		if (Arklumos::Input::IsKeyPressed(AK_KEY_TAB))
		{
			AK_TRACE("Tab key is pressed (poll)!");
		}
	}

	void OnEvent(Arklumos::Event &event) override
	{
		if (event.GetEventType() == Arklumos::EventType::KeyPressed)
		{
			Arklumos::KeyPressedEvent &e = (Arklumos::KeyPressedEvent &)event;
			if (e.GetKeyCode() == AK_KEY_TAB)
			{
				AK_TRACE("Tab key is pressed (event)!");
			}
			AK_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};

class Testbox : public Arklumos::Application
{
public:
	Testbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Arklumos::ImGuiLayer());
	}

	~Testbox()
	{
	}
};

Arklumos::Application *Arklumos::CreateApplication()
{
	return new Testbox();
}