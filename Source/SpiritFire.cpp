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
	if (!enemy_->isActive_)return;
	auto hp = enemy_->GetComponent<IFE::Enemy>()->GetHP();
	if (hp <= 0)hp = 1;
	(this->*ColorTableFunc[hp - 1])();
}

void SpiritFire::SetEnemy(IFE::Object3D* enemy)
{
	enemy_ = enemy;
	enemyPos_ = &enemy->transform_->position_;
	emitterPtr_->isActive_ = true;
}

void SpiritFire::GreenParticle()
{
	for (auto& p : emitterPtr_->particles_)
	{
		float g = IFE::OutQuad(sDefaultColor_.x, 0.1f, emitterPtr_->particleMaxTime_ - 0.25f, max(p->timer_ - 0.25f, 0));
		float b = IFE::Lerp(sDefaultColor_.y, 0, emitterPtr_->particleMaxTime_ * 3 / 4, min(p->timer_, emitterPtr_->particleMaxTime_ * 2 / 3));
		float r = IFE::OutQuad(sDefaultColor_.z, 0, emitterPtr_->particleMaxTime_ / 2, min(p->timer_, emitterPtr_->particleMaxTime_ / 2));
		float s = IFE::OutQuad(0.35f, 0, emitterPtr_->particleMaxTime_, p->timer_);
		p->GetComponent<IFE::ColorBuffer>()->SetColor({ r, g, b, g });
		p->transform_->scale_ = { s,s,s };
		p->transform_->position_.x += IFE::IFERand::GetRandF(-1, 1) * IFE::IFETime::sDeltaTime_ * 3;
		p->transform_->position_.z += IFE::IFERand::GetRandF(-1, 1) * IFE::IFETime::sDeltaTime_ * 3;
		p->transform_->position_.y += IFE::IFERand::GetRandF(-1, 6) * IFE::IFETime::sDeltaTime_ * 3;
	}

	emitterPtr_->deleteFlag_ = enemy_->GetDeleteFlag();
	if (enemy_->GetComponent<IFE::Enemy>()->GetAction() != uint8_t(IFE::EnemyAction::Death))
	{
		IFE::Float3 pos = *enemyPos_ + IFE::Float3(IFE::IFERand::GetRandF(-0.3f, 0.3f), IFE::IFERand::GetRandF(-0.3f, 0.3f), IFE::IFERand::GetRandF(-0.3f, 0.3f));
		auto p = emitterPtr_->AddParticle(pos);
		p->AddComponent<IFE::ColorBuffer>();
		p->GetComponent<IFE::ColorBuffer>()->SetColor({ sDefaultColor_.z, sDefaultColor_.x, sDefaultColor_.y, 1.f });
		p->transform_->scale_ = { 0.35f,0.35f,0.35f };
	}
}

void SpiritFire::YellowParticle()
{
	for (auto& p : emitterPtr_->particles_)
	{
		float g = IFE::OutQuad(sDefaultColor_.x, 0.1f, emitterPtr_->particleMaxTime_ - 0.25f, max(p->timer_ - 0.25f, 0));
		float b = IFE::OutQuad(sDefaultColor_.z, 0, emitterPtr_->particleMaxTime_ / 2, min(p->timer_, emitterPtr_->particleMaxTime_ / 2));
		float s = IFE::OutQuad(0.35f, 0, emitterPtr_->particleMaxTime_, p->timer_);
		p->GetComponent<IFE::ColorBuffer>()->SetColor({ g, g, b, g });
		p->transform_->scale_ = { s,s,s };
		p->transform_->position_.x += IFE::IFERand::GetRandF(-1, 1) * IFE::IFETime::sDeltaTime_ * 3;
		p->transform_->position_.z += IFE::IFERand::GetRandF(-1, 1) * IFE::IFETime::sDeltaTime_ * 3;
		p->transform_->position_.y += IFE::IFERand::GetRandF(-1, 6) * IFE::IFETime::sDeltaTime_ * 3;
	}

	emitterPtr_->deleteFlag_ = enemy_->GetDeleteFlag();
	if (enemy_->GetComponent<IFE::Enemy>()->GetAction() != uint8_t(IFE::EnemyAction::Death))
	{
		IFE::Float3 pos = *enemyPos_ + IFE::Float3(IFE::IFERand::GetRandF(-0.3f, 0.3f), IFE::IFERand::GetRandF(-0.3f, 0.3f), IFE::IFERand::GetRandF(-0.3f, 0.3f));
		auto p = emitterPtr_->AddParticle(pos);
		p->AddComponent<IFE::ColorBuffer>();
		p->GetComponent<IFE::ColorBuffer>()->SetColor({ sDefaultColor_.z, sDefaultColor_.x, sDefaultColor_.y, 1.f });
		p->transform_->scale_ = { 0.35f,0.35f,0.35f };
	}
}

