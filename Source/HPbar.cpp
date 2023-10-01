#include "HPbar.h"
#include "Transform.h"
#include "ObjectManager.h"
#include "Sprite.h"
#include "StartCamera.h"
#include "Ease.h"
#include "ColorBuffer.h"

void IFE::HPbar::Initialize()
{
	playerPtr_ = ObjectManager::Instance()->GetObjectPtr("Player")->GetComponent<Player>();
	scale_ = transform2D_->scale2D_;
}

void IFE::HPbar::Update()
{
	oldHp_ = hp_;
	hp_ = playerPtr_->hp_;
	if (!anim_, oldHp_ != hp_)
	{
		oldX = scale_.x * oldHp_;
		newX = scale_.x * hp_;
		anim_ = true;
		timer_ = 0;
	}

	if (anim_)
	{
		if (IFETime::sTimeScale_ != 0)timer_ += IFETime::sDeltaTime_ / IFETime::sTimeScale_;
		float x = EaseOutBounce(oldX, newX, timerMax_, min(timer_, timerMax_));
		if (timer_ > timerMax_)
		{
			anim_ = false;
		}
		transform2D_->scale2D_ = Float2(x, scale_.y);
	}
	else
	{
		if (playerPtr_->hp_ <= 0)spritePtr_->Destroy();
		transform2D_->scale2D_ = Float2(scale_.x * hp_, scale_.y);
	}

	if (!Player::sMoveFlag_)
	{
		spritePtr_->GetComponent<ColorBuffer>()->SetAlpha(InOutQuad(0, 1, StartCamera::sMaxTime_, StartCamera::sTimer_));
	}
}
