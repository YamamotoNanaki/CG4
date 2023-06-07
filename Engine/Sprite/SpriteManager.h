#pragma once
#include "Sprite.h"
#include "Texture.h"
#include "IFMath.h"
#include <list>
#include <memory>

namespace IFE
{
	class SpriteManager
	{
		std::list<std::unique_ptr<Sprite>>backgroundList;
		std::list<std::unique_ptr<Sprite>>foregroundList;

		SpriteManager() {}
		SpriteManager(const SpriteManager&) {}
		SpriteManager& operator=(const SpriteManager&) {}
		~SpriteManager() {}
	public:
		static SpriteManager* Instance();
		static void Finalize();
		void SPRITEInitialize();
		void Initialize();
		void Update();
		void ForeDraw();
		void BackDraw();
		void AddInitialize(std::string spriteName, std::string textureName);
		Sprite* Add(std::string spriteName);
		Sprite* AddBackGround(std::string spriteName);
		Sprite* GetSpritePtr(std::string spriteName);
		Sprite* Instantiate(Sprite* gameObject, Float2 position, float rotation);
		Sprite* Instantiate(Sprite* gameObject, Float2 position);
		Sprite* Instantiate(Sprite* gameObject);
		void SetTexture(std::string spriteName, std::string texName);
		std::list<std::unique_ptr<Sprite>>* GetBackgroundList();
		std::list<std::unique_ptr<Sprite>>* GetForegroundList();
		void Reset();
		bool SearchName(std::string name);

#ifdef _DEBUG
		void DebugGUI();
		void OutputScene();
		void DebugUpdate();
#endif
		void LoadingScene();

	private:
		std::string GetNewName(std::string spriteName);
	};
}
