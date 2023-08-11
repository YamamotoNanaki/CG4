#include "CommonGameUI.h"
#include "Player.h"
#include "Sprite.h"
#include "ColorBuffer.h"
#include "StartCamera.h"
#include "Ease.h"

void IFE::CommonGameUI::Update()
{
	if (!Player::sMoveFlag_)
	{
		spritePtr_->GetComponent<ColorBuffer>()->SetAlpha(InOutQuad(0, 1, StartCamera::sMaxTime_, StartCamera::sTimer_));
	}
}
