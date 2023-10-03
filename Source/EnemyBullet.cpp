#include "EnemyBullet.h"
#include "IFETime.h"
#include "Object3D.h"
#include "Transform.h"
#include "Enemy.h"
#include "Collider.h"
#include "Player.h"
#include "Boss.h"
#include "FireworkChrysanthemum.h"
#include "BulletParticle.h"

void IFE::EnemyBullet::Update()
{
	Move();
	Timer();
}

void IFE::EnemyBullet::OnColliderHit(Collider* collider)
{
	if (!collider->GetObjectPtr()->GetComponent<Enemy>() && !collider->GetObjectPtr()->GetComponent<Boss>())
	{
		auto e = ParticleManager::Instance()->Instantiate("Chrysanthemum", transform_->position_);
		e->GetComponent<FireworkChrysanthemum>()->StartFirework();
		e->GetComponent<FireworkChrysanthemum>()->SetColor(bulletEmitter_->GetComponent<BulletParticle>()->GetColor());
		objectPtr_->Destroy();
	}
}

void IFE::EnemyBullet::SetMoveVector(const Vector3& moveVec)
{
	moveVec_ = moveVec;
}

void IFE::EnemyBullet::SetBulletEmitter(Emitter* emitter)
{
	bulletEmitter_ = emitter;
}

IFE::Vector3 IFE::EnemyBullet::GetMoveVector()
{
	return moveVec_;
}

void IFE::EnemyBullet::Move()
{
	transform_->position_ += moveVec_ * speed_ * IFETime::sDeltaTime_;
}

void IFE::EnemyBullet::Timer()
{
	lifeTimer_ += IFETime::sDeltaTime_;
	if (lifeTimer_ > timerMax_)objectPtr_->Destroy();
}
