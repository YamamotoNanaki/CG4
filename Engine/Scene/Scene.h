#pragma once
#include <future>
#include "ObjectManager.h"
#include "GraphicsPipelineManager.h"
#include "TextureManager.h"
#include "LightManager.h"
#include "Sound.h"
#include "ModelManager.h"
#include "ImguiManager.h"
#include "CameraManager.h"
#include "SpriteManager.h"
#include "ParticleManager.h"

namespace IFE
{
	class Scene final
	{
	private:
		GraphicsPipelineManager* gp_=GraphicsPipelineManager::Instance();
		TextureManager* tex_ = TextureManager::Instance();
		LightManager* light_ = LightManager::Instance();
		Sound* sound_ = Sound::Instance();
		ObjectManager* objM_ = ObjectManager::Instance();
		ModelManager* modelM_ = ModelManager::Instance();
		CameraManager* cameraM_ = CameraManager::Instance();
		SpriteManager* spriteM_ = SpriteManager::Instance();
		ParticleManager* particleM = ParticleManager::Instance();

		std::string nowScene_;
		std::string nextScene_;
		bool nextFlag_ = false;

		std::future<void> sceneInitialize_;
		bool loadEnd_ = true;
		bool isOut_ = false;
		float transitionTimer_ = 0;
		float maxTransitionTime_ = 1;
	public:
		void Initialize();
		void Update();
		void Draw();
		void SetNextScene(const std::string& nextScene);

	public:
		static Scene* Instance();
		static void Finalize();

	private:
		Scene() {}
		Scene(const Scene&) {}
		Scene& operator=(const Scene&) {}
		~Scene() {}
		void SceneChange();
		void SceneInit();

		void SceneTransitionIn();
		void SceneTransitionOut();
		void LoadUpdate();
		void LoadDraw();

	public:
		void LoadingScene();
		void AsyncLoad();
#ifdef _DEBUG
		ImguiManager& gui_ = *ImguiManager::Instance();
		bool debug_ = false;
		bool stop_ = false;
		void OutputScene();
		void DebugGUI();
#endif
	};
}
