#include "Bullet.h"
#include "IFETime.h"
#include "Object3D.h"
#include "Transform.h"
#include "Enemy.h"
#include "Collider.h"
#include "Player.h"
#include "FireworkChrysanthemum.h"
#include "BulletParticle.h"

void IFE::Bullet::Update()
{
	Move();
	Timer();
}

void IFE::Bullet::OnColliderHit(Collider* collider)
{
	if (!collider->GetObjectPtr()->GetComponent<Player>())
	{
		auto e = ParticleManager::Instance()->Instantiate("Chrysanthemum", transform_->position_);
		e->GetComponent<FireworkChrysanthemum>()->StartFirework();
		e->GetComponent<FireworkChrysanthemum>()->SetColor(bulletEmitter_->GetComponent<BulletParticle>()->GetColor());
		objectPtr_->Destroy();
	}
}

void IFE::Bullet::SetMoveVector(const Vector3& moveVec)
{
	moveVec_ = moveVec;
}

void IFE::Bullet::SetBulletEmitter(Emitter* emitter)
{
	bulletEmitter_ = emitter;
}

IFE::Vector3 IFE::Bullet::GetMoveVector()
{
	return moveVec_;
}

void IFE::Bullet::Move()
{
	transform_->position_ += moveVec_ * speed_ * IFETime::sDeltaTime_;
}

void IFE::Bullet::Timer()
{
	lifeTimer_ += IFETime::sDeltaTime_;
	if (lifeTimer_ > timerMax_)objectPtr_->Destroy();
}
