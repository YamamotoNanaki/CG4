#include "PlayerLantern.h"
#include "Object3D.h"
#include "Material.h"
#include "LightManager.h"
#include "Transform.h"
#include "ImguiManager.h"
#include "Transform.h"
#include "ObjectManager.h"

using namespace IFE;

void PlayerLantern::Initialize()
{
	if (objectPtr_->GetComponent<Material>())
	{
		objectPtr_->GetComponent<Material>()->bloom_ = true;
	}
	useLightNum_ = LightManager::GetPointLightNumber();
	if (useLightNum_ != uint8_t(-1))
	{
		LightManager::Instance()->SetPointLightAtten(useLightNum_, { 0.0015f,0.015f,0.0015f });
		LightManager::Instance()->SetPointLightColor(useLightNum_, { 0.75f,0.35f,0 });
		LightManager::Instance()->SetPointLightActive(useLightNum_, true);
		LightManager::Instance()->SetPointLightPos(useLightNum_, transform_->position_);
	}
	if (!transform_->parent_)
	{
		auto ptr = ObjectManager::Instance()->GetObjectPtr("Player");
		if (ptr)
		{
			transform_->parent_ = ptr->transform_;
		}
	}
}

void PlayerLantern::Draw()
{
	LightManager::Instance()->SetPointLightPos(useLightNum_, transform_->GetWorldPosition());
	LightManager::Instance()->SetConstBufferPointLight(useLightNum_);
}
