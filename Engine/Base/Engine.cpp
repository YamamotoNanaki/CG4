#include "Engine.h"
#include "Input.h"
#include "Scene.h"
#include "Rand.h"

using namespace IFE;

Engine* IFE::Engine::Instance()
{
	static Engine inst;
	return &inst;
}

void IFE::Engine::Initialize()
{
	window_->Initialize(1280, 720);
	gapi_->Initialize(*WindowsAPI::Instance()->GetHWnd(), 1280, 720);
	Input::Initalize();
	IFERand::Initialize();
}

void IFE::Engine::Run()
{
	Scene* scene = Scene::Instance();
	scene->Initialize();
	while (true)
	{
		Input::Update();
		scene->Update();
		gapi_->DrawBefore();
		gapi_->DrawSetViewport();
		scene->Draw();
		gapi_->DrawAfter();

		if (window_->Message() || Input::KeyDown(Key_Esc))
		{
#ifdef _DEBUG
			scene->OutputScene();
#endif
			return;
		}
	}
}

void IFE::Engine::Finalize()
{
	window_->Finalize();
	gapi_->Finalize();
}
