#include "Bullet.h"
#include "IFETime.h"
#include "Object3D.h"
#include "Transform.h"
#include "Enemy.h"
#include "Collider.h"
#include "Player.h"

void IFE::Bullet::Update()
{
	Move();
	Timer();
}

void IFE::Bullet::OnColliderHit(Collider* collider)
{
	if (collider->GetObjectPtr()->GetComponent<Enemy>())
	{

	}
	if (!collider->GetObjectPtr()->GetComponent<Player>())
	{
		objectPtr_->Destroy();
	}
}

void IFE::Bullet::SetMoveVector(const Vector2& moveVec)
{
	moveVec_ = moveVec;
}

void IFE::Bullet::Move()
{
	transform_->position_ += Float3{ moveVec_.x, 0, moveVec_.y } * speed_ * IFETime::sDeltaTime_;
}

void IFE::Bullet::Timer()
{
	lifeTimer_ += IFETime::sDeltaTime_;
	if (lifeTimer_ > timerMax_)objectPtr_->Destroy();
}
