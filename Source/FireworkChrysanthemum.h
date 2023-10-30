#pragma once
#include "Component.h"
#include "ParticleSetting.h"
#include <vector>

namespace IFE
{
	class FireworkChrysanthemum : public Component
	{
		inline static uint32_t particleMaxNum_ = 100;
		inline static float sStartSpeed_ = 5;
		inline static float sStartSpeed1_ = 5;
		inline static float sStartSpeed2_ = 5;
		inline static std::vector<Float3>sDefaultVelocity_;
		inline static float sScale_ = 0.1f;
		float useSpeed_ = 0;
		std::vector<Float3> velocitys_;
		std::vector<Float4> colors_;
		float speed_ = 5;
		float gravity_ = 0;
		Float4 baseColor_ = { 1,1,1,1 };
		uint8_t pointLightNum_ = 0;
		uint8_t colorSetting_ = (uint8_t)ParticleColorSetting::Red;
	public:
		void Update()override;
		void StartFirework(const size_t& num);
		void SetColor(const uint8_t& colorSetting);
		static void InitDefaultVelocity();
		~FireworkChrysanthemum();

	private:
		void ColorUpdate(const float& t,const float& maxt);

	public:
#ifdef NDEBUG
#else
		virtual void ComponentDebugGUI()override;
		virtual void OutputComponent(nlohmann::json& json)override;
#endif
		virtual void LoadingComponent(nlohmann::json& json)override;
	};
}
