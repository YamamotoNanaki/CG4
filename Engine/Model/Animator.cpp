#include "Animator.h"
#include "IFETime.h"
#include "Object3D.h"
#include "GraphicsPipelineManager.h"

void IFE::Animator::Initialize()
{
	skinBuffer_ = std::make_unique<ConstBuffer<ConstBufferDataSkin>>();
	constMapSkin_ = skinBuffer_->GetCBMapObject();
	objectPtr_->gp_ = GraphicsPipelineManager::Instance()->GetGraphicsPipeline("3dAnim");
	model_ = dynamic_cast<FBXModel*>(objectPtr_->model_);
}

void IFE::Animator::Draw()
{
	animTimer_ += animSpeed_ * IFETime::sDeltaTime_;
	if (animTimer_ > model_->animations_[0].duration)animTimer_ = 0;
	model_->BoneTransform(animTimer_);
	for (int i = 0; i < model_->bones_.size(); i++)
	{
		constMapSkin_->bones[i] = model_->bones_[i].finalMatrix;
	}
	skinBuffer_->SetConstBuffView(5);
}

IFE::Animator::~Animator()
{
	objectPtr_->gp_ = GraphicsPipelineManager::Instance()->GetGraphicsPipeline("3dNormal");
}
