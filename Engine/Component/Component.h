#pragma once
#include <list>
#include <string>
#include <typeinfo>

//class ADXCollider;
namespace IFE
{
	class Object3D;
	class Sprite;
	class Transform;
	class Transform2D;
	class TransformParticle;
	//class Particle;
	//class Emitter;
	class Component
	{
	public:
		Object3D* objectPtr = nullptr;
		Sprite* spritePtr = nullptr;
		//Particle* particlePtr = nullptr;
		//Emitter* emitterPtr = nullptr;
		std::string componentName;
		bool componentDeleteFlag = false;
	protected:
		virtual void Initialize() {};
	public:
		Transform* transform = nullptr;
		TransformParticle* transformParticle = nullptr;
		Transform2D* transform2D = nullptr;
		void INITIALIZE();
		virtual void Update() {};
		virtual void Draw() {};
		virtual ~Component() {};
		Component(Object3D* component);
		Component(Sprite* component);
		//Component(Particle* component);
		//Component(Emitter* component);
		Component() {}
		std::string GetComponentName();
		void SetComponentName(std::string componentName);
		void SetComponents(Object3D* components);
		void SetComponents(Sprite* components);
		//void SetComponents(Particle* components);
		//void SetComponents(Emitter* components);
		bool GetComponentDeleteFlag();
		Object3D* GetObjectPtr();
		Sprite* GetSpritePtr();
		//virtual void OnColliderHit(ADXCollider* myCol, ADXCollider* col);
		virtual void Copy(Component* component);
#ifdef _DEBUG
		//îÒêÑèß
		virtual void DebugGUI();
		//êÑèß
		virtual void ComponentDebugGUI() {};
		//îÒêÑèß
		virtual void OutputScene(std::string object);
		//êÑèß
		virtual void OutputComponent();
#endif
		//îÒêÑèß
		virtual void LoadingScene(std::string object, std::string componentName);
		//êÑèß
		virtual void LoadingComponent();
	};
}