void SpiritFire::RedParticle()
{
	for (auto& p : emitterPtr_->particles_)
	{
		float r = IFE::OutQuad(sDefaultColor_.x, 0.1f, emitterPtr_->particleMaxTime_ - 0.25f, max(p->timer_ - 0.25f, 0));
		float g = IFE::Lerp(sDefaultColor_.y, 0, emitterPtr_->particleMaxTime_ * 3 / 4, min(p->timer_, emitterPtr_->particleMaxTime_ * 2 / 3));
		float b = IFE::OutQuad(sDefaultColor_.z, 0, emitterPtr_->particleMaxTime_ / 2, min(p->timer_, emitterPtr_->particleMaxTime_ / 2));
		float s = IFE::OutQuad(0.35f, 0, emitterPtr_->particleMaxTime_, p->timer_);
		p->GetComponent<IFE::ColorBuffer>()->SetColor({ r, g, b, r });
		p->transform_->scale_ = { s,s,s };
		p->transform_->position_.x += IFE::IFERand::GetRandF(-1, 1) * IFE::IFETime::sDeltaTime_ * 3;
		p->transform_->position_.z += IFE::IFERand::GetRandF(-1, 1) * IFE::IFETime::sDeltaTime_ * 3;
		p->transform_->position_.y += IFE::IFERand::GetRandF(-1, 6) * IFE::IFETime::sDeltaTime_ * 3;
	}

	emitterPtr_->deleteFlag_ = enemy_->GetDeleteFlag();
	if (enemy_->GetComponent<IFE::Enemy>()->GetAction() != uint8_t(IFE::EnemyAction::Death))
	{
		IFE::Float3 pos = *enemyPos_ + IFE::Float3(IFE::IFERand::GetRandF(-0.3f, 0.3f), IFE::IFERand::GetRandF(-0.3f, 0.3f), IFE::IFERand::GetRandF(-0.3f, 0.3f));
		auto p = emitterPtr_->AddParticle(pos);
		p->AddComponent<IFE::ColorBuffer>();
		p->GetComponent<IFE::ColorBuffer>()->SetColor({ sDefaultColor_.z, sDefaultColor_.x, sDefaultColor_.y, 1.f });
		p->transform_->scale_ = { 0.35f,0.35f,0.35f };
	}
}

