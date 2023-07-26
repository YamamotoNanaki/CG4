#include "Particle.h"
#include "View.h"
#include "Ease.h"
#include "GraphicsAPI.h"
#include "Transform.h"
#include <cassert>

using namespace IFE;
using namespace std;

VertexBuffer<VertexPos> Particle::vb_;

IFE::Particle::Particle()
{
	SetParticlePtr(this);
}

void Particle::Initialize()
{
	AddComponent<TransformParticle>();
	ComponentManager::Initialize();
	transform_ = GetComponent<TransformParticle>();
	transform_->parent_ = emitter_->transform_;
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
	//���_�o�b�t�@�̐ݒ�
	commandList->IASetVertexBuffers(0, 1, vb_.GetVBView());
	//�`��R�}���h
	commandList->DrawInstanced((UINT)vb_.GetSize(), 1, 0, 0);
}

Particle::~Particle()
{
	ComponentManager::Finalize();
}
