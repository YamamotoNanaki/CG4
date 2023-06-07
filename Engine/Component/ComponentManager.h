#pragma once
#include "Component.h"
#include "Debug.h"
#include <list>
#include <vector>
#include <string>
#include <typeinfo>

class ADXCollider;
namespace IFE
{
	class Object3D;
	class Particle;
	class Sprite;
	class Emitter;
	class ComponentManagerEmitter
	{
		std::list<Component*> componentList;

	private:
		size_t colNum = 0;

	public:
		Emitter* emitterPtr = nullptr;

	public:
		template <class T>
		void AddComponent();
		template <class T>
		void AddComponent(Component* ptr);
		template <class T>
		void AddComponentBack();
		template <class T>
		void AddComponentBack(Component* ptr);
		template <class T>
		void AddComponent(Component* ptr, Emitter* components);
		template <class T>
		void AddComponentBack(Component* ptr, Emitter* components);
		template <class T>
		T* GetComponent();
		template <class T>
		void DeleteComponent();
		template <class T>
		void RemoveComponent();
		void Initialize();
		void Update();
		void Draw();
		void Finalize();
		void SetEmitterPtr(Emitter* emitterPtr);
		std::vector<std::string> GetAllComponentName();
#ifdef _DEBUG
		void DebugGUI();
		void OutputScene(std::string emitter);
		void DebugUpdate();
#endif
		void LoadingScene(std::string emitter, std::string componentName);

	private:
		std::string SetName(std::string name);
	};
	template<class T>
	inline void ComponentManagerEmitter::AddComponent()
	{
		T* ptr = new T(emitterPtr);
		ptr->SetComponentName(SetName(typeid(T).name()));
		ptr->INITIALIZE();
		componentList.push_front(ptr);
	}
	template<class T>
	inline void ComponentManagerEmitter::AddComponent(Component* ptr)
	{
		ptr->SetComponents(emitterPtr);
		ptr->INITIALIZE();
		componentList.push_front(ptr);
	}
	template<class T>
	inline void ComponentManagerEmitter::AddComponentBack()
	{
		T* ptr = new T(emitterPtr);
		ptr->SetComponentName(SetName(typeid(T).name()));
		ptr->INITIALIZE();
		componentList.push_back(ptr);
	}
	template<class T>
	inline void ComponentManagerEmitter::AddComponentBack(Component* ptr)
	{
		ptr->SetComponents(emitterPtr);
		ptr->INITIALIZE();
		componentList.push_back(ptr);
	}
	template<class T>
	inline void ComponentManagerEmitter::AddComponent(Component* ptr, Emitter* components)
	{
		if (components)ptr->SetComponents(components);
		componentList.push_front(ptr);
	}
	template<class T>
	inline void ComponentManagerEmitter::AddComponentBack(Component* ptr, Emitter* components)
	{
		if (components)ptr->SetComponents(components);
		componentList.push_back(ptr);
	}
	template<class T>
	inline T* ComponentManagerEmitter::GetComponent()
	{
		for (auto& itr : componentList)
		{
			T* ptr = dynamic_cast<T*>(itr);
			if (ptr)return ptr;
		}
		return nullptr;
	}
	template<class T>
	inline void ComponentManagerEmitter::DeleteComponent()
	{
		for (auto& itr : componentList)
		{
			T* ptr = dynamic_cast<T*>(itr);
			if (ptr)
			{
				delete ptr;
				componentList.remove(itr);
			}
		}
	}
	template<class T>
	inline void ComponentManagerEmitter::RemoveComponent()
	{
		for (auto& itr : componentList)
		{
			T* ptr = dynamic_cast<T*>(itr);
			if (ptr)
			{
				componentList.remove(itr);
			}
		}
	}
	class ComponentManagerParticle
	{
		std::list<Component*> componentList;

	private:
		size_t colNum = 0;

	public:
		Particle* particlePtr = nullptr;

	public:
		template <class T>
		void AddComponent();
		template <class T>
		void AddComponent(Component* ptr);
		template <class T>
		void AddComponentBack();
		template <class T>
		void AddComponentBack(Component* ptr);
		template <class T>
		void AddComponent(Component* ptr, Particle* components);
		template <class T>
		void AddComponentBack(Component* ptr, Particle* components);
		template <class T>
		T* GetComponent();
		template <class T>
		void DeleteComponent();
		template <class T>
		void RemoveComponent();
		void Initialize();
		void Update();
		void Draw();
		void Finalize();
		void SetParticlePtr(Particle* particlePtr);
		std::vector<std::string> GetAllComponentName();
#ifdef _DEBUG
		void DebugGUI();
		void OutputScene(std::string particle);
		void DebugUpdate();
#endif
		void LoadingScene(std::string particle, std::string componentName);

