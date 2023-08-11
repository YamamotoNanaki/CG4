#pragma once
#include "Component.h"
#include "Player.h"

namespace IFE
{
	class StartCamera : public Component
	{
	public:
		inline static Player* sPlayerPtr_ = nullptr;
		float timer_ = 0;
		inline static float sMaxTime_ = 0.5;
		float saveAngle_ = 0;
	public:
		void Update()override;

#ifdef NDEBUG
#else
		virtual void OutputComponent(nlohmann::json& json)override;
		virtual void LoadingComponent(nlohmann::json& json)override;
#endif
	};
}
