#include "BulletParticle.h"
#include "Emitter.h"
#include "ColorBuffer.h"
#include "Ease.h"
#include "Rand.h"
#include "Object3D.h"
#include "Transform.h"

void IFE::BulletParticle::Update()
{
	for (auto& p : emitterPtr_->particles_)
	{
		float t = OutQuad(1, 0, emitterPtr_->particleMaxTime_, p->timer_);
		float s = OutQuad(0.15f, 0, emitterPtr_->particleMaxTime_, p->timer_);
		p->GetComponent<ColorBuffer>()->color_ = (t, t / 2, 0, t);
		p->transform_->scale_ = { s,s,s };
	}
	Float3 pos = *bulletPos_ + Float3(IFERand::GetRandF(-0.3f, 0.3f), IFERand::GetRandF(-0.3f, 0.3f), IFERand::GetRandF(-0.3f, 0.3f));
	auto p = emitterPtr_->AddParticle(pos);
	p->AddComponent<ColorBuffer>();
	p->GetComponent<ColorBuffer>()->color_ = (1.f, 0.5f, 0.f, 1.f);
	p->transform_->scale_ = { 0.15f,0.15f,0.15f };
	emitterPtr_->deleteFlag_ = bullet_->GetDeleteFlag();
}

void IFE::BulletParticle::GetBullet(Object3D* bullet)
{
	bullet_ = bullet;
	bulletPos_ = &bullet->GetComponent<Transform>()->position_;
}
