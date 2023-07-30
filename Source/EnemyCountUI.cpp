#include "EnemyCountUI.h"
#include "Enemy.h"
#include "Sprite.h"

void IFE::EnemyCountUI::Initialize()
{
	for (uint16_t i = 0; i < 10; i++)
	{
		std::string s = "number" + std::to_string(i);
		numberTextures_[i] = TextureManager::Instance()->GetTexture(s);
	}
}

void IFE::EnemyCountUI::Update()
{
	spritePtr_->tex_ = numberTextures_[Enemy::sDeathEnemyNum_];
}
