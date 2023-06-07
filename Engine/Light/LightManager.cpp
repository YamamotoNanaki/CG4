#include <cassert>
#include "LightManager.h"
#include "GraphicsAPI.h"
#include "Debug.h"

using namespace IFE;
using namespace std;

void IFE::LightManager::Initialize()
{
	constBuff = make_unique<ConstBuffer<ConstBufferData>>();

	TransferConstBuffer();
}

void IFE::LightManager::TransferConstBuffer()
{
	ConstBufferData* constMap = constBuff->GetCBMapObject();
	constMap->ambientColor = ambientColor;
	for (int32_t i = 0; i < DLightNum; i++) {
		if (dLight[i].IsActive()) {
			constMap->dLights[i].active = 1;
			constMap->dLights[i].lightv = -dLight[i].GetLightDir();
			constMap->dLights[i].lightColor = dLight[i].GetLightColor();
		}
		else {
			constMap->dLights[i].active = 0;
		}
	}
	for (int32_t i = 0; i < PLightNum; i++) {
		if (pLight[i].IsActive()) {
			constMap->pLights[i].active = 1;
			constMap->pLights[i].lightPos = pLight[i].GetLightPos();
			constMap->pLights[i].lightColor = pLight[i].GetLightColor();
			constMap->pLights[i].lightAtten = pLight[i].GetLightAtten();
		}
		else {
			constMap->pLights[i].active = 0;
		}
	}
	for (int32_t i = 0; i < SLightNum; i++) {
		if (sLight[i].IsActive()) {
			constMap->sLights[i].active = 1;
			constMap->sLights[i].lightv = -sLight[i].GetLightDir();
			constMap->sLights[i].lightPos = sLight[i].GetLightPos();
			constMap->sLights[i].lightColor = sLight[i].GetLightColor();
			constMap->sLights[i].lightAtten = sLight[i].GetLightAtten();
			constMap->sLights[i].lightFactorAngleCos = sLight[i].GetLightFactorAngle();
		}
		else {
			constMap->sLights[i].active = 0;
		}
	}
	for (int32_t i = 0; i < CShadowNum; i++) {
		if (cShadow[i].IsActive()) {
			constMap->cShadows[i].active = 1;
			constMap->cShadows[i].shadowv = -cShadow[i].GetShadowDir();
			constMap->cShadows[i].casterPos = cShadow[i].GetCasterPos();
			constMap->cShadows[i].distanceCasterLight = cShadow[i].GetDistanceCasterLight();
			constMap->cShadows[i].shadowAtten = cShadow[i].GetShadowAtten();
			constMap->cShadows[i].shadowFactorAngleCos = cShadow[i].GetShadowFactorAngle();
		}
		else {
			constMap->sLights[i].active = 0;
		}
	}
}

void IFE::LightManager::DefaultLightSetting()
{
	dLight[0].SetActive(true);
	dLight[0].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dLight[0].SetLightDir({ 0.0f, -1.0f, 0.0f });

	dLight[1].SetActive(true);
	dLight[1].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dLight[1].SetLightDir({ +0.5f, +0.1f, +0.2f });

	dLight[2].SetActive(true);
	dLight[2].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dLight[2].SetLightDir({ -0.5f, +0.1f, -0.2f });
}

void IFE::LightManager::SetAmbientColor(const Float3& color)
{
	ambientColor = color;
	dirty = true;
}

void IFE::LightManager::SetDirLightActive(int32_t index, bool active)
{
	assert(0 <= index && index < DLightNum);

	dLight[index].SetActive(active);
}

void IFE::LightManager::SetDirLightDir(int32_t index, const Vector3& lightdir)
{
	assert(0 <= index && index < DLightNum);

	dLight[index].SetLightDir({ lightdir.x,lightdir.y,lightdir.z });
	dirty = true;
}

void IFE::LightManager::SetDirLightColor(int32_t index, const Float3& lightcolor)
{
	assert(0 <= index && index < DLightNum);

	dLight[index].SetLightColor(lightcolor);
	dirty = true;
}

