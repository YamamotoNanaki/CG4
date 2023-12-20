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

	lightVPBuffer_ = std::make_unique<ConstBuffer<ConstBufferLightVP>>();
	constMapLightVP_ = lightVPBuffer_->GetCBMapObject();
}

void IFE::ShadowObject::Draw()
{
	if (!drawFlag_)
	{
		drawFlag_ = true;
		return;
	}
	auto c = CameraManager::Instance()->GetCamera("shadow");
	Matrix v;
	v.SetX(c->GetView()->eye_);
	v.SetY(c->GetView()->target_);
	v.SetZ(c->GetView()->up_);
	Matrix mat = v * c->GetProjection()->Get();
	constMapLightVP_->lightVP = mat;
	lightVPBuffer_->SetConstBuffView(7);
	TextureManager::Instance()->GetTexture("shadow_Render_depth")->SetTexture(6);
}

void IFE::ShadowObject::SetDrawFlag(bool flag)
{
	drawFlag_ = flag;
}

