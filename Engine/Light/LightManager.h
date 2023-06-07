#pragma once
#include <d3dx12.h>
#include <memory>
#include "IFMath.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CircleShadow.h"
#include "ConstBuffer.h"

namespace IFE
{
	class LightManager
	{
	private:
		static const int32_t LightMax = 100;
		static const int32_t PLightNum = LightMax;
		static const int32_t DLightNum = LightMax;
		static const int32_t SLightNum = LightMax;
		static const int32_t CShadowNum = LightMax;

	public:
		struct ConstBufferData
		{
			Float3 ambientColor;
			float pad1;
			DLight::ConstDLightData dLights[DLightNum];
			PLight::ConstPLightData pLights[PLightNum];
			SLight::ConstSLightData sLights[PLightNum];
			CShadow::ConstCShadowData cShadows[CShadowNum];
		};

	private:
		std::unique_ptr<ConstBuffer<ConstBufferData>>constBuff;
		Float3 ambientColor = { 1,1,1 };
		DLight dLight[DLightNum];
		PLight pLight[PLightNum];
		SLight sLight[SLightNum];
		CShadow cShadow[CShadowNum];
		bool dirty = false;
		LightManager() {}
		LightManager(const LightManager&) {}
		LightManager& operator=(const LightManager&) {}
		~LightManager() {}

	public:
		void Initialize();
		void TransferConstBuffer();
		void DefaultLightSetting();
		void SetAmbientColor(const Float3& color);
		void SetDirLightActive(int32_t index, bool active);
		void SetDirLightDir(int32_t index, const Vector3& lightdir);
		void SetDirLightColor(int32_t index, const Float3& lightcolor);
		void SetPointLightActive(int32_t index, bool active);
		void SetPointLightPos(int32_t index, const Float3& lightpos);
		void SetPointLightColor(int32_t index, const Float3& lightcolor);
		void SetPointLightAtten(int32_t index, const Float3& lightAtten);
		void SetSpotLightActive(int32_t index, bool active);
		void SetSpotLightDir(int32_t index, const Vector3& lightdir);
		void SetSpotLightPos(int32_t index, const Float3& lightpos);
		void SetSpotLightColor(int32_t index, const Float3& lightcolor);
		void SetSpotLightAtten(int32_t index, const Float3& lightAtten);
		void SetSpotLightFactorAngle(int32_t index, const Float2& lightFactorAngle);
		void SetCircleShadowActive(int32_t index, bool active);
		void SetCircleShadowDir(int32_t index, const Vector3& shadowdir);
		void SetCircleShadowCasterPos(int32_t index, const Float3& shadowpos);
		void SetCircleShadowDistanceCasterLight(int32_t index, float distanceCasterLight);
		void SetCircleShadowAtten(int32_t index, const Float3& shadowAtten);
		void SetCircleShadowFactorAngle(int32_t index, const Float2& shadowFactorAngle);
		void Update();
		void Draw(UINT rootParameterIndex);
		static LightManager* Instance();
		static void Finalize();
	};
}