	private:
		std::string SetName(std::string name);
	};
	template<class T>
	inline void ComponentManagerParticle::AddComponent()
	{
		T* ptr = new T(particlePtr);
		ptr->SetComponentName(SetName(typeid(T).name()));
		ptr->INITIALIZE();
		componentList.push_front(ptr);
	}
	template<class T>
	inline void ComponentManagerParticle::AddComponent(Component* ptr)
	{
		ptr->SetComponents(particlePtr);
		ptr->INITIALIZE();
		componentList.push_front(ptr);
	}
	template<class T>
	inline void ComponentManagerParticle::AddComponentBack()
	{
		T* ptr = new T(particlePtr);
		ptr->SetComponentName(SetName(typeid(T).name()));
		ptr->INITIALIZE();
		componentList.push_back(ptr);
	}
	template<class T>
	inline void ComponentManagerParticle::AddComponentBack(Component* ptr)
	{
		ptr->SetComponents(particlePtr);
		ptr->INITIALIZE();
		componentList.push_back(ptr);
	}
	template<class T>
	inline void ComponentManagerParticle::AddComponent(Component* ptr, Particle* components)
	{
		if (components)ptr->SetComponents(components);
		componentList.push_front(ptr);
	}
	template<class T>
	inline void ComponentManagerParticle::AddComponentBack(Component* ptr, Particle* components)
	{
		if (components)ptr->SetComponents(components);
		componentList.push_back(ptr);
	}
	template<class T>
	inline T* ComponentManagerParticle::GetComponent()
	{
		for (auto& itr : componentList)
		{
			T* ptr = dynamic_cast<T*>(itr);
			if (ptr)return ptr;
		}
		return nullptr;
	}
	template<class T>
	inline void ComponentManagerParticle::DeleteComponent()
	{
		for (auto& itr : componentList)
		{
			T* ptr = dynamic_cast<T*>(itr);
			if (ptr)
			{
				delete ptr;
				componentList.remove(itr);
			}
		}
	}
	template<class T>
	inline void ComponentManagerParticle::RemoveComponent()
	{
		for (auto& itr : componentList)
		{
			T* ptr = dynamic_cast<T*>(itr);
			if (ptr)
			{
				componentList.remove(itr);
			}
		}
	}
	class ComponentManager2D
	{
		std::list<Component*> componentList;

	private:
		size_t colNum = 0;

	public:
		Sprite* spritePtr = nullptr;

	public:
		template <class T>
		void AddComponent();
		template <class T>
		void AddComponent(Component* ptr);
		template <class T>
		void AddComponentBack();
		template <class T>
		void AddComponentBack(Component* ptr);
		template <class T>
		void AddComponent(Component* ptr, Sprite* components);
		template <class T>
		void AddComponentBack(Component* ptr, Sprite* components);
		template <class T>
		T* GetComponent();
		template <class T>
		void DeleteComponent();
		template <class T>
		void RemoveComponent();
		void Initialize();
		void Update();
		void Draw();
		void Finalize();
		void SetSpritePtr(Sprite* spritePtr);
		std::vector<std::string> GetAllComponentName();
#ifdef _DEBUG
		void DebugGUI();
		void OutputScene(std::string sprite);
		void DebugUpdate();
#endif
		void LoadingScene(std::string sprite, std::string componentName);

