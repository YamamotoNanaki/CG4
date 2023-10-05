#include "Particle.h"
#include "View.h"
#include "Ease.h"
#include "GraphicsAPI.h"
#include "Transform.h"
#include "Warning.h"
WarningDisabled
#include <cassert>
WarningDisabledPop


using namespace IFE;
using namespace std;

VertexBuffer<VertexPos> Particle::vb_;

IFE::Particle::Particle()
{
	SetParticlePtr(this);
}

void IFE::Particle::Initialize(Float3 position)
{
	AddComponent<TransformParticle>();
	ComponentManager::Initialize();
	transform_ = GetComponent<TransformParticle>();
	transform_->position_ = position;
}

void IFE::Particle::Initialize(bool parentFlag)
{
	AddComponent<TransformParticle>();
	ComponentManager::Initialize();
	transform_ = GetComponent<TransformParticle>();
	transform_->parent_ = emitter_->transform_;
	parentFlag;
}

void IFE::Particle::SetComponent(std::unique_ptr<Component> component)
{
	AddComponentBack<Component>(std::move(component));
}
void IFE::Particle::SetComponentFront(std::unique_ptr<Component> component)
{
	AddComponent<Component>(std::move(component));
}

void IFE::Particle::StaticInitialize()
{
	VertexPos vp[1];
	vb_.SetVerticle(vp, _countof(vp));
	vb_.Initialize();
}

void IFE::Particle::Update()
{
	ComponentManager::Update();
}

void IFE::Particle::DrawBefore()
{
	ID3D12GraphicsCommandList* commandList = GraphicsAPI::Instance()->GetCmdList();
	//commandList->SetGraphicsRootSignature(root);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void IFE::Particle::SetEmitter(Emitter* emitter)
{
	emitter_ = emitter;
}

void Particle::Draw()
{
	ComponentManager::Draw();
	ID3D12GraphicsCommandList* commandList = GraphicsAPI::Instance()->GetCmdList();
	//頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, vb_.GetVBView());
	//描画コマンド
	commandList->DrawInstanced((UINT)vb_.GetSize(), 1, 0, 0);
}

Particle::~Particle()
{
	ComponentManager::Finalize();
}
