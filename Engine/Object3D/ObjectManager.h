#pragma once
#include "Object3D.h"
#include "IFMath.h"
#include <list>
#include <memory>

namespace IFE
{
	class ObjectManager
	{
		std::list<std::unique_ptr<Object3D>>objectList;
		std::list<std::unique_ptr<Object3D>>prefabList;


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
		void AddInitialize(std::string objectName, Component* model);
		Object3D* Add(std::string objectName);
		Object3D* AddPrefab(std::string objectName);
		Object3D* GetObjectPtr(std::string objectName);
		Object3D* AddFromPrefab(std::string objectName, std::string prefabName);
		Object3D* AddFromPrefab(std::string prefabName);
		Object3D* Instantiate(Object3D* gameObject, Float3 position, Quaternion rotation);
		Object3D* Instantiate(Object3D* gameObject, Float3 position);
		Object3D* Instantiate(Object3D* gameObject);
		Object3D* GetPrefab(std::string name);
		void SetModel(std::string objectName, Component* model);
		std::list<std::unique_ptr<Object3D>>* GetObjList();
		void Reset();
		bool SearchName(std::string name);

#ifdef _DEBUG
		void DebugGUI();
		void OutputScene();
		void DebugUpdate();
#endif
		void LoadingScene();

	private:
		std::string GetNewName(std::string objectName);
	};
}