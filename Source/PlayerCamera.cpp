#include "PlayerCamera.h"
#include "Transform.h"

#include "IFETime.h"

void IFE::PlayerCamera::Initialize()
{
    transformCamera_->position_ = sPlayerPtr_->transform_->position_ + Float3(0, 7.5, -distance_);
}

void IFE::PlayerCamera::Update()
{
}
