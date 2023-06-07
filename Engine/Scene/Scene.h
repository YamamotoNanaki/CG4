#pragma once
#include "ObjectManager.h"
#include "GraphicsPipeline.h"
#include "TextureManager.h"
#include "LightManager.h"
#include "Sound.h"
#include "ModelManager.h"
#include "ImguiManager.h"
#include "CameraManager.h"
#include "SpriteManager.h"
//#include "ParticleManager.h"

namespace IFE
{
	class Scene final
	{
	private:
		GraphicsPipeline gp;
		TextureManager* tex = TextureManager::Instance();
		LightManager* light = LightManager::Instance();
		Sound* sound = Sound::Instance();
		ObjectManager* objM = ObjectManager::Instance();
		ModelManager* modelM = ModelManager::Instance();
		CameraManager* cameraM = CameraManager::Instance();
		SpriteManager* spriteM = SpriteManager::Instance();
		//ParticleManager* particleM = ParticleManager::Instance();

		std::string nowScene;
		std::string nextScene;
		bool nextFlag = false;
	public:
		void Initialize();
		void Update();
		void Draw();
		void SetNextScene(std::string nextScene);

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

	public:
		void LoadingScene();
#ifdef _DEBUG
		ImguiManager& gui = *ImguiManager::Instance();
		bool debug = false;
		bool stop = false;
		void OutputScene();
		void DebugGUI();
#endif
	};
}
