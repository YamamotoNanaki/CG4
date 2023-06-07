#pragma once
#include "IFMath.h"

namespace IFE
{
	class PLight
	{
	public:
		struct ConstPLightData
		{
			Float3 lightPos;
			float pad1;
			Float3 lightColor;
			float pad2;
			Float3 lightAtten;
			uint32_t active;
		};

	private:
		Float3 lightPos = { 0,0,0 };
		Float3 lightColor = { 1,1,1 };
		Float3 lightAtten = { 1.0f,1.0f,1.0f };
		bool active = false;

	public:
		inline void SetLightPos(const Float3& lightPos_) { this->lightPos = lightPos_; }
		inline const Float3 GetLightPos() { return lightPos; }
		inline void SetLightColor(const Float3& lightColor_) { this->lightColor = lightColor_; }
		inline const Float3 GetLightColor() { return lightColor; }
		inline void SetLightAtten(const Float3& lightAtten_) { this->lightAtten = lightAtten_; }
		inline const Float3 GetLightAtten() { return lightAtten; }
		inline void SetActive(bool active_) { this->active = active_; }
		inline bool IsActive() { return active; }
	};
}