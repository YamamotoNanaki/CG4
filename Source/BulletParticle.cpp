#include "BulletParticle.h"
#include "Emitter.h"
#include "ColorBuffer.h"
#include "Ease.h"
#include "Rand.h"
#include "Object3D.h"
#include "Transform.h"
#include "LightManager.h"
#include "EnemyBullet.h"


void IFE::BulletParticle::Update()
{
	switch (colorSetting_)
	{
	case (uint8_t)ParticleColorSetting::Blue:
		for (auto& p : emitterPtr_->particles_)
		{
			float b = OutQuad(1.f, 0.1f, emitterPtr_->particleMaxTime_ - 0.25f, max(p->timer_ - 0.25f, 0));
			float g = Lerp(1, 0, emitterPtr_->particleMaxTime_ * 3 / 4, min(p->timer_, emitterPtr_->particleMaxTime_ * 2 / 3));
			float r = OutQuad(1, 0, emitterPtr_->particleMaxTime_ / 2, min(p->timer_, emitterPtr_->particleMaxTime_ / 2));
			float s = OutQuad(0.15f, 0, emitterPtr_->particleMaxTime_, p->timer_);
			p->GetComponent<ColorBuffer>()->SetColor({ r, g, b, b });
			p->transform_->scale_ = { s,s,s };
		}
		LightManager::Instance()->SetPointLightColor(pointLightNum_, { 0.25f,0.5f,1.f });
		break;
	default:
		for (auto& p : emitterPtr_->particles_)
		{
			float r = OutQuad(1.f, 0.1f, emitterPtr_->particleMaxTime_ - 0.25f, max(p->timer_ - 0.25f, 0));
			float g = Lerp(1, 0, emitterPtr_->particleMaxTime_ * 3 / 4, min(p->timer_, emitterPtr_->particleMaxTime_ * 2 / 3));
			float b = OutQuad(1, 0, emitterPtr_->particleMaxTime_ / 2, min(p->timer_, emitterPtr_->particleMaxTime_ / 2));
			float s = OutQuad(0.15f, 0, emitterPtr_->particleMaxTime_, p->timer_);
			p->GetComponent<ColorBuffer>()->SetColor({ r, g, b, r });
			p->transform_->scale_ = { s,s,s };
		}
		LightManager::Instance()->SetPointLightColor(pointLightNum_, { 1.f,0.5f,0.25f });
		break;
	}
	Float3 pos = *bulletPos_ + Float3(IFERand::GetRandF(-0.3f, 0.3f), IFERand::GetRandF(-0.3f, 0.3f), IFERand::GetRandF(-0.3f, 0.3f));
	auto p = emitterPtr_->AddParticle(pos);
	p->AddComponent<ColorBuffer>();
	p->GetComponent<ColorBuffer>()->SetColor({ 1.f, 1.f, 1.f, 1.f });
	p->transform_->scale_ = { 0.15f,0.15f,0.15f };
	pos = *bulletPos_ + Float3(IFERand::GetRandF(-0.3f, 0.3f), IFERand::GetRandF(-0.3f, 0.3f), IFERand::GetRandF(-0.3f, 0.3f));
	auto p2 = emitterPtr_->AddParticle(pos);
	p2->AddComponent<ColorBuffer>();
	p2->GetComponent<ColorBuffer>()->SetColor({ 1.f, 1.f, 1.f, 1.f });
	p2->transform_->scale_ = { 0.15f,0.15f,0.15f };
	emitterPtr_->deleteFlag_ = bullet_->GetDeleteFlag();
	LightManager::Instance()->SetPointLightPos(pointLightNum_, *bulletPos_);
	LightManager::Instance()->SetPointLightAtten(pointLightNum_, { 0.0001f,0.001f,0.0001f });
}

void IFE::BulletParticle::SetBullet(Object3D* bullet)
{
	bullet_ = bullet;
	bulletPos_ = &bullet->GetComponent<Transform>()->position_;
	LightManager::Instance()->SetPointLightActive(LightManager::nextPNum_, true);
	LightManager::Instance()->SetPointLightPos(LightManager::nextPNum_, transformParticle_->position_);
	LightManager::Instance()->SetPointLightAtten(LightManager::nextPNum_, { 0.001f,0,0 });
	pointLightNum_ = LightManager::nextPNum_;
	LightManager::nextPNum_++;

	if (bullet->GetComponent<EnemyBullet>())
	{
		bullet->GetComponent<EnemyBullet>()->SetBulletEmitter(emitterPtr_);
	}
}

void IFE::BulletParticle::SetColor(ParticleColorSetting colorSetting)
{
	colorSetting_ = (uint8_t)colorSetting;
}

uint8_t IFE::BulletParticle::GetColor()
{
	return colorSetting_;
}

IFE::BulletParticle::~BulletParticle()
{
	LightManager::Instance()->SetPointLightActive(pointLightNum_, false);
}
