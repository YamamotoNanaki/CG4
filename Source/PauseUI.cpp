#include "PauseUI.h"
#include "PauseSystemUI.h"
#include "Sprite.h"

void PauseUI::Update()
{
	spritePtr_->drawFlag_ = PauseSystemUI::pauseFlag_;
}
