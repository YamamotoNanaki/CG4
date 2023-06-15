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
	tex_->Initialize();
	objM_->OBJInitialize();
	spriteM_->SPRITEInitialize();
	gp_.CreateBasicGraphicsPipeLine();
	light_->Initialize();
	light_->DefaultLightSetting();
	light_->SetDirLightColor(0, { 1,0,0 });
	light_->SetDirLightColor(1, { 0,1,0 });
	light_->SetDirLightColor(2, { 0,0,1 });
	sound_->Initialize();
	gui_.Initialize();
	cameraM_->Instance();
	cameraM_->Initialize();

	SceneInit();
	objM_->Initialize();
	spriteM_->Initialize();
	//particleM->Initialize();
}

void IFE::Scene::Update()
{
	SceneChange();
	gui_.StartNewFrame();
	DebugGUI();

	cameraM_->Update();
	if (debug_ && !stop_)
	{
		objM_->Update();
		spriteM_->Update();
		//particleM->Update();
	}
	else
	{
		objM_->DebugUpdate();
		spriteM_->DebugUpdate();
		//particleM->DebugUpdate();
	}
	light_->Update();
}

void IFE::Scene::Draw()
{
	Sprite::DrawBefore();
	spriteM_->BackDraw();
	gp_.SetDrawBlendMode();
	light_->Draw(3);
	objM_->Draw();
	//particleM->Draw();
	Sprite::DrawBefore();
	spriteM_->ForeDraw();
	gui_.Draw();
}
#else

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
	cameraM->Instance();
	cameraM->Initialize();

	//SceneInit();
	objM->Initialize();
	spriteM->Initialize();
	//particleM->Initialize();
}

void IFE::Scene::Update()
{
	SceneChange();
	objM->Update();
	spriteM->Update();
	//particleM->Update();
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
}
#endif

void IFE::Scene::Finalize()
{
	ModelManager::Finalize();
	//ParticleManager::Finalize();
	Sound::Finalize();
}

Scene* IFE::Scene::Instance()
{
	static Scene inst;
	return &inst;
}

void IFE::Scene::SetNextScene(const std::string& n)
{
	nextScene_ = n;
	nextFlag_ = true;
}

#include "JsonManager.h"
void IFE::Scene::SceneChange()
{
	if (nextFlag_)
	{
		nowScene_ = nextScene_;
		LoadingScene();
		nextFlag_ = false;
	}
}

void IFE::Scene::SceneInit()
{
	nowScene_ = JsonManager::Instance()->SceneInit();
	JsonManager::Instance()->SetSceneName(nowScene_);
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
	gui_.NewGUI("SceneManager", 1024);
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
	if (debug_)
	{
		if (stop_)
		{
			if (gui_.ButtonGUI("start"))
			{
				stop_ = false;
			}
		}
		else
		{
			if (gui_.ButtonGUI("Stop"))
			{
				stop_ = true;
			}
		}
		if (gui_.ButtonGUI("Debug End"))
		{
			debug_ = false;
			stop_ = false;
			LoadingScene();
			objM_->Initialize();
		}
	}
	else
	{
		if (gui_.ButtonGUI("Debug Start"))
		{
			OutputScene();
			debug_ = true;
			objM_->Initialize();
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
	gui_.EndGUI();
	objM_->DebugGUI();
	spriteM_->DebugGUI();
	modelM_->DebugGUI();
	tex_->DebugGUI();
	cameraM_->DebugGUI();
	//particleM->DebugGUI();
}
#endif