	private:
		std::string SetName(std::string name);
	};
	template<class T>
	inline void ComponentManager2D::AddComponent()
	{
		T* ptr = new T(spritePtr);
		ptr->SetComponentName(SetName(typeid(T).name()));
		ptr->INITIALIZE();
		componentList.push_front(ptr);
	}
	template<class T>
	inline void ComponentManager2D::AddComponent(Component* ptr)
	{
		ptr->SetComponents(spritePtr);
		ptr->INITIALIZE();
		componentList.push_front(ptr);
	}
	template<class T>
	inline void ComponentManager2D::AddComponentBack()
	{
		T* ptr = new T(spritePtr);
		ptr->SetComponentName(SetName(typeid(T).name()));
		ptr->INITIALIZE();
		componentList.push_back(ptr);
	}
	template<class T>
	inline void ComponentManager2D::AddComponentBack(Component* ptr)
	{
		ptr->SetComponents(spritePtr);
		ptr->INITIALIZE();
		componentList.push_back(ptr);
	}
	template<class T>
	inline void ComponentManager2D::AddComponent(Component* ptr, Sprite* components)
	{
		if (components)ptr->SetComponents(components);
		componentList.push_front(ptr);
	}
	template<class T>
	inline void ComponentManager2D::AddComponentBack(Component* ptr, Sprite* components)
	{
		if (components)ptr->SetComponents(components);
		componentList.push_back(ptr);
	}
	template<class T>
	inline T* ComponentManager2D::GetComponent()
	{
		for (auto& itr : componentList)
		{
			T* ptr = dynamic_cast<T*>(itr);
			if (ptr)return ptr;
		}
		return nullptr;
	}
	template<class T>
	inline void ComponentManager2D::DeleteComponent()
	{
		for (auto& itr : componentList)
		{
			T* ptr = dynamic_cast<T*>(itr);
			if (ptr)
			{
				delete ptr;
				componentList.remove(itr);
			}
		}
	}
	template<class T>
	inline void ComponentManager2D::RemoveComponent()
	{
		for (auto& itr : componentList)
		{
			T* ptr = dynamic_cast<T*>(itr);
			if (ptr)
			{
				componentList.remove(itr);
			}
		}
	}
	class ComponentManager
	{
	public:
		std::list<Component*> componentList;

	private:
		size_t colNum = 0;

	public:
		Object3D* objectPtr = nullptr;

	public:
		template <class T>
		void AddComponent();
		template <class T>
		void AddComponent(Component* ptr);
		template <class T>
		void AddComponentBack();
		template <class T>
		void AddComponentBack(Component* ptr);
		template <class T>
		void AddComponent(Component* ptr, Object3D* components);
		template <class T>
		void AddComponentBack(Component* ptr, Object3D* components);
		template <class T>
		T* GetComponent();
		template <class T>
		void DeleteComponent();
		template <class T>
		void RemoveComponent();
		void Initialize();
		void Update();
		void Draw();
		void Finalize();
		void SetObjectPtr(Object3D* objectPtr);
		//void OnColliderHit(ADXCollider* myCol, ADXCollider* col);
		std::vector<std::string> GetAllComponentName();
#ifdef _DEBUG
		void DebugGUI();
		void OutputScene(std::string object3D);
		void DebugUpdate();
#endif
		void LoadingScene(std::string object3D, std::string componentName);

	private:
		std::string SetName(std::string name);
	};
	template<class T>
	inline void ComponentManager::AddComponent()
	{
		T* ptr = new T(objectPtr);
		ptr->SetComponentName(SetName(typeid(T).name()));
		ptr->INITIALIZE();
		componentList.push_front(ptr);
	}
	template<class T>
	inline void ComponentManager::AddComponent(Component* ptr)
	{
		ptr->SetComponents(objectPtr);
		ptr->INITIALIZE();
		componentList.push_front(ptr);
	}
	template<class T>
	inline void ComponentManager::AddComponentBack()
	{
		T* ptr = new T(objectPtr);
		ptr->SetComponentName(SetName(typeid(T).name()));
		ptr->INITIALIZE();
		componentList.push_back(ptr);
	}
	template<class T>
	inline void ComponentManager::AddComponentBack(Component* ptr)
	{
		ptr->SetComponents(objectPtr);
		ptr->INITIALIZE();
		componentList.push_back(ptr);
	}
	template<class T>
	inline void ComponentManager::AddComponent(Component* ptr, Object3D* components)
	{
		if (components)ptr->SetComponents(components);
		componentList.push_front(ptr);
	}
	template<class T>
	inline void ComponentManager::AddComponentBack(Component* ptr, Object3D* components)
	{
		if (components)ptr->SetComponents(components);
		componentList.push_back(ptr);
	}
	template<class T>
	inline T* ComponentManager::GetComponent()
	{
		for (auto& itr : componentList)
		{
			T* ptr = dynamic_cast<T*>(itr);
			if (ptr)return ptr;
		}
		return nullptr;
	}
	template<class T>
	inline void ComponentManager::DeleteComponent()
	{
		for (auto& itr : componentList)
		{
			T* ptr = dynamic_cast<T*>(itr);
			if (ptr)
			{
				delete ptr;
				componentList.remove(itr);
			}
		}
	}
	template<class T>
	inline void ComponentManager::RemoveComponent()
	{
		for (auto& itr : componentList)
		{
			T* ptr = dynamic_cast<T*>(itr);
			if (ptr)
			{
				componentList.remove(itr);
			}
		}
	}
}
