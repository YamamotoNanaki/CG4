#include "Engine.h"

using namespace IFE;

int32_t WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int32_t nCmdShow)
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Engine* engine = Engine::Instance();
	engine->Initialize();
	engine->Run();
	engine->Finalize();
	//_CrtDumpMemoryLeaks();

	return 0;
	hInstance;
	hPrevInstance;
	lpCmdLine;
	nCmdShow;
}