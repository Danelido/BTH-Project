#define STB_IMAGE_IMPLEMENTATION
#include "Application.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Application* app = new Application();
	app->createWindow("BTH Project", 1280, 720);
	app->run();
	delete app;
	return 0;
}