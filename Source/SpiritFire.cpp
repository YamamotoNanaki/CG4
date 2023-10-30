#include "SpiritFire.h"
#include "Emitter.h"
#include "ColorBuffer.h"
#include "Ease.h"
#include "Rand.h"
#include "Object3D.h"
#include "Transform.h"
#include "Enemy.h"

void SpiritFire::Update()
{
	for (auto& p : emitterPtr_->particles_)
	{
		float b = IFE::OutQuad(0.6f, 0.1f, emitterPtr_->particleMaxTime_ - 0.25f, max(p->timer_ - 0.25f, 0));
		float g = IFE::Lerp(0.4f, 0, emitterPtr_->particleMaxTime_ * 3 / 4, min(p->timer_, emitterPtr_->particleMaxTime_ * 2 / 3));
		float r = IFE::OutQuad(0.2f, 0, emitterPtr_->particleMaxTime_ / 2, min(p->timer_, emitterPtr_->particleMaxTime_ / 2));
		float s = IFE::OutQuad(0.35f, 0, emitterPtr_->particleMaxTime_, p->timer_);
		p->GetComponent<IFE::ColorBuffer>()->SetColor({ r, g, b, b });
		p->transform_->scale_ = { s,s,s };
		p->transform_->position_.x += IFE::IFERand::GetRandF(-1, 1) * IFE::IFETime::sDeltaTime_;
		p->transform_->position_.z += IFE::IFERand::GetRandF(-1, 1) * IFE::IFETime::sDeltaTime_;
		p->transform_->position_.y += IFE::IFERand::GetRandF(-1, 6) * IFE::IFETime::sDeltaTime_;
	}

	emitterPtr_->deleteFlag_ = enemy_->GetDeleteFlag();
	if (enemy_->GetComponent<IFE::Enemy>()->GetHP() != 0)
	{
		IFE::Float3 pos = *enemyPos_ + IFE::Float3(IFE::IFERand::GetRandF(-0.3f, 0.3f), IFE::IFERand::GetRandF(-0.3f, 0.3f), IFE::IFERand::GetRandF(-0.3f, 0.3f));
		auto p = emitterPtr_->AddParticle(pos);
		p->AddComponent<IFE::ColorBuffer>();
		p->GetComponent<IFE::ColorBuffer>()->SetColor({ 0.2f, 0.4f, 0.6f, 1.f });
		p->transform_->scale_ = { 0.35f,0.35f,0.35f };
		pos = *enemyPos_ + IFE::Float3(IFE::IFERand::GetRandF(-0.3f, 0.3f), IFE::IFERand::GetRandF(-0.3f, 0.3f), IFE::IFERand::GetRandF(-0.3f, 0.3f));
		auto p2 = emitterPtr_->AddParticle(pos);
		p2->AddComponent<IFE::ColorBuffer>();
		p2->GetComponent<IFE::ColorBuffer>()->SetColor({ 0.2f, 0.4f, 0.6f, 1.f });
		p2->transform_->scale_ = { 0.35f,0.35f,0.35f };
	}
}

void SpiritFire::SetEnemy(IFE::Object3D* enemy)
{
	enemy_ = enemy;
	enemyPos_ = &enemy->transform_->position_;
	emitterPtr_->isActive_ = true;
}
