#pragma once
#include "Component.h"
#include "Player.h"

namespace IFE
{
	class PlayerCamera :public Component
	{
	private:
		float distance_ = 50;
	public:
		inline static Player* sPlayerPtr_ = nullptr;

		void Update();
	};
}
