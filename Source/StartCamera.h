#pragma once
#include "Component.h"
#include "Player.h"

namespace IFE
{
	class StartCamera : public Component
	{
	public:
		inline static Player* sPlayerPtr_ = nullptr;
	public:
		void Update();
	};
}
