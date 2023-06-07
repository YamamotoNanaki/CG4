#pragma once
#include "IFMath.h"

namespace IFE
{
	class DLight
	{
	public:
		struct ConstDLightData
		{
			Vector3 lightv;
			float pad;
			Float3 lightColor;
			uint32_t active;
		};

	private:
		Vector3 lightDir = { 1,0,0 };
		Float3 lightColor = { 1,1,1 };
		bool active = false;

	public:
		inline void SetLightDir(const Vector3& lightDir_)
		{
			this->lightDir = lightDir_;
			this->lightDir = Vector3Normalize(this->lightDir);
		}
		inline const Vector3& GetLightDir() { return lightDir; }
		inline void SetLightColor(const Float3& lightColor_) { this->lightColor = lightColor_; }
		inline const Float3& GetLightColor() { return lightColor; }
		inline void SetActive(bool active_) { this->active = active_; }
		inline bool IsActive() { return active; }
	};
}