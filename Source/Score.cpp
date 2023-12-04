#include "Score.h"
#include "SpriteManager.h"
#include "Enemy.h"
#include "TextureManager.h"

void Score::Initialize()
{
	sprites_[0] = IFE::SpriteManager::Instance()->GetSpritePtr("scoreNum");
	sprites_[1] = IFE::SpriteManager::Instance()->GetSpritePtr("scoreNum1");
	sprites_[2] = IFE::SpriteManager::Instance()->GetSpritePtr("scoreNum2");

	for (uint16_t i = 0; i < 10; i++)
	{
		std::string s = "number" + std::to_string(i);
		numberTextures_[i] = IFE::TextureManager::Instance()->GetTexture(s);
	}
}

void Score::Update()
{
	score_ = uint16_t(IFE::Enemy::Enemy::sScore_);

	uint16_t temNum = 100;
	uint16_t scoreTem = score_;
	for (size_t i = 0; i < 3; i++)
	{
		uint8_t num = uint8_t(scoreTem / temNum);
		sprites_[i]->tex_ = numberTextures_[num];
		scoreTem -= num * temNum;
		temNum /= 10;
	}
}
