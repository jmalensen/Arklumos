#include <Arklumos.h>
#include <Arklumos/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Arklumos
{

	class Arklusis : public Application
	{
	public:
		Arklusis()
				: Application("Arklusis the editor")
		{
			PushLayer(new EditorLayer());
		}

		~Arklusis()
		{
		}
	};

	Application *CreateApplication()
	{
		return new Arklusis();
	}

}