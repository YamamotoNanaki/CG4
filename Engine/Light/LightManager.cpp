#include "LightManager.h"
#include "GraphicsAPI.h"
#include "Debug.h"
#include "Warning.h"
WarningDisabled
#include <cassert>
WarningDisabledPop

using namespace IFE;
using namespace std;


void IFE::LightManager::Initialize()
{
	constBuff_ = make_unique<ConstBuffer<ConstBufferData>>();

	TransferConstBuffer();
}

void IFE::LightManager::TransferConstBuffer()
{
	ConstBufferData* constMap = constBuff_->GetCBMapObject();
	constMap->ambientColor = ambientColor_;
	for (int32_t i = 0; i < s_DLIGHT_NUM; i++) {
		if (dLight_[i].IsActive()) {
			constMap->dLights[i].active = 1;
			constMap->dLights[i].lightv = -dLight_[i].GetLightDir();
			constMap->dLights[i].lightColor = dLight_[i].GetLightColor();
		}
		else {
			constMap->dLights[i].active = 0;
		}
	}
	for (int32_t i = 0; i < s_PLIGHT_NUM; i++) {
		if (pLight_[i].IsActive()) {
			constMap->pLights[i].active = 1;
			constMap->pLights[i].lightPos = pLight_[i].GetLightPos();
			constMap->pLights[i].lightColor = pLight_[i].GetLightColor();
			constMap->pLights[i].lightAtten = pLight_[i].GetLightAtten();
		}
		else {
			constMap->pLights[i].active = 0;
		}
	}
	for (int32_t i = 0; i < s_SLIGHT_NUM; i++) {
		if (sLight_[i].IsActive()) {
			constMap->sLights[i].active = 1;
			constMap->sLights[i].lightv = -sLight_[i].GetLightDir();
			constMap->sLights[i].lightPos = sLight_[i].GetLightPos();
			constMap->sLights[i].lightColor = sLight_[i].GetLightColor();
			constMap->sLights[i].lightAtten = sLight_[i].GetLightAtten();
			constMap->sLights[i].lightFactorAngleCos = sLight_[i].GetLightFactorAngle();
		}
		else {
			constMap->sLights[i].active = 0;
		}
	}
	for (int32_t i = 0; i < s_CSHADOW_NUM; i++) {
		if (cShadow_[i].IsActive()) {
			constMap->cShadows[i].active = 1;
			constMap->cShadows[i].shadowv = -cShadow_[i].GetShadowDir();
			constMap->cShadows[i].casterPos = cShadow_[i].GetCasterPos();
			constMap->cShadows[i].distanceCasterLight = cShadow_[i].GetDistanceCasterLight();
			constMap->cShadows[i].shadowAtten = cShadow_[i].GetShadowAtten();
			constMap->cShadows[i].shadowFactorAngleCos = cShadow_[i].GetShadowFactorAngle();
		}
		else {
			constMap->sLights[i].active = 0;
		}
	}
}

void IFE::LightManager::DefaultLightSetting()
{
	dLight_[0].SetActive(true);
	dLight_[0].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dLight_[0].SetLightDir({ 0.0f, -1.0f, 0.0f });

	dLight_[1].SetActive(true);
	dLight_[1].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dLight_[1].SetLightDir({ +0.5f, +0.1f, +0.2f });

	dLight_[2].SetActive(true);
	dLight_[2].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dLight_[2].SetLightDir({ -0.5f, +0.1f, -0.2f });

	dirty_ = true;
}

void IFE::LightManager::SetAmbientColor(const Float3& color)
{
	ambientColor_ = color;
	dirty_ = true;
}

void IFE::LightManager::SetDirLightActive(int32_t index, bool active)
{
	assert(0 <= index && index < s_DLIGHT_NUM);

	dLight_[index].SetActive(active);
	dirty_ = true;
}

void IFE::LightManager::SetDirLightDir(int32_t index, const Vector3& lightdir)
{
	assert(0 <= index && index < s_DLIGHT_NUM);

	dLight_[index].SetLightDir({ lightdir.x,lightdir.y,lightdir.z });
	dirty_ = true;
}

void IFE::LightManager::SetDirLightColor(int32_t index, const Float3& lightcolor)
{
	assert(0 <= index && index < s_DLIGHT_NUM);

	dLight_[index].SetLightColor(lightcolor);
	dirty_ = true;
}

void IFE::LightManager::SetPointLightActive(int32_t index, bool active)
{
	assert(0 <= index && index < s_PLIGHT_NUM);

	pLight_[index].SetActive(active);
	dirty_ = true;
}

void IFE::LightManager::SetPointLightPos(int32_t index, const Float3& lightpos)
{
	assert(0 <= index && index < s_PLIGHT_NUM);

	pLight_[index].SetLightPos(lightpos);
	dirty_ = true;
}

void IFE::LightManager::SetPointLightColor(int32_t index, const Float3& lightcolor)
{
	assert(0 <= index && index < s_PLIGHT_NUM);

	pLight_[index].SetLightColor(lightcolor);
	dirty_ = true;
}

void IFE::LightManager::SetPointLightAtten(int32_t index, const Float3& lightAtten)
{
	assert(0 <= index && index < s_PLIGHT_NUM);

	pLight_[index].SetLightAtten(lightAtten);
	dirty_ = true;
}