void IFE::LightManager::SetPointLightActive(int32_t index, bool active)
{
	assert(0 <= index && index < PLightNum);

	pLight[index].SetActive(active);
}

void IFE::LightManager::SetPointLightPos(int32_t index, const Float3& lightpos)
{
	assert(0 <= index && index < PLightNum);

	pLight[index].SetLightPos(lightpos);
	dirty = true;
}

void IFE::LightManager::SetPointLightColor(int32_t index, const Float3& lightcolor)
{
	assert(0 <= index && index < PLightNum);

	pLight[index].SetLightColor(lightcolor);
	dirty = true;
}

void IFE::LightManager::SetPointLightAtten(int32_t index, const Float3& lightAtten)
{
	assert(0 <= index && index < PLightNum);

	pLight[index].SetLightAtten(lightAtten);
	dirty = true;
}

void IFE::LightManager::SetSpotLightActive(int32_t index, bool active)
{
	assert(0 <= index && index < SLightNum);

	sLight[index].SetActive(active);
}

void IFE::LightManager::SetSpotLightDir(int32_t index, const Vector3& lightdir)
{
	assert(0 <= index && index < SLightNum);

	sLight[index].SetLightDir(lightdir);
	dirty = true;
}

void IFE::LightManager::SetSpotLightPos(int32_t index, const Float3& lightpos)
{
	assert(0 <= index && index < SLightNum);

	sLight[index].SetLightPos(lightpos);
	dirty = true;
}

void IFE::LightManager::SetSpotLightColor(int32_t index, const Float3& lightcolor)
{
	assert(0 <= index && index < SLightNum);

	sLight[index].SetLightColor(lightcolor);
	dirty = true;
}

void IFE::LightManager::SetSpotLightAtten(int32_t index, const Float3& lightAtten)
{
	assert(0 <= index && index < SLightNum);

	sLight[index].SetLightAtten(lightAtten);
	dirty = true;
}

void IFE::LightManager::SetSpotLightFactorAngle(int32_t index, const Float2& lightFactorAngle)
{
	assert(0 <= index && index < SLightNum);

	sLight[index].SetLightFactorAngle(lightFactorAngle);
	dirty = true;
}

void IFE::LightManager::SetCircleShadowActive(int32_t index, bool active)
{
	assert(0 <= index && index < CShadowNum);

	cShadow[index].SetActive(active);
}

void IFE::LightManager::SetCircleShadowDir(int32_t index, const Vector3& lightdir)
{
	assert(0 <= index && index < CShadowNum);

	cShadow[index].SetShadowDir(lightdir);
	dirty = true;
}

void IFE::LightManager::SetCircleShadowCasterPos(int32_t index, const Float3& lightpos)
{
	assert(0 <= index && index < CShadowNum);

	cShadow[index].SetCasterPos(lightpos);
	dirty = true;
}

void IFE::LightManager::SetCircleShadowDistanceCasterLight(int32_t index, float distanceCasterLight)
{
	assert(0 <= index && index < CShadowNum);

	cShadow[index].SetDistanceCasterLight(distanceCasterLight);
	dirty = true;
}

void IFE::LightManager::SetCircleShadowAtten(int32_t index, const Float3& lightAtten)
{
	assert(0 <= index && index < CShadowNum);

	cShadow[index].SetShadowAtten(lightAtten);
	dirty = true;
}

void IFE::LightManager::SetCircleShadowFactorAngle(int32_t index, const Float2& lightFactorAngle)
{
	assert(0 <= index && index < CShadowNum);

	cShadow[index].SetShadowFactorAngle(lightFactorAngle);
	dirty = true;
}


void IFE::LightManager::Update()
{
	if (dirty)
	{
		TransferConstBuffer();
		dirty = false;
	}
}

void IFE::LightManager::Draw(UINT rootParameterIndex)
{
	constBuff->SetConstBuffView(rootParameterIndex);
}

LightManager* IFE::LightManager::Instance()
{
	static LightManager instance;
	return &instance;
}
