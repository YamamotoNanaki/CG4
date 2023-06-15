#pragma once
#include "Object3D.h"
#include "IFMath.h"
#include <list>
#include <memory>

namespace IFE
{
	class ObjectManager
	{
		std::list<std::unique_ptr<Object3D>>objectList_;
		std::list<std::unique_ptr<Object3D>>prefabList_;


		ObjectManager() {}
		ObjectManager(const ObjectManager&) {}
		ObjectManager& operator=(const ObjectManager&) {}
		~ObjectManager() {}
	public:
		static ObjectManager* Instance();
		static void Finalize();
		void OBJInitialize();
		void Initialize();
		void Update();
		void Draw();
		void AddInitialize(const std::string& objectName, Component* model);
		Object3D* Add(const std::string& objectName);
		Object3D* AddPrefab(const std::string& objectName);
		Object3D* GetObjectPtr(const std::string& objectName);
		Object3D* GetPrefab(const std::string& name);
		void SetModel(const std::string& objectName, Component* model);
		std::list<std::unique_ptr<Object3D>>* GetObjList();
		void Reset();
		bool SearchName(const std::string& name);

#ifdef _DEBUG
		void DebugGUI();
		void OutputScene();
		void DebugUpdate();
#endif
		void LoadingScene();

	private:
		std::string GetNewName(const std::string& objectName);
	};
}