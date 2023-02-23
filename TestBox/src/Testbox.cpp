#include "../../Arklumos/src/Arklumos.h"
// #include <Arklumos.h>

class Testbox : public Arklumos::Application
{
public:
	Testbox()
	{
	}

	~Testbox()
	{
	}
};

Arklumos::Application *Arklumos::CreateApplication()
{
	return new Testbox();
}