#include "LightCamera.h"
#include "ObjectManager.h"
#include "Transform.h"

void LightCamera::Initialize()
{
	if (IFE::ObjectManager::Instance()->GetObjectPtr("Player"))
	{
		playerPos_ = &IFE::ObjectManager::Instance()->GetObjectPtr("Player")->transform_->position_;
	}
}

void LightCamera::Update()
{
	transformCamera_->position_ = { playerPos_->x, 80, playerPos_->z + 80 };
	transformCamera_->eulerAngleDegrees_ = { 45, 180, 0 };
	transformCamera_->UpdateMatrix();
}
