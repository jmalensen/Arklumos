// #include "../../Arklumos/src/Arklumos.h"
#include <Arklumos.h>
#include <Arklumos/Core/EntryPoint.h>

#include "Testbox2D.h"
#include "ExampleLayer.h"

class Testbox : public Arklumos::Application
{
public:
	Testbox()
	{
		// PushLayer(new ExampleLayer());
		PushLayer(new Testbox2D());
	}

	~Testbox()
	{
	}
};

Arklumos::Application *Arklumos::CreateApplication()
{
	return new Testbox();
}