void IFE::LightManager::SetSpotLightActive(int32_t index, bool active)
{
	assert(0 <= index && index < s_SLIGHT_NUM);

	sLight_[index].SetActive(active);
	dirty_ = true;
}

void IFE::LightManager::SetSpotLightDir(int32_t index, const Vector3& lightdir)
{
	assert(0 <= index && index < s_SLIGHT_NUM);

	sLight_[index].SetLightDir(lightdir);
	dirty_ = true;
}

void IFE::LightManager::SetSpotLightPos(int32_t index, const Float3& lightpos)
{
	assert(0 <= index && index < s_SLIGHT_NUM);

	sLight_[index].SetLightPos(lightpos);
	dirty_ = true;
}

void IFE::LightManager::SetSpotLightColor(int32_t index, const Float3& lightcolor)
{
	assert(0 <= index && index < s_SLIGHT_NUM);

	sLight_[index].SetLightColor(lightcolor);
	dirty_ = true;
}

void IFE::LightManager::SetSpotLightAtten(int32_t index, const Float3& lightAtten)
{
	assert(0 <= index && index < s_SLIGHT_NUM);

	sLight_[index].SetLightAtten(lightAtten);
	dirty_ = true;
}

void IFE::LightManager::SetSpotLightFactorAngle(int32_t index, const Float2& lightFactorAngle)
{
	assert(0 <= index && index < s_SLIGHT_NUM);

	sLight_[index].SetLightFactorAngle(lightFactorAngle);
	dirty_ = true;
}

void IFE::LightManager::SetCircleShadowActive(int32_t index, bool active)
{
	assert(0 <= index && index < s_CSHADOW_NUM);

	cShadow_[index].SetActive(active);
	dirty_ = true;
}

void IFE::LightManager::SetCircleShadowDir(int32_t index, const Vector3& lightdir)
{
	assert(0 <= index && index < s_CSHADOW_NUM);

	cShadow_[index].SetShadowDir(lightdir);
	dirty_ = true;
}

void IFE::LightManager::SetCircleShadowCasterPos(int32_t index, const Float3& lightpos)
{
	assert(0 <= index && index < s_CSHADOW_NUM);

	cShadow_[index].SetCasterPos(lightpos);
	dirty_ = true;
}

void IFE::LightManager::SetCircleShadowDistanceCasterLight(int32_t index, float distanceCasterLight)
{
	assert(0 <= index && index < s_CSHADOW_NUM);

	cShadow_[index].SetDistanceCasterLight(distanceCasterLight);
	dirty_ = true;
}

void IFE::LightManager::SetCircleShadowAtten(int32_t index, const Float3& lightAtten)
{
	assert(0 <= index && index < s_CSHADOW_NUM);

	cShadow_[index].SetShadowAtten(lightAtten);
	dirty_ = true;
}

void IFE::LightManager::SetCircleShadowFactorAngle(int32_t index, const Float2& lightFactorAngle)
{
	assert(0 <= index && index < s_CSHADOW_NUM);

	cShadow_[index].SetShadowFactorAngle(lightFactorAngle);
	dirty_ = true;
}


void IFE::LightManager::Update()
{
	if (nextPNum_ >= s_PLIGHT_NUM)
	{
		nextPNum_ = 0;
	}
	if (dirty_)
	{
		TransferConstBuffer();
		dirty_ = false;
	}
}

void IFE::LightManager::Draw(uint32_t rootParameterIndex)
{
	constBuff_->SetConstBuffView(rootParameterIndex);
}

LightManager* IFE::LightManager::Instance()
{
	static LightManager instance;
	return &instance;
}

#ifdef NDEBUG
#else
#include "ImguiManager.h"
void IFE::LightManager::DebugGUI()
{
	auto imgui = ImguiManager::Instance();
	imgui->NewGUI("LightManager");
	if (imgui->CollapsingHeaderGUI("Directional Light"))
	{
		static int32_t num = 0;
		imgui->DragIntGUI(&num, "select number", 1.0f, 0, s_DLIGHT_NUM);
	}
	if (imgui->CollapsingHeaderGUI("Point Light"))
	{
		static int32_t num = 0;
		imgui->DragIntGUI(&num, "select number", 1.0f, 0, s_PLIGHT_NUM);
		bool active = pLight_[num].IsActive();
		Float3 pos = pLight_[num].GetLightPos();
		Float3 atten = pLight_[num].GetLightAtten();
		Float3 color = pLight_[num].GetLightColor();

		imgui->CheckBoxGUI(&active, "active");
		imgui->DragFloat3GUI(&pos, "pos");
		imgui->DragFloat3GUI(&atten, "atten");
		imgui->ColorEdit3GUI(&color, "color");

		SetPointLightActive(num, active);
		SetPointLightAtten(num, atten);
		SetPointLightColor(num, color);
		SetPointLightPos(num, pos);
	}
	if (imgui->CollapsingHeaderGUI("Spot Light"))
	{
		static int32_t num = 0;
		imgui->DragIntGUI(&num, "select number", 1.0f, 0, s_SLIGHT_NUM);
	}
	imgui->EndGUI();
}
#endif