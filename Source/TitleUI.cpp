#include "TitleUI.h"
#include "Transform.h"
#include "IFETime.h"

using namespace IFE;

void IFE::TitleUI::Initialize()
{
	if (transform2D_)initPos_ = transform2D_->position_;
}

void IFE::TitleUI::Update()
{
	Float2 pos = initPos_;

	timer_ += IFETime::sDeltaTime_;
	if (timer_ > maxTime_)timer_ -= maxTime_;

	pos.y = initPos_.y + sinf(float(M_PI) * 2 * (timer_ / maxTime_)) * 20;

	transform2D_->position_ = pos;
}
