#include "Component.h"
#include "Transform.h"
#include "Object3D.h"
#include "Sprite.h"
//#include "Particle.h"

std::string IFE::Component::GetComponentName()
{
	return componentName_;
}

void IFE::Component::INITIALIZE()
{
	if (objectPtr_ != nullptr)transform_ = objectPtr_->GetComponent<Transform>();
	Initialize();
}

IFE::Component::~Component()
{
}

IFE::Component::Component(Object3D* c)
{
	objectPtr_ = c;
	transform_ = objectPtr_->GetComponent<Transform>();
}

IFE::Component::Component(Sprite* s)
{
	spritePtr_ = s;
	transform2D_ = spritePtr_->GetComponent<Transform2D>();
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

void IFE::Component::SetComponentName(const std::string& n)
{
	componentName_ = n;
}

void IFE::Component::SetComponents(Object3D* c)
{
	objectPtr_ = c;
	transform_ = objectPtr_->GetComponent<Transform>();
}

void IFE::Component::SetComponents(Sprite* s)
{
	spritePtr_ = s;
	transform2D_ = spritePtr_->GetComponent<Transform2D>();
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
	return componentDeleteFlag_;
}

IFE::Object3D* IFE::Component::GetObjectPtr()
{
	return objectPtr_;
}

IFE::Sprite* IFE::Component::GetSpritePtr()
{
	return spritePtr_;
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
		ImguiManager::Instance()->TextGUI(componentName_.c_str());
		ComponentDebugGUI();
	};
	std::function<void(void)>deleteFunc = [&]()
	{
		componentDeleteFlag_ = true;
	};
	ImguiManager::Instance()->ComponentGUI(func, deleteFunc, componentName_.c_str());
}

void IFE::Component::OutputScene(std::string object)
{
	JsonManager* j = JsonManager::Instance();
	j->OutputString("componentName", componentName_);
	OutputComponent();
	j->OutputAndMakeDirectry(object, componentName_);
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
	if (j->error_)
	{
		j->error_ = false;
		return;
	}
	componentName_ = j->InputString("componentName");
	LoadingComponent();
	j->JsonReset();
}

void IFE::Component::LoadingComponent()
{
}
