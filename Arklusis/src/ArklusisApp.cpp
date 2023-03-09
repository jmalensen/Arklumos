#include <Arklumos.h>
#include <Arklumos/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Arklumos
{

	class Arklusis : public Application
	{
	public:
		Arklusis(ApplicationCommandLineArgs args)
				: Application("Arklusis the editor", args)
		{
			PushLayer(new EditorLayer());
		}

		~Arklusis()
		{
		}
	};

	Application *CreateApplication(ApplicationCommandLineArgs args)
	{
		return new Arklusis(args);
	}

}