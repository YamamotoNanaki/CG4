#include "Lantern.h"
#include "Object3D.h"
#include "Material.h"
#include "LightManager.h"
#include "Transform.h"
#include "ImguiManager.h"

void IFE::Lantern::Initialize()
{
	if (objectPtr_->GetComponent<Material>())
	{
		objectPtr_->GetComponent<Material>()->bloom_ = true;
	}
	if (!useLightFlag_)
	{
		useLightNum_ = LightManager::GetPointLightNumber();
		if (useLightNum_ != uint8_t(-1))
		{
			useLightFlag_ = true;
			LightManager::Instance()->SetPointLightAtten(useLightNum_, { 0.0015f,0.015f,0.0015f });
			LightManager::Instance()->SetPointLightColor(useLightNum_, { 0.75f,0.35f,0 });
			LightManager::Instance()->SetPointLightActive(useLightNum_, lightFlag_);
			LightManager::Instance()->SetPointLightPos(useLightNum_, transform_->position_);
		}
	}
}

void IFE::Lantern::Update()
{
	LightManager::Instance()->SetPointLightPos(useLightNum_, transform_->position_);
#ifdef NDEBUG
#else
	LightManager::Instance()->SetPointLightActive(useLightNum_, lightFlag_);
#endif
}

#ifdef NDEBUG
#else
void IFE::Lantern::ComponentDebugGUI()
{
	ImguiManager::Instance()->CheckBoxGUI(&lightFlag_, "debug Light    On / Off");
}
#endif
