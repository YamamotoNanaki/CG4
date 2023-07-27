#include "Emitter.h"
#include "Transform.h"
#include "Component.h"
#include "TextureManager.h"
#include "GraphicsPipelineManager.h"
#include "ColorBuffer.h"

using namespace IFE;
using namespace std;

IFE::Emitter::Emitter()
{
	SetEmitterPtr(this);
}

void IFE::Emitter::Initialize()
{
	if (GetComponent<ColorBuffer>() == nullptr)AddComponent<ColorBuffer>();
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

Particle* IFE::Emitter::AddParticle(bool parentFlag)
{
	particles_.push_back(std::make_unique<Particle>());
	auto p = particles_.back().get();
	p->SetEmitter(this);
	p->Initialize(parentFlag);
	return p;
}

Particle* IFE::Emitter::AddParticle(Float3 pos)
{
	particles_.push_back(std::make_unique<Particle>());
	auto p = particles_.back().get();
	p->SetEmitter(this);
	p->Initialize(pos);
	return p;
}

Particle* IFE::Emitter::AddParticle()
{
	particles_.push_back(std::make_unique<Particle>());
	auto p = particles_.back().get();
	p->SetEmitter(this);
	p->Initialize(transform_->position_);
	return p;
}

void IFE::Emitter::Update()
{
	if (!isActive_)return;
	particles_.remove_if([&](unique_ptr<Particle>& obj) {return (obj->timer_ += IFETime::sDeltaTime_) > particleMaxTime_; });
	ComponentManager::Update();
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
	gp_->SetDrawBlendMode();
	Particle::DrawBefore();
	tex_->SetTexture(2);
	ComponentManager::Draw();
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
		SetComponent(std::move(com));
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

void IFE::Emitter::OutputScene(nlohmann::json& j)
{
	j["name"] = emitterName_;
	j["maxTime"] = particleMaxTime_;
	j["texture"] = tex_->texName_;
	uint32_t i = 0;
	for (auto& com : componentList_)
	{
		j["component"][i] = com->componentName_;
		i++;
	}
	ComponentManager::OutputScene(j);
}

void IFE::Emitter::DebugUpdate()
{
	ComponentManager::DebugUpdate();
}
#endif
void IFE::Emitter::LoadingScene(nlohmann::json& j)
{
	tex_ = TextureManager::Instance()->GetTexture(j["texture"]);
	particleMaxTime_ = j["maxTime"];
	for (auto& com : j["component"])
	{
		ComponentManager::LoadingScene(j, com);
	}
}
