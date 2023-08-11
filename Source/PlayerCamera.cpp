#include "PlayerCamera.h"
#include "Transform.h"

void IFE::PlayerCamera::Update()
{
	Float3 eye = sPlayerPtr_->transform_->position_ + + Float3(0, 5, -50);

	transformCamera_->position_ = eye;
}
