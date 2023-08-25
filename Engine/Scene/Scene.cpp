#include "Scene.h"
#include "Debug.h"
#include "Transform.h"
#include "Compute.h"
#include "IFETime.h"
#include "ColorBuffer.h"
#include "Ease.h"
#include "WindowsAPI.h"

using namespace IFE;

#ifdef NDEBUG
void IFE::Scene::Initialize()
{
	Sprite::StaticInitialize();
	Transform2D::StaticInitialize();
	gp_->CreateBasicGraphicsPipeLine();
	gp_->CreateAnimGraphicsPipeLine();
	gp_->CreateBasicParticleGraphicsPipeLine();
	Emitter::StaticInitialize();
	tex_->Initialize();
	objM_->OBJInitialize();
	spriteM_->SPRITEInitialize();
	light_->Initialize();
	light_->DefaultLightSetting();
	sound_->Initialize();
	cameraM_->Initialize();

	SceneInit();

	particleM->Initialize();
}

void IFE::Scene::Update()
{
	SceneChange();
	if (loadEnd_)
	{
		objM_->Update();
		spriteM_->Update();
		particleM->Update();
		cameraM_->Update();
		light_->Update();
	}
	else
	{
		LoadUpdate();
	}
}

void IFE::Scene::PostEffectDraw()
{
	if (loadEnd_)
	{
		Sprite::DrawBefore();
		spriteM_->BackDraw();
		//objM_->DrawBackGround();
		objM_->Draw();
		particleM->Draw();
		Sprite::DrawBefore();
		spriteM_->ForeDraw();
		gui_.Draw();
	}
	else
	{
		LoadDraw();
	}
}

void IFE::Scene::Draw()
{
}
#else
void IFE::Scene::Initialize()
{
	Sprite::StaticInitialize();
	Transform2D::StaticInitialize();
	gp_->CreateBasicGraphicsPipeLine();
	gp_->CreateAnimGraphicsPipeLine();
	gp_->CreateBasicParticleGraphicsPipeLine();
	Emitter::StaticInitialize();
	tex_->Initialize();
	objM_->OBJInitialize();
	spriteM_->SPRITEInitialize();
	light_->Initialize();
	light_->DefaultLightSetting();
	sound_->Initialize();
	gui_.Initialize();
	cameraM_->Initialize();
	oldPostEffect = std::make_unique<OldPostEffect>();
	oldPostEffect->Initialize();
	SceneInit();

	particleM->Initialize();
}

void IFE::Scene::Update()
{
	SceneChange();
	if (loadEnd_)
	{
		gui_.StartNewFrame();
		DebugGUI();

		if (debug_ && !stop_)
		{
			objM_->Update();
			spriteM_->Update();
			particleM->Update();
			cameraM_->Update();
		}
		else
		{
			objM_->DebugUpdate();
			spriteM_->DebugUpdate();
			particleM->DebugUpdate();
			cameraM_->DebugUpdate();
		}
		light_->Update();
	}
	else
	{
		LoadUpdate();
	}
	PostEffectDraw();
}

void IFE::Scene::PostEffectDraw()
{
	oldPostEffect->DrawBefore();
	objM_->Draw();
	particleM->Draw();
	oldPostEffect->DrawAfter();
}

void IFE::Scene::Draw()
{
	if (loadEnd_)
	{
		Sprite::DrawBefore();
		spriteM_->BackDraw();
		oldPostEffect->Draw();
		spriteM_->ForeDraw();
		gui_.Draw();
	}
	else
	{
		LoadDraw();
	}
}
#endif

void IFE::Scene::Finalize()
{
	ModelManager::Finalize();
	ParticleManager::Finalize();
	Sound::Finalize();
#ifdef NDEBUG
#else
	JsonManager::Instance()->SetDebugScene();
#endif
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
		AsyncLoad();
	}
}

void IFE::Scene::SceneInit()
{
	nowScene_ = JsonManager::Instance()->SceneInit();
	JsonManager::Instance()->SetSceneName(nowScene_);
	tex_->TexReset();
	objM_->Reset();
	modelM_->Reset();
	spriteM_->Reset();
	particleM->Reset();
	cameraM_->Reset();
	tex_->LoadingScene();
	modelM_->LoadingScene();
	objM_->LoadingScene();
	spriteM_->LoadingScene();
	particleM->LoadingScene();
	cameraM_->LoadingScene();
	objM_->Initialize();
	spriteM_->Initialize();
	particleM->Initialize();
}

