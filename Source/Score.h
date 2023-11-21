#pragma once
#include "Component.h"
#include "Sprite.h"
#include "Texture.h"

class Score : public IFE::Component
{
	uint16_t score_;
	std::array<IFE::Sprite*, 3>sprites_;
	std::array<IFE::Texture*, 10>numberTextures_;
public:
	void Initialize()override;
	void Update()override;
};

