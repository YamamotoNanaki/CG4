#include "PauseSystemUI.h"
#include "Input.h"
#include "Engine.h"
#include "IFETime.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Scene.h"
#include "Transform.h"

void PauseSystemUI::Initialize()
{
	sprites_[0] = IFE::SpriteManager::Instance()->GetSpritePtr("return");
	sprites_[1] = IFE::SpriteManager::Instance()->GetSpritePtr("restart");
	sprites_[2] = IFE::SpriteManager::Instance()->GetSpritePtr("config");
	sprites_[3] = IFE::SpriteManager::Instance()->GetSpritePtr("toTitle");
	sprites_[4] = IFE::SpriteManager::Instance()->GetSpritePtr("gameEnd");
	for (size_t i = 0; i < sprites_.size(); i++)
	{
		if (sprites_[i] && sprites_[i]->transform_)
		{
			spriteScales_[i] = sprites_[i]->transform_->scale2D_;
		}
	}
	spritePtr_->drawFlag_ = false;
}

void PauseSystemUI::Update()
{
	static float timer = 0;
	if (configFlag_)
	{
		if (IFE::Input::GetKeyTrigger(IFE::Key::Esc))
		{
			configFlag_ = false;
		}
	}
	else
	{
		if (pauseFlag_)
		{
			if (IFE::Input::GetKeyTrigger(IFE::Key::Esc))
			{
				IFE::Engine::SetShutdown();
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
			if (IFE::Input::GetKeyTrigger(IFE::Key::Space))
			{
				IFE::IFETime::sTimeScale_ = 1;
				pauseFlag_ = false;
				switch (activeNum_)
				{
				case 1:
					IFE::Scene::Instance()->SetNextScene("main");
					break;
				case 2:
					IFE::IFETime::sTimeScale_ = 0;
					configFlag_ = true;
					break;
				case 3:
					IFE::Scene::Instance()->SetNextScene("title");
					break;
				case 4:
					IFE::Engine::SetShutdown();
					break;
				default:
					break;
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
		else
		{
			if (IFE::Input::GetKeyTrigger(IFE::Key::Esc))
			{
				pauseFlag_ = true;
				IFE::IFETime::sTimeScale_ = 0;
				activeNum_ = 0;
				timer = 0;
			}
		}
	}
	spritePtr_->drawFlag_ = PauseSystemUI::pauseFlag_;
}
