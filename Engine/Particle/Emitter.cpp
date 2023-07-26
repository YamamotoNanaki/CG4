#include "Emitter.h"
#include "Transform.h"
#include "Component.h"
#include "TextureManager.h"
#include "GraphicsPipelineManager.h"

using namespace IFE;
using namespace std;

IFE::Emitter::Emitter()
{
	SetEmitterPtr(this);
}

void IFE::Emitter::Initialize()
{
	if (GetComponent<TransformParticle>() == nullptr)AddComponent<TransformParticle>();
	transform_ = GetComponent<TransformParticle>();
	gp_ = GraphicsPipelineManager::Instance()->GetGraphicsPipeline("ParticleNormal");
}

void IFE::Emitter::StaticInitialize()
{
	Particle::StaticInitialize();
}

void IFE::Emitter::SetComponent(std::unique_ptr<Component> component)
{
	AddComponentBack<Component>(std::move(component));
}

void IFE::Emitter::SetComponentFront(std::unique_ptr<Component> component)
{
	AddComponent<Component>(std::move(component));
}

void IFE::Emitter::SetTexture(const std::string& texName)
{
	this->tex_ = TextureManager::Instance()->GetTexture(texName);
}

void IFE::Emitter::Update()
{
	if (!isActive_)return;
	ComponentManager::Update();
	particles_.remove_if([](unique_ptr<Particle>& obj) {return obj->deleteFlag_; });
	for (std::unique_ptr<Particle>& itr : particles_)
	{
		itr->Update();
	}
}

//void IFE::Emitter::DrawBefore()
//{
//}

void IFE::Emitter::Draw()
{
	if (!isActive_)return;
	if (!DrawFlag_)return;
	Particle::DrawBefore();
	gp_->SetDrawBlendMode();
	tex_->SetTexture(2);
	for (std::unique_ptr<Particle>& itr : particles_)
	{
		itr->Draw();
	}
}

IFE::Emitter::~Emitter()
{
	ComponentManager::Finalize();
	particles_.clear();
}

#ifdef _DEBUG
#include "ImguiManager.h"
void IFE::Emitter::DebugGUI(bool flagdelete)
{
	if (ImguiManager::Instance()->EmitterGUI(emitterName_, flagdelete))
	{
		deleteFlag_ = true;
	}
}

void IFE::Emitter::ComponentGUI()
{
	std::function<void(std::unique_ptr<Component>)> addFunc = [&](std::unique_ptr<Component> com)
	{
		SetComponentFront(std::move(com));
	};
	std::function<void(void)>f = [&]()
	{
		ComponentManager::DebugGUI();
	};
	std::function<void(const std::string&)>texFunc = [&](const std::string& name)
	{
		tex_ = TextureManager::Instance()->GetTexture(name);
	};
	ImguiManager::Instance()->ComponentGUI2D(emitterName_, f, addFunc);
}

void IFE::Emitter::OutputScene()
{
}

void IFE::Emitter::DebugUpdate()
{
	ComponentManager::DebugUpdate();
}
#endif
