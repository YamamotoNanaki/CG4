#include "PlayerCamera.h"
#include "Transform.h"
#include "Ease.h"
#include "IFETime.h"
#include "Rand.h"


void IFE::PlayerCamera::Initialize()
{
	transformCamera_->position_ = sPlayerPtr_->transform_->position_ + Float3(0, 7.5, -distance_);
	transformCamera_->eulerAngleDegrees_.x = 11;
}

void IFE::PlayerCamera::Update()
{
	bool moveFlag = false;
	if (sPlayerPtr_->pos_ != sPlayerPtr_->oldPos_)
	{
		moveFlag = true;
	}
	if (moveFlag)
	{
		timer_ += IFETime::sDeltaTime_;
	}
	else
	{
		timer_ -= IFETime::sDeltaTime_;
	}
	timer_ = min(max(0, timer_), sMaxTime_);

	distance_ = InOutQuad(sMinDistance_, sMaxDistance_, sMaxTime_, timer_);
	float padX = sInput_->GetRXAnalog(4000) * 5 * IFETime::sDeltaTime_ * 40;
	//float padX = ((int32_t)sInput_->PadDown(PADCODE::RIGHT) - (int32_t)sInput_->PadDown(PADCODE::LEFT)) * 5 * IFETime::sDeltaTime_ * 40;
	float padY = sInput_->GetRYAnalog(4000) * IFETime::sDeltaTime_ * 40;
	if (fabsf(padX) > 0.01)
	{
		cameraMoveX_ = true;
		transformCamera_->eulerAngleDegrees_.y += Lerp(0, padX, moveMaxTime_, moveTimerX_);
		moveTimerX_ += IFETime::sDeltaTime_;
		moveTimerX_ = min(moveMaxTime_, moveTimerX_);
	}
	else if (cameraMoveX_)
	{
		static float LX = 0;
		if (oldPadX_ != 0)
		{
			LX = oldPadX_;
		}
		transformCamera_->eulerAngleDegrees_.y += Lerp(0, LX, moveMaxTime_, moveTimerX_);
		moveTimerX_ -= IFETime::sDeltaTime_ * 2;
		if (moveTimerX_ <= 0)
		{
			moveTimerX_ = 0;
			cameraMoveX_ = false;
		}
	}

	Float3 shake;
	if (sPlayerPtr_->GetInvincible())
	{
		shakeTimer_ += IFETime::sDeltaTime_;
		if (sPlayerPtr_->hp_ <= 0)
		{
			float f = Lerp(2, 0, 2, min(shakeTimer_, 2));
			shake.x = IFERand::GetRandF(-f, f);
			shake.y = IFERand::GetRandF(-f, f);
			shake.z = IFERand::GetRandF(-f, f);
		}
		else
		{
			float f = Lerp(0.25f, 0, 0.5f, min(shakeTimer_, 0.5f));
			shake.x = IFERand::GetRandF(-f, f);
			shake.y = IFERand::GetRandF(-f, f);
			shake.z = IFERand::GetRandF(-f, f);
		}
	}
	else
	{
		shakeTimer_ = 0;
	}

	float rotaX = ConvertToRadians(transformCamera_->eulerAngleDegrees_.y);
	Vector3 cameraF;
	cameraF.Set({ sinf(rotaX),0,cosf(rotaX) }, { 0,0,0 });
	cameraF.Normalize();
	transformCamera_->position_ = sPlayerPtr_->transform_->position_ + Float3(0, 7.5, 0) + Float3(cameraF * distance_) + shake;

	float rotaY = transformCamera_->eulerAngleDegrees_.x;
	if (rotaY > 180)
	{
		rotaY -= 360;
	}
	if (fabsf(padY) > 0.01)
	{
		cameraMoveY_ = true;
		rotaY -= Lerp(0, padY, moveMaxTimeY_, moveTimerY_);
		moveTimerY_ += IFETime::sDeltaTime_;
		moveTimerY_ = min(moveMaxTimeY_, moveTimerY_);
	}
	else if (cameraMoveY_)
	{
		static float LY = 0;
		if (oldPadX_ != 0)
		{
			LY = oldPadY_;
		}
		rotaY -= Lerp(0, LY, moveMaxTimeY_, moveTimerY_);
		moveTimerY_ -= IFETime::sDeltaTime_ * 2;
		if (moveTimerY_ <= 0)
		{
			moveTimerY_ = 0;
			cameraMoveY_ = false;
		}
	}
	rotaY = min(max(-5, rotaY), 5);
	transformCamera_->eulerAngleDegrees_.x = rotaY;

	oldPadX_ = padX;
	oldPadY_ = padY;
}

#ifdef NDEBUG
#else
#include "ImguiManager.h"
void IFE::PlayerCamera::ComponentDebugGUI()
{
	static ImguiManager* im = ImguiManager::Instance();
	im->DragFloatGUI(&sMinDistance_, "S min distance");
	im->DragFloatGUI(&sMaxDistance_, "S max distance");
	im->DragFloatGUI(&sMaxTime_, "S max time");
}
void IFE::PlayerCamera::OutputComponent(nlohmann::json& json)
{
	json["minDistance"] = sMinDistance_;
	json["maxDistance"] = sMaxDistance_;
	json["maxTime"] = sMaxTime_;
}
#endif
void IFE::PlayerCamera::LoadingComponent(nlohmann::json& json)
{
	sMinDistance_ = json["minDistance"];
	sMaxDistance_ = json["maxDistance"];
	sMaxTime_ = json["maxTime"];
}
