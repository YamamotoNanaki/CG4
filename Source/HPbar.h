#pragma once
#include "Component.h"
#include "Player.h"


namespace IFE
{
	class HPbar : public Component
	{
		Player* playerPtr_;
		Float2 scale_;
		float timer_ = 0;
		bool anim_ = false;
		float timerMax_ = 1.25f;
		int16_t hp_ = 5;
		int16_t oldHp_ = 5;
		float oldX;
		float newX;

	public:
		void Initialize();
		void Update();
	};
}
