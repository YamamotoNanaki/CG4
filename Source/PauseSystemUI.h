#pragma once
#include "Component.h"
#include <array>

class PauseSystemUI : public IFE::Component
{
	inline static uint8_t activeNum_ = 0;
	std::array<IFE::Sprite*, 5>sprites_;
	std::array<IFE::Float2, 5>spriteScales_;
public:
	inline static bool pauseFlag_ = false;
	inline static bool configFlag_ = false;
	void Initialize();
	void Update();
};

