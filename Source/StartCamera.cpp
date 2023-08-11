#include "StartCamera.h"
#include "PlayerCamera.h"
#include "CameraManager.h"
#include "Transform.h"

void IFE::StartCamera::Update()
{
	if (Player::sMoveFlag_)
	{
		CameraManager::Instance()->SetActiveCamera("playerCamera");
	}

	Float3 eye = sPlayerPtr_->transform_->position_ + +Float3(0, 5, -50);

	transformCamera_->position_ = eye;
}
