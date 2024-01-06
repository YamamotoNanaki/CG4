#include "GameConfigUI.h"
#include "Input.h"
#include "SpriteManager.h"
#include "Transform.h"
#include "PauseSystemUI.h"

void GameConfigUI::Initialize()
{
	sprites_[0] = IFE::SpriteManager::Instance()->GetSpritePtr("bgm");
	sprites_[1] = IFE::SpriteManager::Instance()->GetSpritePtr("se");
	sprites_[2] = IFE::SpriteManager::Instance()->GetSpritePtr("con");
	sprites_[3] = IFE::SpriteManager::Instance()->GetSpritePtr("bri");
	for (size_t i = 0; i < sprites_.size(); i++)
	{
		if (sprites_[i] && sprites_[i]->transform_)
		{
			spriteScales_[i] = sprites_[i]->transform_->scale2D_;
			bars_[i] = IFE::SpriteManager::Instance()->GetSpritePtr("bar" + std::to_string(i));
		}
	}
}

void GameConfigUI::Update()
{
	static float timer = 0;
	if (PauseSystemUI::configFlag_)
	{
		int32_t num = 0;
		switch (activeNum_)
		{
		case int8_t(SelectConfig::BGM):
			num = bgmVolume_;
			break;
		case int8_t(SelectConfig::SE):
			num = seVolume_;
			break;
		case int8_t(SelectConfig::CON):
			num = contrast_;
			break;
		case int8_t(SelectConfig::BRI):
			num = brightness_;
			break;
		default:
			break;
		}
		if (IFE::Input::GetKeyTrigger(IFE::Key::RIGHT))
		{
			num++;
		}
		if (IFE::Input::GetKeyTrigger(IFE::Key::LEFT))
		{
			num--;
		}
		num = min(max(minNum_, num), maxNum_);
		switch (activeNum_)
		{
		case int8_t(SelectConfig::BGM):
			bgmVolume_ = num;
			break;
		case int8_t(SelectConfig::SE):
			seVolume_ = num;
			break;
		case int8_t(SelectConfig::CON):
			contrast_ = num;
			break;
		case int8_t(SelectConfig::BRI):
			brightness_ = num;
			break;
		default:
			break;
		}
		if (IFE::Input::GetKeyTrigger(IFE::Key::UP))
		{
			if (activeNum_ == 0)
			{
				activeNum_ = (uint8_t)sprites_.size();
			}
			activeNum_--;
		}
		if (IFE::Input::GetKeyTrigger(IFE::Key::DOWN))
		{
			activeNum_++;
			if (activeNum_ >= sprites_.size())
			{
				activeNum_ = 0;
			}
		}
		timer += 3;
		if (timer > 360)timer -= 360;
		IFE::Float2 scale = { spriteScales_[activeNum_].x + cosf(IFE::ConvertToRadians(timer)) * spriteScales_[activeNum_].x / 5,
			spriteScales_[activeNum_].y + cosf(IFE::ConvertToRadians(timer)) * spriteScales_[activeNum_].y / 5 };
		sprites_[activeNum_]->transform_->scale2D_ = scale;
		for (size_t i = 0; i < sprites_.size(); i++)
		{
			if (i != activeNum_)
			{
				sprites_[i]->transform_->scale2D_ = spriteScales_[i];
			}
		}
	}
}
