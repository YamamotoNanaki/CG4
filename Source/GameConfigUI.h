#pragma once
#include "Component.h"

enum class SelectConfig
{
	BGM, SE, CON, BRI
};

class GameConfigUI : public IFE::Component
{
	int32_t bgmVolume_ = 5;
	int32_t seVolume_ = 5;
	int32_t brightness_ = 5;
	int32_t contrast_ = 5;
	const int32_t maxNum_ = 10;
	const int32_t minNum_ = 0;
	const float defaultContrast_ = 1.25f;
	const float defaultBrightness_ = 0;

	uint8_t activeNum_ = 0;
	std::array<IFE::Sprite*, 4>sprites_;
	std::array<IFE::Sprite*, 4>bars_;
	std::array<IFE::Float2, 4>spriteScales_;
	IFE::Float2 barDefaultScale = { 5,1 };

public:
	void Initialize();
	void Update();
};

