#pragma once
#include "Texture.h"
#include "ComponentManager.h"
#include "Component.h"
#include <memory>

namespace IFE
{
	class Transform;
	class Object3D :public ComponentManager
	{
		std::string objectName;
	public:
		bool isActive = true;
		bool deleteFlag = false;
		bool DrawFlag = true;
		Transform* transform = nullptr;
		Object3D* parent = nullptr;
		std::vector<Object3D*> child;
		std::string parentName;
		std::vector<std::string> childName;
		int32_t childCount = 0;

	public:
		Object3D();
		void OBJInitialize();
		void Initialize();
		void SetModel(Component* model);
		void SetComponent(Component* component);
		void SetComponentFront(Component* component);
		void OBJUpdate();
		static void DrawBefore(D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		void Draw();
		~Object3D();
		std::string GetObjectName();
		void SetObjectName(std::string objectName);
		std::vector<std::string> GetAllComponentName();
		void SetActive(bool value);
		Object3D* GetChild(int32_t index);
		void DeleteParent();
		void DeleteChild(int32_t childNum);
		void DeleteChild(std::string childName);
		void DeleteChildAll();

#ifdef _DEBUG
		void DebugGUI(bool flagdelete, bool fmove, std::string* str);
		void ComponentGUI();
		void OutputScene(bool prefab = false);
		void DebugUpdate();
#endif
		//void LoadingScene();
		void LoadChild();
	};
}
