#include "PauseUI.h"
#include "PauseSystemUI.h"
#include "Sprite.h"

void PauseUI::Initialize()
{
	spritePtr_->drawFlag_ = false;
}

void PauseUI::Update()
{
	spritePtr_->drawFlag_ = PauseSystemUI::pauseFlag_ && !PauseSystemUI::configFlag_;
}
