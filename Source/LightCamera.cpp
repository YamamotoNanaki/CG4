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
		transformCamera_->nearZ_ = 20;
		transformCamera_->ferZ_ = 1000;
		transformCamera_->eye_ = { 0, 500, 250 };
		transformCamera_->target_ = { 0,0,-200 };
		transformCamera_->up_ = { 0,1,0 };
	}
}

void LightCamera::Update()
{
	//transformCamera_->eye_ = { playerPos_->x, playerPos_->y + 20, playerPos_->z - 20 };
	//transformCamera_->target_ = { playerPos_->x,playerPos_->y,playerPos_->z };
	//transformCamera_->up_ = { 0,1,0 };
	transformCamera_->UpdateMatrix();
}