void IFE::Scene::SceneTransitionIn()
{
	if (!isOut_)
	{
		transitionTimer_ += IFETime::sDeltaTime_;
		if (transitionTimer_ < maxTransitionTime_)
		{
			//‘JˆÚ‚Ì‰‰o

			//float alpha = InOutQuad(0, 1, maxTransitionTime_, transitionTimer_);
			float y = EaseOutBounce(-(float)WindowsAPI::Instance()->winHeight_ / 2, (float)WindowsAPI::Instance()->winHeight_ / 2, maxTransitionTime_, transitionTimer_);
			auto* fade = spriteM_->GetSpritePtr("fade");
			if (!fade)
			{
				spriteM_->AddInitialize("fade", "white");
				fade = spriteM_->GetSpritePtr("fade");
				fade->GetComponent<Transform2D>()->position2D_ = Float2((float)WindowsAPI::Instance()->winWidth_ / 2, (float)WindowsAPI::Instance()->winHeight_ / 2);
				fade->GetComponent<Transform2D>()->scale2D_ = Float2(16, 9.1f);
			}
			fade->transform_->position2D_.y = y;
			fade->GetComponent<ColorBuffer>()->SetColor(0.7f, 0.7f, 0.7f, 1);
		}
		else
		{
			transitionTimer_ = 0;
			isOut_ = true;
			loadEnd_ = false;
			sound_->AllStop();
			sceneInitialize_ = std::async(std::launch::async, [this] {return LoadingScene(); });
		}
	}
}

void IFE::Scene::SceneTransitionOut()
{
	if (isOut_ && loadEnd_)
	{
		transitionTimer_ += IFETime::sDeltaTime_;
		if (transitionTimer_ < maxTransitionTime_)
		{
			//‘JˆÚ‚Ì‰‰o

			//float alpha = InOutQuad(1, 0, maxTransitionTime_, transitionTimer_);
			auto* fade = spriteM_->GetSpritePtr("fade");
			float y = EaseInBounce((float)WindowsAPI::Instance()->winHeight_ / 2, 3 * (float)WindowsAPI::Instance()->winHeight_ / 2, maxTransitionTime_, transitionTimer_);
			if (!fade)
			{
				spriteM_->AddInitialize("fade", "white");
				fade = spriteM_->GetSpritePtr("fade");
				fade->GetComponent<Transform2D>()->position2D_ = Float2((float)WindowsAPI::Instance()->winWidth_ / 2, (float)WindowsAPI::Instance()->winHeight_ / 2);
				fade->GetComponent<Transform2D>()->scale2D_ = Float2(16, 9.1f);
			}
			fade->transform_->position2D_.y = y;
			fade->GetComponent<ColorBuffer>()->SetColor(0.7f, 0.7f, 0.7f, 1);
			//fade->GetComponent<ColorBuffer>()->SetColor(0, 0, 0, alpha);
		}
		else
		{
			auto* fade = spriteM_->GetSpritePtr("fade");
			fade->spritePtr_->Destroy();
			transitionTimer_ = 0;
			isOut_ = false;
			nextFlag_ = false;
		}
	}
}

void IFE::Scene::LoadUpdate()
{
}

void IFE::Scene::LoadDraw()
{
}

void IFE::Scene::LoadingScene()
{
	JsonManager::Instance()->SetSceneName(nextScene_);
	objM_->Reset();
	spriteM_->Reset();
	particleM->Reset();
	cameraM_->Reset();
	objM_->LoadingScene();
	spriteM_->LoadingScene();
	particleM->LoadingScene();
	objM_->Initialize();
	spriteM_->Initialize();
	particleM->Initialize();
	cameraM_->LoadingScene();
	nowScene_ = nextScene_;
	loadEnd_ = true;
}

void IFE::Scene::AsyncLoad()
{
	SceneTransitionIn();
	SceneTransitionOut();
}

#ifdef NDEBUG
#else
void IFE::Scene::OutputScene()
{
	if (debug_)return;
	JsonManager::Instance()->MakeSceneDirectry();
	tex_->OutputScene();
	modelM_->OutputScene();
	objM_->OutputScene();
	spriteM_->OutputScene();
	particleM->OutputScene();
	cameraM_->OutputScene();
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
			nextScene_ = nowScene_;
			sound_->AllStop();
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
	particleM->DebugGUI();
}
#endif