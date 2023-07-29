#pragma once
#include "Component.h"
#include <vector>

namespace IFE
{
	class FireworkChrysanthemum : public Component
	{
		inline static uint32_t particleMaxNum_ = 100;
		inline static float sStartSpeed_ = 5;
		inline static std::vector<Float3>sDefaultVelocity_;
		inline static float sScale_ = 0.1f;
		std::vector<Float3> velocitys_;
		std::vector<Float4> colors_;
		float speed_ = 5;
		float gravity_ = 0;
		Float4 baseColor_ = { 1,1,1,1 };
	public:
		void Update()override;
		void StartFirework();
		static void InitDefaultVelocity();

#ifdef _DEBUG
		virtual void ComponentDebugGUI()override;
		virtual void OutputComponent(nlohmann::json& json)override;
#endif
		virtual void LoadingComponent(nlohmann::json& json)override;
	};
}