void SpiritFire::PurpleParticle()
{
	for (auto& p : emitterPtr_->particles_)
	{
		float r = IFE::OutQuad(sDefaultColor_.x, 0.1f, emitterPtr_->particleMaxTime_ - 0.25f, max(p->timer_ - 0.25f, 0));
		float g = IFE::OutQuad(sDefaultColor_.z, 0, emitterPtr_->particleMaxTime_ / 2, min(p->timer_, emitterPtr_->particleMaxTime_ / 2));
		float s = IFE::OutQuad(0.35f, 0, emitterPtr_->particleMaxTime_, p->timer_);
		p->GetComponent<IFE::ColorBuffer>()->SetColor({ r, g, r, r });
		p->transform_->scale_ = { s,s,s };
		p->transform_->position_.x += IFE::IFERand::GetRandF(-1, 1) * IFE::IFETime::sDeltaTime_ * 3;
		p->transform_->position_.z += IFE::IFERand::GetRandF(-1, 1) * IFE::IFETime::sDeltaTime_ * 3;
		p->transform_->position_.y += IFE::IFERand::GetRandF(-1, 6) * IFE::IFETime::sDeltaTime_ * 3;
	}

	emitterPtr_->deleteFlag_ = enemy_->GetDeleteFlag();
	if (enemy_->GetComponent<IFE::Enemy>()->GetAction() != uint8_t(IFE::EnemyAction::Death))
	{
		IFE::Float3 pos = *enemyPos_ + IFE::Float3(IFE::IFERand::GetRandF(-0.3f, 0.3f), IFE::IFERand::GetRandF(-0.3f, 0.3f), IFE::IFERand::GetRandF(-0.3f, 0.3f));
		auto p = emitterPtr_->AddParticle(pos);
		p->AddComponent<IFE::ColorBuffer>();
		p->GetComponent<IFE::ColorBuffer>()->SetColor({ sDefaultColor_.z, sDefaultColor_.x, sDefaultColor_.y, 1.f });
		p->transform_->scale_ = { 0.35f,0.35f,0.35f };
	}
}

void SpiritFire::BlueParticle()
{
	for (auto& p : emitterPtr_->particles_)
	{
		float b = IFE::OutQuad(sDefaultColor_.x, 0.1f, emitterPtr_->particleMaxTime_ - 0.25f, max(p->timer_ - 0.25f, 0));
		float g = IFE::Lerp(sDefaultColor_.y, 0, emitterPtr_->particleMaxTime_ * 3 / 4, min(p->timer_, emitterPtr_->particleMaxTime_ * 2 / 3));
		float r = IFE::OutQuad(sDefaultColor_.z, 0, emitterPtr_->particleMaxTime_ / 2, min(p->timer_, emitterPtr_->particleMaxTime_ / 2));
		float s = IFE::OutQuad(0.35f, 0, emitterPtr_->particleMaxTime_, p->timer_);
		p->GetComponent<IFE::ColorBuffer>()->SetColor({ r, g, b, b });
		p->transform_->scale_ = { s,s,s };
		p->transform_->position_.x += IFE::IFERand::GetRandF(-1, 1) * IFE::IFETime::sDeltaTime_ * 3;
		p->transform_->position_.z += IFE::IFERand::GetRandF(-1, 1) * IFE::IFETime::sDeltaTime_ * 3;
		p->transform_->position_.y += IFE::IFERand::GetRandF(-1, 6) * IFE::IFETime::sDeltaTime_ * 3;
	}

	emitterPtr_->deleteFlag_ = enemy_->GetDeleteFlag();
	if (enemy_->GetComponent<IFE::Enemy>()->GetAction() != uint8_t(IFE::EnemyAction::Death))
	{
		IFE::Float3 pos = *enemyPos_ + IFE::Float3(IFE::IFERand::GetRandF(-0.3f, 0.3f), IFE::IFERand::GetRandF(-0.3f, 0.3f), IFE::IFERand::GetRandF(-0.3f, 0.3f));
		auto p = emitterPtr_->AddParticle(pos);
		p->AddComponent<IFE::ColorBuffer>();
		p->GetComponent<IFE::ColorBuffer>()->SetColor({ sDefaultColor_.z, sDefaultColor_.y, sDefaultColor_.x, 1.f });
		p->transform_->scale_ = { 0.35f,0.35f,0.35f };
	}
}

void (SpiritFire::* SpiritFire::ColorTableFunc[])() =
{
	&SpiritFire::BlueParticle,&SpiritFire::GreenParticle,&SpiritFire::YellowParticle,&SpiritFire::RedParticle,&SpiritFire::PurpleParticle,
};