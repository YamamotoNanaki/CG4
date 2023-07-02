#include "Enemy.h"
#include "IFETime.h"
#include "Transform.h"
#include "Collider.h"
#include "Bullet.h"
#include "Object3D.h"
#include "Material.h"

void IFE::Enemy::Update()
{
	Move();
	if (isHit_)
	{
		objectPtr_->GetComponent<Material>()->color_ = { 1,0,0,1 };
		colorTimer_ += IFETime::sDeltaTime_;
		if (colorTimer_ > 0.7f)
		{
			isHit_ = false;
		}
	}
	else objectPtr_->GetComponent<Material>()->color_ = { 0,0,1,1 };
}

void IFE::Enemy::OnColliderHit(Collider* collider)
{
	if (collider->GetObjectPtr()->GetComponent<Bullet>())
	{
		colorTimer_ = 0;
		isHit_ = true;
	}
}

void IFE::Enemy::Move()
{
	static float timer;
	timer += IFETime::sDeltaTime_;
	if (timer > 2)
	{
		timer = 0;
		left_ = !left_;
	}
	if (left_)
	{
		transform_->position_.x -= speed_;
	}
	else
	{
		transform_->position_.x += speed_;
	}
}
