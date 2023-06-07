#include "Component.h"
#include "Transform.h"
#include "Object3D.h"
#include "Sprite.h"
//#include "Particle.h"

std::string IFE::Component::GetComponentName()
{
	return componentName;
}

void IFE::Component::INITIALIZE()
{
	if (objectPtr != nullptr)transform = objectPtr->GetComponent<Transform>();
	Initialize();
}

IFE::Component::Component(Object3D* c)
{
	objectPtr = c;
	transform = objectPtr->GetComponent<Transform>();
}

IFE::Component::Component(Sprite* s)
{
	spritePtr = s;
	transform2D = spritePtr->GetComponent<Transform2D>();
}
//
//IFE::Component::Component(Particle* p)
//{
//	particlePtr = p;
//	transformParticle = particlePtr->GetComponent<TransformParticle>();
//}
//
//IFE::Component::Component(Emitter* e)
//{
//	emitterPtr = e;
//	transformParticle = emitterPtr->GetComponent<TransformParticle>();
//}

void IFE::Component::SetComponentName(std::string n)
{
	componentName = n;
}

void IFE::Component::SetComponents(Object3D* c)
{
	objectPtr = c;
	transform = objectPtr->GetComponent<Transform>();
}

void IFE::Component::SetComponents(Sprite* s)
{
	spritePtr = s;
	transform2D = spritePtr->GetComponent<Transform2D>();
}
//
//void IFE::Component::SetComponents(Particle* p)
//{
//	particlePtr = p;
//	transformParticle = particlePtr->GetComponent<TransformParticle>();
//}
//
//void IFE::Component::SetComponents(Emitter* e)
//{
//	emitterPtr = e;
//	transformParticle = emitterPtr->GetComponent<TransformParticle>();
//}

bool IFE::Component::GetComponentDeleteFlag()
{
	return componentDeleteFlag;
}

IFE::Object3D* IFE::Component::GetObjectPtr()
{
	return objectPtr;
}

IFE::Sprite* IFE::Component::GetSpritePtr()
{
	return spritePtr;
}

//void IFE::Component::OnColliderHit(ADXCollider* myCol, ADXCollider* col)
//{
//	myCol; col;
//}

void IFE::Component::Copy(Component* c) { c; }

#include "JsonManager.h"
#ifdef _DEBUG
#include "ImguiManager.h"
void IFE::Component::DebugGUI()
{
	std::function<void(void)>func = [&]()
	{
		ImguiManager::Instance()->TextGUI(componentName.c_str());
		ComponentDebugGUI();
	};
	std::function<void(void)>deleteFunc = [&]()
	{
		componentDeleteFlag = true;
	};
	ImguiManager::Instance()->ComponentGUI(func, deleteFunc, componentName.c_str());
}

void IFE::Component::OutputScene(std::string object)
{
	JsonManager* j = JsonManager::Instance();
	j->OutputString("componentName", componentName);
	OutputComponent();
	j->OutputAndMakeDirectry(object, componentName);
}
void IFE::Component::OutputComponent()
{
}
#endif

void IFE::Component::LoadingScene(std::string object, std::string cn)
{
	JsonManager* j = JsonManager::Instance();
	std::string s = cn + "/" + object;
	j->Input(s);
	if (j->error)
	{
		j->error = false;
		return;
	}
	componentName = j->InputString("componentName");
	LoadingComponent();
	j->JsonReset();
}

void IFE::Component::LoadingComponent()
{
}
