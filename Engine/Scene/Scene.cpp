#include "Scene.h"
#include "Debug.h"
#include "Transform.h"

using namespace IFE;

#ifdef _DEBUG
void IFE::Scene::Initialize()
{
	Sprite::StaticInitialize();
	Transform2D::StaticInitialize();
	//Emitter::StaticInitialize();
	tex->Initialize();
	objM->OBJInitialize();
	spriteM->SPRITEInitialize();
	gp.CreateBasicGraphicsPipeLine();
	light->Initialize();
	light->DefaultLightSetting();
	light->SetDirLightColor(0, { 1,0,0 });
	light->SetDirLightColor(1, { 0,1,0 });
	light->SetDirLightColor(2, { 0,0,1 });
	sound->Initialize();
	gui.Initialize();
	cameraM->Instance();
	cameraM->Initialize();

	SceneInit();
	objM->Initialize();
	spriteM->Initialize();
	//particleM->Initialize();
}

void IFE::Scene::Update()
{
	SceneChange();
	gui.StartNewFrame();
	DebugGUI();

	cameraM->Update();
	if (debug && !stop)
	{
		objM->Update();
		spriteM->Update();
		//particleM->Update();
	}
	else
	{
		objM->DebugUpdate();
		spriteM->DebugUpdate();
		//particleM->DebugUpdate();
	}
	light->Update();
}

void IFE::Scene::Draw()
{
	Sprite::DrawBefore();
	spriteM->BackDraw();
	gp.SetDrawBlendMode();
	light->Draw(3);
	objM->Draw();
	//particleM->Draw();
	Sprite::DrawBefore();
	spriteM->ForeDraw();
	gui.Draw();
}

void IFE::Scene::Finalize()
{
	ModelManager::Finalize();
	//ParticleManager::Finalize();
	Sound::Finalize();
}
#else

void IFE::Scene::Initialize()
{
	Sprite::StaticInitialize();
	Transform2D::StaticInitialize();
	Emitter::StaticInitialize();
	tex->Initialize();
	objM->OBJInitialize();
	spriteM->SPRITEInitialize();
	gp.CreateBasicGraphicsPipeLine();
	light->Initialize();
	light->DefaultLightSetting();
	light->SetDirLightColor(0, { 1,0,0 });
	light->SetDirLightColor(1, { 0,1,0 });
	light->SetDirLightColor(2, { 0,0,1 });
	sound->Initialize();
	cameraM->Instance();
	cameraM->Initialize();

	SceneInit();
	objM->Initialize();
	spriteM->Initialize();
	particleM->Initialize();
}

void IFE::Scene::Update()
{
	SceneChange();
	objM->Update();
	spriteM->Update();
	particleM->Update();
	light->Update();
}

void IFE::Scene::Draw()
{
	Sprite::DrawBefore();
	spriteM->BackDraw();
	gp.SetDrawBlendMode();
	light->Draw(3);
	objM->Draw();
	particleM->Draw();
	Sprite::DrawBefore();
	spriteM->ForeDraw();
}

void IFE::Scene::Finalize()
{
	ObjectManager::Finalize();
	SpriteManager::Finalize();
	ModelManager::Finalize();
	LightManager::Finalize();
	Sound::Finalize();
}
#endif
Scene* IFE::Scene::Instance()
{
	static Scene inst;
	return &inst;
}

void IFE::Scene::SetNextScene(std::string n)
{
	nextScene = n;
	nextFlag = true;
}

#include "JsonManager.h"
void IFE::Scene::SceneChange()
{
	if (nextFlag)
	{
		nowScene = nextScene;
		LoadingScene();
		nextFlag = false;
	}
}

void IFE::Scene::SceneInit()
{
	nowScene = JsonManager::Instance()->SceneInit();
	JsonManager::Instance()->SetSceneName(nowScene);
	//tex->TexReset();
	//objM->Reset();
	//modelM->Reset();
	//spriteM->Reset();
	//particleM->Reset();
	//tex->LoadingScene();
	//modelM->LoadingScene();
	//objM->LoadingScene();
	//spriteM->LoadingScene();
	//particleM->LoadingScene();
}

void IFE::Scene::LoadingScene()
{
	//JsonManager::Instance()->SetSceneName(nowScene);
	//objM->Reset();
	//spriteM->Reset();
	//particleM->Reset();
	//objM->LoadingScene();
	//spriteM->LoadingScene();
	//particleM->LoadingScene();
}

#ifdef _DEBUG
void IFE::Scene::OutputScene()
{
	//if (debug)return;
	//JsonManager::Instance()->MakeSceneDirectry();
	//tex->OutputScene();
	//modelM->OutputScene();
	//objM->OutputScene();
	//spriteM->OutputScene();
	//particleM->OutputScene();
}
#include "imgui.h"
void IFE::Scene::DebugGUI()
{
	gui.NewGUI("SceneManager", 1024);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("scene save"))
		{
			bool s = false;
			ImGui::MenuItem("save", "", &s);
			if (s == true)
			{
				OutputScene();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	if (debug)
	{
		if (stop)
		{
			if (gui.ButtonGUI("start"))
			{
				stop = false;
			}
		}
		else
		{
			if (gui.ButtonGUI("Stop"))
			{
				stop = true;
			}
		}
		if (gui.ButtonGUI("Debug End"))
		{
			debug = false;
			stop = false;
			LoadingScene();
			objM->Initialize();
		}
	}
	else
	{
		if (gui.ButtonGUI("Debug Start"))
		{
			OutputScene();
			debug = true;
			objM->Initialize();
		}
	}
	static char name[256];
	ImGui::InputText("scene change name", name, sizeof(name));
	if (ImGui::Button("SceneChange"))
	{
		SetNextScene(name);
	}
	ImGui::SameLine();
	if (ImGui::Button("Make this scene the initial scene"))
	{
		JsonManager::Instance()->SetInitScene();
	}
	gui.EndGUI();
	objM->DebugGUI();
	spriteM->DebugGUI();
	modelM->DebugGUI();
	tex->DebugGUI();
	cameraM->DebugGUI();
	//particleM->DebugGUI();
}
#endif