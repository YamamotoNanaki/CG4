#include "HPbar.h"
#include "Transform.h"
#include "ObjectManager.h"
#include "Sprite.h"

void IFE::HPbar::Initialize()
{
	playerPtr_ = ObjectManager::Instance()->GetObjectPtr("Player")->GetComponent<Player>();
	scale_ = transform2D_->scale2D_;
}

void IFE::HPbar::Update()
{
	if (playerPtr_->hp_ <= 0)spritePtr_->Destroy();
	transform2D_->scale2D_ = Float2(scale_.x * playerPtr_->hp_ , scale_.y);
}
