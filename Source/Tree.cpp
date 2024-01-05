#include "Tree.h"
#include "ModelManager.h"
#include "Rand.h"
#include "Object3D.h"
#include "Transform.h"
#include "FBXModel.h"

void Tree::Initialize()
{
	auto num = IFE::IFERand::GetRand(0, 6);
	switch (num)
	{
	case 0:
		objectPtr_->SetModel(IFE::ModelManager::Instance()->GetModel("Bush1"));
		transform_->scale_ = { 20,20,20 };
		transform_->eulerAngleDegrees_.x = 270;
		break;
	case 1:
		objectPtr_->SetModel(IFE::ModelManager::Instance()->GetModel("Bush2"));
		transform_->scale_ = { 15,15,15 };
		transform_->eulerAngleDegrees_.x = 270;
		break;
	case 2:
		objectPtr_->SetModel(IFE::ModelManager::Instance()->GetModel("Bush3"));
		transform_->scale_ = { 8,8,8 };
		transform_->eulerAngleDegrees_.x = 270;
		break;
	case 3:
		objectPtr_->SetModel(IFE::ModelManager::Instance()->GetModel("Tree1"));
		transform_->scale_ = { 8,8,8 };
		transform_->eulerAngleDegrees_.x = 270;
		break;
	case 4:
		objectPtr_->SetModel(IFE::ModelManager::Instance()->GetModel("Tree2"));
		transform_->scale_ = { 8,8,8 };
		transform_->eulerAngleDegrees_.x = 270;
		break;
	case 5:
		objectPtr_->SetModel(IFE::ModelManager::Instance()->GetModel("Tree3"));
		transform_->scale_ = { 5,5,5 };
		transform_->eulerAngleDegrees_.x = 270;
		break;
	default:
		objectPtr_->SetModel(IFE::ModelManager::Instance()->GetModel("Tree4"));
		transform_->scale_ = { 5,5,5 };
		transform_->eulerAngleDegrees_.x = 270;
		break;
	}
	auto m = dynamic_cast<IFE::FBXModel*>(objectPtr_->GetModel());
	if (!m)return;
	const float c = 9.f / 255.f;
	for (auto& itr : m->nodes_)
	{
		for (auto itr2 : itr->meshes)
		{
			auto mat = itr2->GetMaterial();
			mat.color = { c,c,c,1 };
			itr2->SetMaterial(mat);
		}
	}
}
