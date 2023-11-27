#include "BulletParticle.h"
#include "Emitter.h"
#include "ColorBuffer.h"
#include "Ease.h"
#include "Rand.h"
#include "Object3D.h"
#include "Transform.h"
#include "LightManager.h"
#include "EnemyBullet.h"

using namespace IFE;

void IFE::BulletParticle::Update()
{
	(this->*ColorTableFunc[colorSetting_])();
	AddBulletParticle();
}

void IFE::BulletParticle::SetBullet(Object3D* bullet)
{
	bullet_ = bullet;
	bulletPos_ = &bullet->GetComponent<Transform>()->position_;
	pointLightNum_ = LightManager::GetPointLightNumber();
	LightManager::Instance()->SetPointLightActive(pointLightNum_, true);
	LightManager::Instance()->SetPointLightPos(pointLightNum_, transformParticle_->position_);
	LightManager::Instance()->SetPointLightAtten(pointLightNum_, { 0.001f,0,0 });

	if (bullet->GetComponent<EnemyBullet>())
	{
		bullet->GetComponent<EnemyBullet>()->SetBulletEmitter(emitterPtr_);
	}
}

void IFE::BulletParticle::SetColor(ParticleColorSetting colorSetting)
{
	colorSetting_ = (uint8_t)colorSetting;
}

void IFE::BulletParticle::SetColor(uint8_t colorSetting)
{
	colorSetting_ = colorSetting;
}

uint8_t IFE::BulletParticle::GetColor()
{
	return colorSetting_;
}

IFE::BulletParticle::~BulletParticle()
{
	LightManager::Instance()->SetPointLightActive(pointLightNum_, false);
}

void IFE::BulletParticle::AddBulletParticle()
{
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

void IFE::BulletParticle::BlueParticle()
{
	for (auto& p : emitterPtr_->particles_)
	{
		float b = OutQuad(1.f, 0.1f, emitterPtr_->particleMaxTime_ - 0.25f, max(p->timer_ - 0.25f, 0));
		float g = Lerp(1, 0, emitterPtr_->particleMaxTime_ * 3 / 4, min(p->timer_, emitterPtr_->particleMaxTime_ * 2 / 3));
		float r = OutQuad(1, 0, emitterPtr_->particleMaxTime_ / 2, min(p->timer_, emitterPtr_->particleMaxTime_ / 2));
		float s = OutQuad(0.15f, 0, emitterPtr_->particleMaxTime_, p->timer_);
		p->GetComponent<ColorBuffer>()->SetColor({ r, g, b, b });
		p->transform_->scale_ = { s,s,s };
	}
	LightManager::Instance()->SetPointLightColor(pointLightNum_, { sDefaultColor_.z,sDefaultColor_.y,sDefaultColor_.x });
}

void IFE::BulletParticle::GreenParticle()
{
	for (auto& p : emitterPtr_->particles_)
	{
		float g = OutQuad(1.f, 0.1f, emitterPtr_->particleMaxTime_ - 0.25f, max(p->timer_ - 0.25f, 0));
		float b = Lerp(1, 0, emitterPtr_->particleMaxTime_ * 3 / 4, min(p->timer_, emitterPtr_->particleMaxTime_ * 2 / 3));
		float r = OutQuad(1, 0, emitterPtr_->particleMaxTime_ / 2, min(p->timer_, emitterPtr_->particleMaxTime_ / 2));
		float s = OutQuad(0.15f, 0, emitterPtr_->particleMaxTime_, p->timer_);
		p->GetComponent<ColorBuffer>()->SetColor({ r, g, b, g });
		p->transform_->scale_ = { s,s,s };
	}
	LightManager::Instance()->SetPointLightColor(pointLightNum_, { sDefaultColor_.z,sDefaultColor_.x,sDefaultColor_.y });
}

void IFE::BulletParticle::YellowParticle()
{
	for (auto& p : emitterPtr_->particles_)
	{
		float r = OutQuad(1.f, 0.1f, emitterPtr_->particleMaxTime_ - 0.25f, max(p->timer_ - 0.25f, 0));
		float b = OutQuad(1, 0, emitterPtr_->particleMaxTime_ / 2, min(p->timer_, emitterPtr_->particleMaxTime_ / 2));
		float s = OutQuad(0.15f, 0, emitterPtr_->particleMaxTime_, p->timer_);
		p->GetComponent<ColorBuffer>()->SetColor({ r, r, b, r });
		p->transform_->scale_ = { s,s,s };
	}
	LightManager::Instance()->SetPointLightColor(pointLightNum_, { sDefaultColor_.x,sDefaultColor_.x,sDefaultColor_.z });
}

void IFE::BulletParticle::RedParticle()
{
	for (auto& p : emitterPtr_->particles_)
	{
		float r = OutQuad(1.f, 0.1f, emitterPtr_->particleMaxTime_ - 0.25f, max(p->timer_ - 0.25f, 0));
		float g = Lerp(1, 0, emitterPtr_->particleMaxTime_ * 3 / 4, min(p->timer_, emitterPtr_->particleMaxTime_ * 2 / 3));
		float b = OutQuad(1, 0, emitterPtr_->particleMaxTime_ / 2, min(p->timer_, emitterPtr_->particleMaxTime_ / 2));
		float s = OutQuad(0.15f, 0, emitterPtr_->particleMaxTime_, p->timer_);
		p->GetComponent<ColorBuffer>()->SetColor({ r, g, b, r });
		p->transform_->scale_ = { s,s,s };
	}
	LightManager::Instance()->SetPointLightColor(pointLightNum_, { sDefaultColor_.x,sDefaultColor_.y,sDefaultColor_.z });
}

void IFE::BulletParticle::PurpleParticle()
{
	for (auto& p : emitterPtr_->particles_)
	{
		float r = OutQuad(1.f, 0.1f, emitterPtr_->particleMaxTime_ - 0.25f, max(p->timer_ - 0.25f, 0));
		float g = Lerp(1, 0, emitterPtr_->particleMaxTime_ * 3 / 4, min(p->timer_, emitterPtr_->particleMaxTime_ * 2 / 3));
		float s = OutQuad(0.15f, 0, emitterPtr_->particleMaxTime_, p->timer_);
		p->GetComponent<ColorBuffer>()->SetColor({ r, g, r, r });
		p->transform_->scale_ = { s,s,s };
	}
	LightManager::Instance()->SetPointLightColor(pointLightNum_, { sDefaultColor_.x,sDefaultColor_.y,sDefaultColor_.x });
}

void (BulletParticle::* BulletParticle::ColorTableFunc[])() =
{
	&BulletParticle::BlueParticle,&BulletParticle::GreenParticle,&BulletParticle::YellowParticle,&BulletParticle::RedParticle,
	&BulletParticle::PurpleParticle,
};