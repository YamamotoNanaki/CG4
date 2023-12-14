#pragma once
#include "Component.h"

namespace IFE
{
	class TitleUI : public Component
	{
		float timer_ = 0;
		float maxTime_ = 2;
		Float2 initPos_;
		bool soundFlag_ = false;
		float fireworkTimer_ = 0;
		float fireworkMaxTime_ = 0.5;

	public:
		void Initialize()override;
		void Update()override;

	private:
		void Firework();
	};
}
