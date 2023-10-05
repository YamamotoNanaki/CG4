#pragma once
#include "Component.h"
#include "Player.h"


namespace IFE
{
	class StartCamera : public Component
	{
	private:
		float saveAngle_ = 0;
		bool turnFlag_;
	public:
		inline static Player* sPlayerPtr_ = nullptr;
		inline static float sTimer_ = 0;
		inline static float sMaxTime_ = 0.5;
	public:
		void Initialize()override;
		void Update()override;

#ifdef NDEBUG
#else
		virtual void OutputComponent(nlohmann::json& json)override;
		virtual void LoadingComponent(nlohmann::json& json)override;
#endif
	};
}
