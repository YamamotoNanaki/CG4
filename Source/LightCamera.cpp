#include "LightCamera.h"
#include "ObjectManager.h"
#include "Transform.h"

void LightCamera::Initialize()
{
	if (IFE::ObjectManager::Instance()->GetObjectPtr("Player"))
	{
		playerPos_ = &IFE::ObjectManager::Instance()->GetObjectPtr("Player")->transform_->position_;
		transformCamera_->eye_ = { playerPos_->x, playerPos_->y + 60,playerPos_->z + 60 };
		transformCamera_->target_ = { playerPos_->x,playerPos_->y,playerPos_->z };
	}
	if (transformCamera_)
	{
		//transformCamera_->eyeTargetUpFlag_ = true;
		//transformCamera_->nearZ_ = 0;
		//transformCamera_->ferZ_ = 1000;
		//transformCamera_->eye_ = { 0, 500, 250 };
		//transformCamera_->target_ = { 0,0,-200 };
		transformCamera_->up_ = { 0,-1,1 };
		transformCamera_->draw2dFlag_ = true;
	}
}

void LightCamera::Update()
{
	transformCamera_->eye_ = { playerPos_->x, playerPos_->y + 60,playerPos_->z + 60 };
	transformCamera_->target_ = { playerPos_->x,playerPos_->y,playerPos_->z };
	//transformCamera_->up_ = { 0,0,1 };
	transformCamera_->UpdateMatrix();
}
