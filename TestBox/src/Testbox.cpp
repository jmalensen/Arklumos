// #include "../../Arklumos/src/Arklumos.h"
#include <Arklumos.h>

#include <imgui.h>

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

	virtual void OnImGuiRender() override
	{
		// TODO: figure out how to make this part work
		// ImGui::Begin("Test");
		// ImGui::Text("Hello World");
		// ImGui::End();
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
	}

	~Testbox()
	{
	}
};

Arklumos::Application *Arklumos::CreateApplication()
{
	return new Testbox();
}