#pragma once
#include "Component.h"
#include "ModelLoader.h"
#include "AddModelSettings.h"
#include <list>
#include <memory>
#include <string>

namespace IFE
{
	class ModelManager
	{
		std::list<Component*>modelList;
		ModelLoader modelLoader;

		ModelManager() {}
		ModelManager(const ModelManager&) {}
		ModelManager& operator=(const ModelManager&) {}
		~ModelManager() {}

	public:
		static ModelManager* Instance();
		static void Finalize();
		void Update();
		void Draw();
		void Add(std::string modelName, AddModelSettings modelSetting, std::string fileName = "", bool smooth = false);
		Component* GetModel(std::string modelName);
		void Reset();
#ifdef _DEBUG
		void DebugGUI();
		std::string GetModelNameGUI();
		void OutputScene();
#endif
		void LoadingScene();
	};
}
