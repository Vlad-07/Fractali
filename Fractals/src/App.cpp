#include <Eis.h>
#include <Eis/Core/EntryPoint.h>

#include <imgui.h>

#include "MainLayer.h"


class App : public Eis::Application
{
public:
	App()
	{
		PushLayer(new MainLayer());
	}

	virtual ~App() = default;
};

Eis::Application* Eis::CreateApplication()
{
	return new App();
}