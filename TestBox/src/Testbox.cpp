// #include "../../Arklumos/src/Arklumos.h"
#include <Arklumos.h>

class ExampleLayer : public Arklumos::Layer
{
public:
	ExampleLayer()
			: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		AK_INFO("ExampleLayer::Update");
	}

	void OnEvent(Arklumos::Event &event) override
	{
		AK_TRACE("{0}", event);
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