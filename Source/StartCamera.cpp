#include "StartCamera.h"
#include "PlayerCamera.h"
#include "CameraManager.h"
#include "Transform.h"
#include "Player.h"
#include "IFETime.h"
#include "Ease.h"

void IFE::StartCamera::Initialize()
{
	sTimer_ = 0;
}

void IFE::StartCamera::Update()
{
	transformCamera_->eulerAngleDegrees_.x = 11;
	if (turnFlag_)
	{
		Float3 eye = sPlayerPtr_->transform_->position_ + Float3(0, 7.5, -PlayerCamera::sMinDistance_);
		sTimer_ += IFETime::sDeltaTime_;

		transformCamera_->eulerAngleDegrees_.y = InOutQuad(saveAngle_, 360, sMaxTime_, sTimer_);
		transformCamera_->eulerAngleDegrees_.x = InOutQuad(11, 5, sMaxTime_, sTimer_);
		float rotaX = ConvertToRadians(transformCamera_->eulerAngleDegrees_.y);
		Vector3 cameraF;
		cameraF.Set({ sinf(rotaX),0,cosf(rotaX) }, { 0,0,0 });
		cameraF.Normalize();
		transformCamera_->position_ = sPlayerPtr_->transform_->position_ + Float3(0, 7.5, 0) + Float3(cameraF * PlayerCamera::sMinDistance_);
		if (sTimer_ > sMaxTime_)
		{
			Player::sMoveFlag_ = true;
			CameraManager::Instance()->SetActiveCamera("playerCamera");
		}
	}
	else
	{
		transformCamera_->eulerAngleDegrees_.y += IFETime::sDeltaTime_ * 80;
		float rotaX = ConvertToRadians(transformCamera_->eulerAngleDegrees_.y);
		Vector3 cameraF;
		cameraF.Set({ sinf(rotaX),0,cosf(rotaX) }, { 0,0,0 });
		cameraF.Normalize();
		transformCamera_->position_ = sPlayerPtr_->transform_->position_ + Float3(0, 7.5, 0) + Float3(cameraF * PlayerCamera::sMinDistance_);

		Input* input = Input::Instance();
		if (input->GetPadConnected())
		{
			if (input->PadDown(PADCODE::ALL))
			{
				turnFlag_ = true;
				saveAngle_ = transformCamera_->eulerAngleDegrees_.y;
			}
		}
		else
		{
			if (input->KeyDown(Key_Space))
			{
				turnFlag_ = true;
				saveAngle_ = transformCamera_->eulerAngleDegrees_.y;
			}
		}
	}
}

#ifdef NDEBUG
#else
void IFE::StartCamera::OutputComponent(nlohmann::json& json)
{
	json;
}

void IFE::StartCamera::LoadingComponent(nlohmann::json& json)
{
	json;
}
#endif
