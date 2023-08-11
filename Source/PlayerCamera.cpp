#include "PlayerCamera.h"
#include "Transform.h"
#include "Ease.h"
#include "IFETime.h"

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
	float padX = sInput_->GetRXAnalog(4000) * 5;
	float padY = sInput_->GetRYAnalog(4000)/2;
	if (fabsf(padX) > 0.01)
	{
		transformCamera_->eulerAngleDegrees_.y += padX;
	}

	float rotaX = ConvertToRadians(transformCamera_->eulerAngleDegrees_.y);
	Vector3 cameraF;
	cameraF.Set({ sinf(rotaX),0,cosf(rotaX) }, { 0,0,0 });
	cameraF.Normalize();
	transformCamera_->position_ = sPlayerPtr_->transform_->position_ + Float3(0, 7.5, 0) + Float3(cameraF * distance_);

	float rotaY = transformCamera_->eulerAngleDegrees_.x;
	if (rotaY > 180)
	{
		rotaY -= 360;
	}
	rotaY -= padY;
	rotaY = min(max(-5, rotaY), 5);
	transformCamera_->eulerAngleDegrees_.x = rotaY;
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
void IFE::PlayerCamera::LoadingComponent(nlohmann::json& json)
{
	sMinDistance_ = json["minDistance"];
	sMaxDistance_ = json["maxDistance"];
	sMaxTime_ = json["maxTime"];
}
#endif
