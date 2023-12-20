#include "ShadowObject.h"
#include "GraphicsPipelineManager.h"
#include "Object3D.h"
#include "CameraManager.h"
#include <d3dx12.h>
#include "GraphicsAPI.h"
#include "TextureManager.h"

void IFE::ShadowObject::Initialize()
{
	objectPtr_->gp_ = IFE::GraphicsPipelineManager::Instance()->GetGraphicsPipeline("3dShadow");

	materialBuffer_ = std::make_unique<ConstBuffer<LightVP>>();
	constMapMaterial_ = materialBuffer_->GetCBMapObject();
}

void IFE::ShadowObject::Draw()
{
	if (!drawFlag_)
	{
		drawFlag_ = true;
		return;
	}
	auto c = CameraManager::Instance()->GetCamera("shadow");
	constMapMaterial_->lightVP = c->GetView()->Get() * c->GetProjection()->Get();
	TextureManager::Instance()->GetTexture("shadow_Render_depth")->SetTexture(6);
	materialBuffer_->SetConstBuffView(7);
}

void IFE::ShadowObject::SetDrawFlag(bool flag)
{
	drawFlag_ = flag;
}

