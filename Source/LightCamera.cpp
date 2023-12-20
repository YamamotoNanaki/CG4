#include "LightCamera.h"
#include "ObjectManager.h"
#include "Transform.h"

void LightCamera::Initialize()
{
	if (IFE::ObjectManager::Instance()->GetObjectPtr("Player"))
	{
		playerPos_ = &IFE::ObjectManager::Instance()->GetObjectPtr("Player")->transform_->position_;
	}
	if (transformCamera_)
	{
		transformCamera_->eyeTargetUpFlag_ = true;
	}
}

void LightCamera::Update()
{
	transformCamera_->eye_ = { playerPos_->x, playerPos_->y + 100, playerPos_->z-100 };
	transformCamera_->target_ = { playerPos_->x,playerPos_->y,playerPos_->z };
	transformCamera_->UpdateMatrix();
}
